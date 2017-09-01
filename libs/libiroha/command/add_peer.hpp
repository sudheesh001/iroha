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

#pragma once

#include <common/types.hpp>
#include <string>
#include "command.hpp"

namespace iroha {

  using pubkey_t = iroha::pubkey_t;

  class AddPeer : public Command<iroha::protocol::AddPeer> {
   public:
    AddPeer(std::string address, pubkey_t pubkey)
        : address_{std::move(address)}, pubkey_{pubkey_} {}
    AddPeer(std::string address, const std::string& pubkey)
        : address_{std::move(address)} {
      pubkey_ = pubkey_t::from_string(pubkey);
    }

    void register_cmd(iroha::protocol::Command* cmd) override {
      cmd->set_allocated_add_peer(t.release());
    }

   private:
    std::string address_;
    pubkey_t pubkey_;
  };
}
