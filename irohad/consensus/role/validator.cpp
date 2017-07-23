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

#include "validator.hpp"

namespace consensus {
  namespace role {
    Role Validator::self() { return Role::VALIDATOR; }
    Validator::Validator(consensus::Consensus &consensus,
                         peerservice::PeerServiceImpl &ps,
                         std::atomic<bool> &round_started)
        : Member(consensus, ps, round_started) {}

    void Validator::on_proposal(const model::Proposal &proposal) {
      // TODO: validate proposal and send vote to proxy tail
    }

    void Validator::on_commit(const model::Commit &commit) {
      // commit is the  same as for member
      Member::on_commit(commit);
    }

    void Validator::on_vote(const model::Vote &vote) {
      // ignore votes
    }

    void Validator::on_abort(const model::Abort &abort) {
      // abort processing is the same as for member
      Member::on_abort(abort);
    }
  }
}