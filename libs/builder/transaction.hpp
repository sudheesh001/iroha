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

#include <string>
#include <vector>
#include "command/command.hpp"
#include "block.pb.h"

namespace builder {

  struct TransactionBuilder {
    TransactionBuilder& add_asset_quantity(AddAssetQuantity arg);
    TransactionBuilder& add_peer(AddPeer arg);
    TransactionBuilder& add_signatory(AddSignatory arg);
    TransactionBuilder& create_asset(CreateAsset arg);
    TransactionBuilder& create_account(CreateAccount arg);
    TransactionBuilder& create_domain(CreateDomain arg);
    TransactionBuilder& remove_signatory(RemoveSignatory arg);
    TransactionBuilder& set_account_permissions(SetAccountPermissions arg);
    TransactionBuilder& set_account_quorum(SetAccountQuorum arg);
    TransactionBuilder& transfer_asset(TransferAsset arg);

    void sign();

    iroha::protocol::Transaction proto() const;
  };
}

void f() {
  using namespace builder;

  TransactionBuilder builder;
  builder
      .add_peer({"1", "2"})
      .transfer_asset({"a", "b", "c", 567})
      .add_peer({"2", "3"})
      .sign(keypair);

  proto::Transaction tx = builder.proto();
}

#endif  // IROHA_BUILDER_TRANSACTION_HPP
