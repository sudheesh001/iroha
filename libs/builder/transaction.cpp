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

#include "transaction.hpp"

namespace builder {
  static iroha::protocol::Transaction tx;

  void TransactionBuilder::sign() {

  }
  TransactionBuilder &TransactionBuilder::add_asset_quantity(AddAssetQuantity arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::add_peer(AddPeer arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::add_signatory(AddSignatory arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::create_asset(CreateAsset arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::create_account(CreateAccount arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::create_domain(CreateDomain arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::remove_signatory(RemoveSignatory arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::set_account_permissions(SetAccountPermissions arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::set_account_quorum(SetAccountQuorum arg) {
    return <#initializer#>;
  }
  TransactionBuilder &TransactionBuilder::transfer_asset(TransferAsset arg) {
    return <#initializer#>;
  }
  iroha::protocol::Transaction TransactionBuilder::proto() const {
    return iroha::protocol::Transaction();
  }
}

