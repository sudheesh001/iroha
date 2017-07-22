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

#ifndef IROHA_LEADER_HPP
#define IROHA_LEADER_HPP

#include <consensus/model/commit.hpp>
#include <consensus/model/proposal.hpp>
#include "validator.hpp"

namespace consensus {

  namespace role {

    class Leader final : public Validator {
     public:
      Leader();

      Role self() override;
      virtual void on_proposal(const model::Proposal &proposal);
      virtual void on_commit(const model::Commit &commit);
      virtual void on_vote(const model::Vote &vote);
      virtual void on_abort(const model::Abort &abort);
    };
  }
}

#endif  // IROHA_LEADER_HPP