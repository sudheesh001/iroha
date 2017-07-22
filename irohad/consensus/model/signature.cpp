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

#include "signature.hpp"

namespace consensus {
  namespace model {

    bool Signature::is_schema_valid() const noexcept {
      return this->proto_->pubkey().size() == pubkey_t::size() &&
             this->proto_->signature().size() == signature_t::size() &&
             this->proto_->timestamp() > 0;
    }

    const pubkey_t Signature::pubkey() const {
      return iroha::to_blob<pubkey_t::size()>(proto_->pubkey());
    }

    const signature_t Signature::signature() const {
      return iroha::to_blob<signature_t::size()>(proto_->signature());
    }

    const ts64_t Signature::timestamp() const noexcept {
      return this->proto_->timestamp();
    }

    Signature::Signature(const proto::Signature *ptr) : Message(ptr) {}

    bool Signature::verify(const std::vector<uint8_t> &msg) const noexcept {
      return iroha::verify(msg.data(), msg.size(), pubkey(), signature());
    }

    bool Signature::verify(const std::string &msg) const noexcept {
      return iroha::verify(msg, pubkey(), signature());
    }
  }
}