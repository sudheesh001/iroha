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

#include <block.pb.h>
#include <libiroha/command/commands.hpp>

namespace iroha {
  namespace view {

    class Transaction {
     public:
      explicit Transaction(std::unique_ptr<protocol::Transaction> tx)
          : tx_(std::move(tx)), pl{tx_->payload()} {
        for (const auto &p : pl.commands()) {
          using protocol::Command::CommandCase;
          switch (p.command_case()) {
            case CommandCase::kTransferAsset: {
              cmds.push_back(
                  std::make_unique<TransferAsset>(p.transfer_asset()));
              break;
            }
            // TODO: add others
            default: {
              /// bad command ///
            }
          }
        }
      }

      const std::string creator_account_id() const noexcept {
        return pl.creator_account_id();
      }

      const uint64_t tx_counter() const noexcept { return pl.tx_counter(); }

      const ts64_t created_time() const noexcept { return pl.created_time(); }

      void execute(Executor &e) {
        for (const auto &c : cmds) {
          c->execute(e);
        }
      }

     private:
      std::unique_ptr<protocol::Transaction> tx_;
      const protocol::Transaction_Payload &pl;

      std::vector<std::unique_ptr<Command>> cmds{};
    };

    void f() {
      std::unique_ptr<protocol::Transaction> pb;
      Transaction tx(pb);

      Executor e;
      tx.execute(e);
    }
  }
}
