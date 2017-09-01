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

#ifndef IROHA_BUILDER_TRANSACTION_HPP
#define IROHA_BUILDER_TRANSACTION_HPP

#include <common/types.hpp>
#include <crypto/crypto.hpp>
#include <crypto/hash.hpp>
#include <libiroha/basic/signable.hpp>
#include <libiroha/command/commands.hpp>
#include <string>
#include <vector>
#include "block.pb.h"

/**
 * Registers new command.
 *  - command is the name of interface method
 *  - type is the according interface (model) type
 */
#define REGISTER_COMMAND(command, type)                                      \
  Transaction& command(type arg) {                                           \
    if (final)                                                               \
      throw TransactionFinalException("transaction is immutable and final"); \
    arg.register_cmd(p.mutable_payload()->add_commands());                   \
    return *this;                                                            \
  }

namespace iroha {
  namespace builder {

    class Transaction final : public basic::Signable {
     public:
      using TransactionFinalException = std::domain_error;


      REGISTER_COMMAND(add_asset_quantity, AddAssetQuantity);
      REGISTER_COMMAND(add_peer, AddPeer);
      REGISTER_COMMAND(add_signatory, AddSignatory);
      REGISTER_COMMAND(create_asset, CreateAsset);
      REGISTER_COMMAND(create_account, CreateAccount);
      REGISTER_COMMAND(create_domain, CreateDomain);
      REGISTER_COMMAND(remove_signatory, RemoveSignatory);
      REGISTER_COMMAND(set_account_permissions, SetAccountPermissions);
      REGISTER_COMMAND(set_account_quorum, SetAccountQuorum);
      REGISTER_COMMAND(transfer_asset, TransferAsset);

      basic::Signable& sign(iroha::keypair_t kp) noexcept override;

     private:
      iroha::protocol::Transaction p;

      bool final{};
      std::string payload_hash;
    };
  }  // namespace builder
}  // namespace iroha

void f() {
  using namespace iroha;
  using namespace builder;

  keypair_t kp1 = create_keypair(create_seed("hooi pidr1"));
  keypair_t kp2 = create_keypair(create_seed("hooi pidr2"));

  Transaction builder;
  builder.add_peer({"1", "2"})
      .transfer_asset({"from", "to", "usd", 666})
      .sign(kp1)
      .sign(kp2);
  /*
  .transfer_asset()
    .set_asset_id("usd#me")
    .set_from("ppc@me")
    .set_to("ppz@me")
    .set_amount(256)
    .finish();
    */
}

#endif  // IROHA_BUILDER_TRANSACTION_HPP
