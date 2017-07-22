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

namespace consensus {
  namespace model {

    View::View(const proto::View *ptr)
        : Message(ptr),
          peer{this->proto_->peer().begin(), this->proto_->peer().end()},
          sig{&this->proto_->sig()} {}

    bool View::is_schema_valid() const {
      if (this->proto_->peer_size() <= 0) return false;
      for (auto &&p : peer) {
        if (!p.is_schema_valid()) return false;
      }

      return true;
    }

    bool View::is_signature_valid() const noexcept {
      // TODO
      // peer1.bytes() + peer2.bytes() + ... + peerN.bytes() + timestamp
      return true;
      //return sig.verify(this->proto_->);
    }
    uint64_t View::view_id() const noexcept { return this->proto_->view_id(); }
  }
}