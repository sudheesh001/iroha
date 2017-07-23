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

#ifndef IROHA_COMMIT_HPP
#define IROHA_COMMIT_HPP

#include <consensus.pb.h>
#include "ledger_state.hpp"
#include <model/message.hpp>
#include "signature.hpp"

namespace consensus {
  namespace model {

    class Commit final : public ::model::Message<const proto::Commit> {
     public:
      Commit(const proto::Commit *ptr);

      bool is_schema_valid() const noexcept override;

      const LedgerState commit_state;
      const std::vector<Signature> sigs;
      const std::vector<std::string> transactions;
    };
  }
}

#endif  // IROHA_COMMIT_HPP
