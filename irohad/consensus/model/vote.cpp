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

#include "vote.hpp"

bool consensus::Vote::is_schema_valid() const {
  return next_state.is_schema_valid() && sig.is_schema_valid();
}

consensus::Vote::Vote(const proto::consensus::Vote *ptr)
    : Message(ptr),
      next_state{&this->proto_->next_state()},
      sig{&this->proto_->sig()} {}
