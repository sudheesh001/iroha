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

#ifndef IROHA_LEDGER_STATE_HPP
#define IROHA_LEDGER_STATE_HPP

#include <consensus.pb.h>
#include <model/message.hpp>

namespace consensus {
  namespace model {

    class LedgerState final : public model::Message<const proto::LedgerState> {
     public:
      LedgerState(const proto::LedgerState *ptr);

      bool is_schema_valid() const noexcept override;

      /**
       * Returns global merkle root.
       * @return
       */
      const hash256_t gmroot() const;

      /**
       * Returns ledger height.
       * @return
       */
      const uint64_t height() const noexcept;

      /**
       * Serialize
       * <gmroot + height>
       * @return
       */
      const std::vector<uint8_t> bytes() const noexcept;
    };
  }
}

#endif  // IROHA_LEDGER_STATE_HPP
