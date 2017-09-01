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

#include <commands.pb.h>
#include <common/uint256_t.h>
#include <libiroha/basic/types.hpp>
#include <string>
#include "command.hpp"

namespace iroha {
  class TransferAsset : public Command {
   public:
    /**
     * Build view from protobuf
     * @param p
     */
    explicit TransferAsset(std::unique_ptr<protocol::TransferAsset> p) : t{std::move(p)} {}

    /**
     * Build protobuf object from arguments
     * @param from
     * @param to
     * @param asset_id
     * @param amount
     */
    explicit TransferAsset(const std::string &from, const std::string &to,
                           const std::string &asset_id,
                           const uint256_t &amount);

    const std::string from() const { return t->src_account_id(); }

    const std::string to() const { return t->dest_account_id(); }

    const std::string asset_id() const { return t->asset_id(); }

    /* decide how to store amount
    const uint256_t amount() const {
      return // amount;
    }
     */

    void execute(Executor &e) final {
      e.execute(*this);
    }

    TransferAsset() = delete;

   private:
    friend class Transaction;

    void register_cmd(iroha::protocol::Command *cmd) override {
      cmd->set_allocated_transfer_asset(t.release());
    }

    std::unique_ptr<protocol::TransferAsset> t;

  };

}
