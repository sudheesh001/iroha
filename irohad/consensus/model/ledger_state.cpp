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

bool consensus::LedgerState::is_schema_valid() const {
  bool valid = this->proto_->gmroot().size() == hash256_t::size();
  return valid;
}

const uint64_t consensus::LedgerState::height() const {
  return this->proto_->height();
}

const hash256_t consensus::LedgerState::gmroot() const {
  return to_blob<hash256_t::size()>(this->proto_->gmroot());
}

consensus::LedgerState::LedgerState(const proto::consensus::LedgerState *ptr)
    : Message(ptr) {}
