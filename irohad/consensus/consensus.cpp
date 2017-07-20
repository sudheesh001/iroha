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

#include <consensus/model/abort.hpp>
#include <consensus/model/commit.hpp>
#include <consensus/model/proposal.hpp>
#include <consensus/model/view.hpp>

using grpc::Status;
using grpc::ServerContext;

namespace consensus {

  Status Consensus::SendProposal(ServerContext *context,
                                 const proto::Proposal *request,
                                 proto::Void *response) {
    model::Proposal proposal(request);
    if (!proposal.is_schema_valid()) return Status::CANCELLED;

    return Status::OK;
  }

  Status Consensus::SendVote(ServerContext *context, const proto::Vote *request,
                             proto::Void *response) {
    model::Vote vote(request);
    if (!vote.is_schema_valid()) return Status::CANCELLED;

    return Status::OK;
  }

  Status Consensus::SendCommit(grpc::ServerContext *context,
                               const proto::Commit *request,
                               proto::Void *response) {
    model::Commit commit(request);
    if (!commit.is_schema_valid()) return Status::CANCELLED;

    return Status::OK;
  }

  Status Consensus::SendAbort(grpc::ServerContext *context,
                              const proto::Abort *request,
                              proto::Void *response) {
    model::Abort abort(request);
    if (!abort.is_schema_valid()) return Status::CANCELLED;

    return Status::OK;
  }

  Status Consensus::GetView(ServerContext *context, const proto::Void *request,
                            proto::View *response) {
    // return my view
    return Status::OK;
  }

  Status Consensus::SetView(ServerContext *context, const proto::View *request,
                            proto::Void *response) {
    model::View view(request);
    if (!view.is_schema_valid()) return Status::CANCELLED;

    return Status::OK;
  }
}