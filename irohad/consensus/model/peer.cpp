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

#include "peer.hpp"
#include <regex>

namespace consensus {
  namespace model {

    Peer::Peer(const proto::Peer *ptr) : Message(ptr) {}

    const auto Peer::address() const noexcept {
      return this->proto_->address();
    }

    bool Peer::is_schema_valid() const noexcept {
      bool valid = this->proto_->pubkey().size() == pubkey_t::size();

      // regex for ip:port
      std::regex reg(
          "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-"
          "9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\:([0-9]{1,4}|[1-5][0-9]{4}|"
          "6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])");

      valid &= std::regex_match(this->address(), reg);

      return valid;
    }

    const pubkey_t Peer::pubkey() const {
      return iroha::to_blob<pubkey_t::size()>(this->proto_->pubkey());
    }

    const std::vector<uint8_t> Peer::bytes() const noexcept {
      auto &&addr = address();
      std::vector<uint8_t> b{addr.begin(), addr.end()};

      auto &&pubk = pubkey();
      std::copy(pubk.begin(), pubk.end(), std::back_inserter(b));

      return b;
    }
  }
}