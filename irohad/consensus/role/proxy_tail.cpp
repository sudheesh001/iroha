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

#include "proxy_tail.hpp"

namespace consensus {
  namespace role {

    Role ProxyTail::self() { return Role::PROXY_TAIL; }
    ProxyTail::ProxyTail(peerservice::PeerServiceImpl &ps, std::atomic<bool> &round_started) : Validator(ps,
                                                                                                         round_started) {}
    void ProxyTail::on_proposal(const model::Proposal &proposal) {
      // TODO validate proposal and send vote
    }

    void ProxyTail::on_commit(const model::Commit &commit) {
      // proxy tail does the same action with commit as validator
      Validator::on_commit(commit);
      //TODO: shouldn't the proxy tail be sending commits and NEVER receive a commit in the current view (the answer depends on whether or not a non-leader fault is treated as a view change or not)
    }

    void ProxyTail::on_vote(const model::Vote &vote) {
      // TODO: parse and save vote inside proxy_tail role (this)
    }

    void ProxyTail::on_abort(const model::Abort &abort) {
      // abort logic is the same as in validator
      Validator::on_abort(abort);
    }

  }
}