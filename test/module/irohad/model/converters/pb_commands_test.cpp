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

#include <gtest/gtest.h>
#include "commands.pb.h"
#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"
#include "model/commands/remove_signatory.hpp"
#include "model/commands/set_permissions.hpp"
#include "model/commands/set_quorum.hpp"
#include "model/commands/transfer_asset.hpp"

#include "model/converters/pb_command_factory.hpp"

#include <algorithm>

void command_converter_test(iroha::model::Command &abstract_command) {
  auto factory = iroha::model::converters::PbCommandFactory();
  auto pb_repr = factory.serializeAbstractCommand(abstract_command);
  auto model_repr = factory.deserializeAbstractCommand(pb_repr);
  ASSERT_EQ(abstract_command, *model_repr);
}

TEST(CommandTest, add_asset_quantity) {
  auto orig_command = iroha::model::AddAssetQuantity();
  orig_command.account_id = "23";
  iroha::Amount amount;
  amount.frac_part = 50;
  amount.int_part = 1;

  orig_command.amount = amount;
  orig_command.asset_id = "23";

  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddAssetQuantity(orig_command);
  auto serial_command = factory.deserializeAddAssetQuantity(proto_command);

  ASSERT_EQ(orig_command, serial_command);
  command_converter_test(orig_command);
}

TEST(CommandTest, add_peer) {
  auto orig_addPeer = iroha::model::AddPeer();
  orig_addPeer.address = "10.90.129.23";

  auto factory = iroha::model::converters::PbCommandFactory();

  auto proto_add_peer = factory.serializeAddPeer(orig_addPeer);
  auto serial_addPeer = factory.deserializeAddPeer(proto_add_peer);

  ASSERT_EQ(orig_addPeer, serial_addPeer);
  command_converter_test(orig_addPeer);

  orig_addPeer.address = "134";
  ASSERT_NE(serial_addPeer, orig_addPeer);
}

TEST(CommandTest, add_signatory) {
  auto orig_command = iroha::model::AddSignatory();
  orig_command.account_id = "23";

  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddSignatory(orig_command);
  auto serial_command = factory.deserializeAddSignatory(proto_command);

  ASSERT_EQ(orig_command, serial_command);
  command_converter_test(orig_command);

  orig_command.account_id = "100500";
  ASSERT_NE(orig_command, serial_command);
}

TEST(CommandTest, add_signatory_abstract_factory) {
  auto orig_command = iroha::model::AddSignatory();
  orig_command.account_id = "23";

  command_converter_test(orig_command);
}

TEST(CommandTest, create_asset) {
  auto factory = iroha::model::converters::PbCommandFactory();

  auto orig_command = iroha::model::CreateAsset();
  orig_command.domain_id = "kek_cheburek";
  orig_command.precision = 1;
  orig_command.asset_name = "test_asset";

  auto proto_command = factory.serializeCreateAsset(orig_command);
  auto serial_command = factory.deserializeCreateAsset(proto_command);

  ASSERT_EQ(orig_command, serial_command);

  command_converter_test(orig_command);
}

TEST(CommandTest, create_account) {
  auto orig_command = iroha::model::CreateAccount();
  orig_command.account_name = "keker";
  orig_command.domain_id = "cheburek";
  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeCreateAccount(orig_command);
  auto serial_command = factory.deserializeCreateAccount(proto_command);
  ASSERT_EQ(orig_command, serial_command);
  command_converter_test(orig_command);
}

TEST(CommandTest, remove_signatory) {
  auto factory = iroha::model::converters::PbCommandFactory();

  auto orig_command = iroha::model::RemoveSignatory();
  orig_command.account_id = "Vasya";
  std::fill(orig_command.pubkey.begin(), orig_command.pubkey.end(), 0xF);

  auto proto_command = factory.serializeRemoveSignatory(orig_command);
  auto serial_command = factory.deserializeRemoveSignatory(proto_command);

  ASSERT_EQ(orig_command, serial_command);

  command_converter_test(orig_command);
}

TEST(CommandTest, set_acount_permissions) {
  auto factory = iroha::model::converters::PbCommandFactory();

  auto orig_command = iroha::model::SetAccountPermissions();
  orig_command.account_id = "Vasya";
  iroha::model::Account::Permissions perm;
  perm.can_transfer = true;
  perm.add_signatory = true;
  perm.issue_assets = true;
  orig_command.new_permissions = perm;

  auto proto_command = factory.serializeSetAccountPermissions(orig_command);
  auto serial_command = factory.deserializeSetAccountPermissions(proto_command);

  ASSERT_EQ(orig_command, serial_command);

  command_converter_test(orig_command);
}

TEST(CommandTest, set_account_quorum) {
  auto factory = iroha::model::converters::PbCommandFactory();

  auto orig_command = iroha::model::SetQuorum();
  orig_command.new_quorum = 11;
  orig_command.account_id = "Vasya";

  auto proto_command = factory.serializeSetQuorum(orig_command);
  auto serial_command = factory.deserializeSetQuorum(proto_command);

  ASSERT_EQ(orig_command, serial_command);

  command_converter_test(orig_command);
}

TEST(CommandTest, set_transfer_asset) {
  auto factory = iroha::model::converters::PbCommandFactory();

  auto orig_command = iroha::model::TransferAsset();
  orig_command.amount = {1, 20};
  orig_command.asset_id = "tugrik";
  orig_command.src_account_id = "Vasya";
  orig_command.dest_account_id = "Petya";

  auto proto_command = factory.serializeTransferAsset(orig_command);
  auto serial_command = factory.deserializeTransferAsset(proto_command);

  ASSERT_EQ(orig_command, serial_command);

  command_converter_test(orig_command);
}

