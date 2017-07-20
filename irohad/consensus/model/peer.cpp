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

consensus::Peer::Peer(const proto::consensus::Peer *ptr) : Message(ptr) {}

bool consensus::Peer::is_schema_valid() const {
  // len(0.0.0.0) = 7, len(255.255.255.255) = 15
  bool valid = this->proto_->ip().size() >= 7;
  valid &= this->proto_->ip().size() <= 15;

  valid &= this->proto_->port() >= 1;
  valid &= this->proto_->port() <= 65535;

  valid &= this->proto_->pubkey().size() == pubkey_t::size();

  return valid;
}

const pubkey_t consensus::Peer::pubkey() const {
  return to_blob<pubkey_t::size()>(this->proto_->pubkey());
}

const auto consensus::Peer::port() const {
  return (uint16_t)this->proto_->port();
}

const auto consensus::Peer::ip() const { return this->proto_->ip(); }
