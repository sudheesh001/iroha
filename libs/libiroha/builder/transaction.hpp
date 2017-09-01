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
#include <string>
#include <vector>
#include "block.pb.h"
#include "builder/command/commands.hpp"
#include "signable.hpp"

/**
 * Registers new command.
 *  - command is the name of interface method
 *  - type is the according interface (model) type
 */
#define REGISTER_COMMAND(command, type)             \
  Transaction& command(type arg) {                  \
    auto cmd = p.mutable_payload()->add_commands(); \
    arg.register_cmd(cmd);                          \
    return *this;                                   \
  }

namespace builder {

  class Transaction final : public Signable {
   public:
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

    Signable& sign(iroha::keypair_t kp) noexcept override {
      if (!cache.final) {
        cache.final = true;
        cache.payload_hash =
            iroha::sha3_256(p.payload().SerializeAsString()).to_string();
      }

      const iroha::sig_t signature =
          iroha::sign(cache.payload_hash, kp.pubkey, kp.privkey);

      auto sig = p.add_signature();
      sig->set_pubkey(kp.pubkey.to_string());
      sig->set_signature(signature.to_string());

      return *this;
    }

   private:
    iroha::protocol::Transaction p;

    struct {
      bool final{};
      std::string payload_hash;
    } cache;

    void a() {}
  };
}

void f() {
  using namespace builder;

  iroha::keypair_t kp1 =
      iroha::create_keypair(iroha::create_seed("hooi pidr1"));
  iroha::keypair_t kp2 =
      iroha::create_keypair(iroha::create_seed("hooi pidr2"));

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
