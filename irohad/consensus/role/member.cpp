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

#include "consensus/role/member.hpp"
#include "consensus/consensus.hpp"
#include "model/block.hpp"
#include "block.pb.h"

namespace consensus {
  namespace role {
    Role Member::self() { return Role::MEMBER; }
    Member::Member(consensus::Consensus &consensus,
                   peerservice::PeerServiceImpl &ps,
                   std::atomic<bool> &round_started)
        : consensus_(consensus), ps_(ps), round_started_(round_started) {}

    void Member::on_commit(const model::Commit &commit) {
      auto block = iroha::model::Block();
      for (const auto &s : commit.sigs){
        block.sigs.emplace_back(s.signature(), s.pubkey());
      }
      block.merkle_root = commit.commit_state.gmroot();
      block.height = commit.commit_state.height();
      for (const auto &tx : commit.transactions){
        auto pb_tx = iroha::protocol::Transaction();
        auto res = pb_tx.ParseFromString(tx);
        if (!res) {
          // TODO handle parse failure
        }
        block.transactions.push_back(txFactory_.deserialize(pb_tx));
      }
      consensus_.on_next(std::move(block));
    }

    void Member::on_proposal(const model::Proposal &proposal) {
      // member ignores proposals
    }
    void Member::on_vote(const model::Vote &vote) {
      // member ignores votes
    }

    void Member::on_abort(const model::Abort &abort) {
      // TODO handle consensus failure
    }
  }
}