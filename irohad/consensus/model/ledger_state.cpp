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

#include "ledger_state.hpp"

namespace consensus {
  namespace model {

    bool LedgerState::is_schema_valid() const noexcept {
      return this->proto_->gmroot().size() == hash256_t::size();
    }

    const uint64_t LedgerState::height() const noexcept {
      return this->proto_->height();
    }

    const hash256_t LedgerState::gmroot() const {
      return iroha::to_blob<hash256_t::size()>(this->proto_->gmroot());
    }

    LedgerState::LedgerState(const proto::LedgerState *ptr) : Message(ptr) {}

    const std::vector<uint8_t> LedgerState::bytes() const noexcept {
      constexpr size_t size_ = hash256_t::size() + sizeof(uint64_t);

      auto &&rootbegin = proto_->gmroot().begin();
      auto &&rootend = proto_->gmroot().end();

      uint64_t h_ = this->height();
      uint8_t *h_ptr = (uint8_t *)h_;

      std::array<uint8_t, size_> blob{};

      std::copy(rootbegin, rootend, &blob[0]);
      std::copy(h_ptr, h_ptr + sizeof(uint64_t), &blob[hash256_t::size()]);

      return std::vector<uint8_t>{blob.begin(), blob.end()};
    }
  }
}
