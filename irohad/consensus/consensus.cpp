/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "consensus.hpp"
#include "block.pb.h"
#include "consensus.pb.h"
#include "consensus/role/member.hpp"
#include "model/abort.hpp"
#include "model/commit.hpp"
#include "model/proposal.hpp"
#include "model/view.hpp"

using grpc::Status;
using grpc::ServerContext;

namespace consensus {

  Consensus::Consensus(iroha::simulator::BlockCreator &block_creator,
                       peerservice::PeerServiceImpl &chain,
                       iroha::model::ModelCryptoProvider &crypto_provider)
      : chain_(chain), crypto_provider_(crypto_provider) {
    block_creator.on_block().subscribe([this](auto block) {
      this->vote(block);
    });
  }

  Status Consensus::SendProposal(ServerContext *context,
                                 const proto::Proposal *request,
                                 proto::Void *response) {
    model::Proposal proposal(request);
    if (!proposal.is_schema_valid()) return Status::CANCELLED;
    auto pb_proposal = proposal.proto();

    // signature verification
    for (auto bytes_tx : pb_proposal->transactions()) {
      iroha::protocol::Transaction pb_tx;
      pb_tx.ParseFromString(bytes_tx);
      auto tx = tx_factory_.deserialize(pb_tx);
      if (!crypto_provider_.verify(tx)) return Status::CANCELLED;
    }
    this->role_->on_proposal(proposal);

    return Status::OK;
  }

  Status Consensus::SendVote(ServerContext *context, const proto::Vote *request,
                             proto::Void *response) {
    model::Vote vote(request);
    if (!vote.is_schema_valid()) return Status::CANCELLED;

    // signature verification
    if (!vote.is_signature_valid()) return Status::CANCELLED;
    this->role_->on_vote(vote);

    return Status::OK;
  }

  Status Consensus::SendCommit(grpc::ServerContext *context,
                               const proto::Commit *request,
                               proto::Void *response) {
    model::Commit commit(request);
    if (!commit.is_schema_valid()) return Status::CANCELLED;

    // signature verification
    auto str_commit = commit.proto()->commit_state().SerializeAsString();
    for (const auto &sig : commit.sigs) {
      if (!sig.verify(str_commit + std::to_string(sig.timestamp()))) {
        return Status::CANCELLED;
      }
    }
    this->role_->on_commit(commit);

    return Status::OK;
  }

  Status Consensus::SendAbort(grpc::ServerContext *context,
                              const proto::Abort *request,
                              proto::Void *response) {
    model::Abort abort(request);
    if (!abort.is_schema_valid()) {
      return Status::CANCELLED;
    }
    // TODO signature verification
    return Status::OK;
  }

  Status Consensus::GetView(ServerContext *context, const proto::Void *request,
                            proto::View *response) {
    response->CopyFrom(*chain_);
    return Status::OK;
  }

  Status Consensus::SetView(ServerContext *context, const proto::View *request,
                            proto::Void *response) {
    model::View view(request);
    if (!view.is_schema_valid()) return Status::CANCELLED;

    return Status::OK;
  }

  void Consensus::vote(iroha::model::Block block) {
    grpc::ClientContext context;
    proto::Vote request;
    proto::Void response;
    // TODO sign block
    auto next_state = request.mutable_next_state();
    next_state->set_gmroot(block.merkle_root.data(), block.merkle_root.size());
    next_state->set_height(block.height);
    auto sig = request.mutable_sig();
//    sig->set_pubkey();
//    sig->set_signature();
//    sig->set_timestamp();
    auto res = chain_.proxy_tail()->SendVote(&context, request, &response);
    if (!res.ok()){
      // TODO handle error
    }
  }

  rxcpp::observable<iroha::model::Block> Consensus::on_commit() {
    return commits_.get_observable();
  }

  void Consensus::on_next(iroha::model::Block &&block) {
    commits_.get_subscriber().on_next(std::forward<iroha::model::Block>(block));
  }
}