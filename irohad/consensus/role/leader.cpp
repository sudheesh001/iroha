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

#include "leader.hpp"

namespace consensus {
  namespace role {
    Role Leader::self() { return Role::LEADER; }
    Leader::Leader(consensus::Consensus &consensus,
                   peerservice::PeerServiceImpl &ps,
                   std::atomic<bool> &round_started)
        : Validator(consensus, ps, round_started) {}

    void Leader::on_proposal(const model::Proposal &proposal) {
      // TODO
      // leader receives proposal from ordering service and sends it to
      // chain[1]...chain[2*chain.max_faulty() + 1] peers
    }

    void Leader::on_commit(const model::Commit &commit) {
      // leader does the same action with commit as validator
      Validator::on_commit(commit);
    }

    void Leader::on_vote(const model::Vote &vote) {
      // leader ignores votes
    }

    void Leader::on_abort(const model::Abort &abort) {
      // leader does the same action with abort as validator
      Validator::on_abort(abort);
    }
  }
}