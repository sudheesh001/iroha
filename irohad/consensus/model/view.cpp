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

#include "view.hpp"

consensus::View::View(const proto::consensus::View *ptr)
    : Message(ptr),
      view{this->proto_->view().begin(), this->proto_->view().end()},
      sig{&this->proto_->sig()} {}

bool consensus::View::is_schema_valid() const {
  if (this->proto_->view_size() <= 0) return false;
  for (auto &&peer : view) {
    if (!peer.is_schema_valid()) return false;
  }

  return true;
}
