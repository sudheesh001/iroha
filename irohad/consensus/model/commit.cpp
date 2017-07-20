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

#include "commit.hpp"

bool consensus::Commit::is_schema_valid() const {
  if (this->proto_->transactions_size() <= 0) return false;
  if (this->proto_->sigs_size() <= 0) return false;

  for (auto&& sig : sigs) {
    if (!sig.is_schema_valid()) {
      return false;
    }
  }

  return true;
}

consensus::Commit::Commit(const proto::consensus::Commit* ptr)
    : Message(ptr),
      commit_state{&this->proto_->commit_state()},
      sigs{this->proto_->sigs().begin(), this->proto_->sigs().end()} {}
