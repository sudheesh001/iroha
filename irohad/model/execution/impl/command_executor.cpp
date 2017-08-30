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

#include <algorithm>
#include <limits>

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
#include "model/execution/command_executor.hpp"

using namespace iroha::model;
using namespace iroha::ametsuchi;

bool CommandExecutor::validate(const Command &command, WsvQuery &queries,
                               const Account &creator) {
  return hasPermissions(command, queries, creator) && isValid(command, queries);
}

// ----------------- AddAssetQuantity -----------------

bool AddAssetQuantityExecutor::execute(const Command &command,
                                       WsvQuery &queries,
                                       WsvCommand &commands) {
  auto add_asset_quantity = static_cast<const AddAssetQuantity &>(command);

  auto asset = queries.getAsset(add_asset_quantity.asset_id);
  if (not asset.has_value()) {
    // No such asset
    return false;
  }
  auto precision = asset.value().precision;
  // Amount is wrongly formed
  if (add_asset_quantity.amount.get_frac_number() > precision) {
    return false;
  }
  if (not queries.getAccount(add_asset_quantity.account_id).has_value()) {
    // No such account
    return false;
  }
  auto account_asset = queries.getAccountAsset(add_asset_quantity.account_id,
                                               add_asset_quantity.asset_id);
  AccountAsset accountAsset;
  // Such accountAsset not found
  if (not account_asset.has_value()) {
    // No wallet found -> create new
    accountAsset = AccountAsset();
    accountAsset.asset_id = add_asset_quantity.asset_id;
    accountAsset.account_id = add_asset_quantity.account_id;
    accountAsset.balance =
        add_asset_quantity.amount.get_joint_amount(precision);
  } else {
    accountAsset = account_asset.value();
    // TODO: handle non trivial arithmetic
    auto new_balance = account_asset.value().balance +
                       add_asset_quantity.amount.get_joint_amount(precision);
    // TODO: handle overflow
    accountAsset.balance = new_balance;
  }

  // accountAsset.value().balance += amount;
  return commands.upsertAccountAsset(accountAsset);
}

bool AddAssetQuantityExecutor::hasPermissions(const Command &command,
                                              WsvQuery &queries,
                                              const Account &creator) {
  // Check if creator has MoneyCreator permission
  return creator.permissions.issue_assets;
}

bool AddAssetQuantityExecutor::isValid(const Command &command,
                                       WsvQuery &queries) {
  auto add_asset_quantity = static_cast<const AddAssetQuantity &>(command);

  // TODO move to stateless validation

  // Amount must be in some meaningful range
  return (add_asset_quantity.amount.int_part > 0 ||
          add_asset_quantity.amount.frac_part > 0) &&
         add_asset_quantity.amount.int_part <
             (std::numeric_limits<uint32_t>::max)();
}

// ----------------- AddPeer -----------------

bool AddPeerExecutor::execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands) {
  auto add_peer = static_cast<const AddPeer &>(command);

  Peer peer;
  peer.address = add_peer.address;
  peer.pubkey = add_peer.peer_key;
  // Will return false if peer is not unique
  return commands.insertPeer(peer);
}

bool AddPeerExecutor::hasPermissions(const Command &command,
                                     ametsuchi::WsvQuery &queries,
                                     const Account &creator) {
  return true;
}

bool AddPeerExecutor::isValid(const Command &command,
                              ametsuchi::WsvQuery &queries) {
  // TODO: check that address is formed right
  return true;
}

// ----------------- AddSignatory -----------------

bool AddSignatoryExecutor::execute(const Command &command,
                                   ametsuchi::WsvQuery &queries,
                                   ametsuchi::WsvCommand &commands) {
  auto add_signatory = static_cast<const AddSignatory &>(command);

  return commands.insertAccountSignatory(add_signatory.account_id,
                                         add_signatory.pubkey);
}

bool AddSignatoryExecutor::hasPermissions(const Command &command,
                                          ametsuchi::WsvQuery &queries,
                                          const Account &creator) {
  auto add_signatory = static_cast<const AddSignatory &>(command);

  return
      // Case 1. When command creator wants to add signatory to their
      // account
      add_signatory.account_id == creator.account_id ||
      // Case 2. System admin wants to add signatory to account
      creator.permissions.add_signatory;
}

bool AddSignatoryExecutor::isValid(const Command &command,
                                   ametsuchi::WsvQuery &queries) {
  auto add_signatory = static_cast<const AddSignatory &>(command);

  return true;
}

// ----------------- CreateAccount -----------------

bool CreateAccountExecutor::execute(const Command &command,
                                    ametsuchi::WsvQuery &queries,
                                    ametsuchi::WsvCommand &commands) {
  auto create_account = static_cast<const CreateAccount &>(command);

  Account account;
  account.master_key = create_account.pubkey;
  account.account_id =
      create_account.account_name + "@" + create_account.domain_id;

  account.domain_name = create_account.domain_id;
  account.quorum = 1;
  Account::Permissions permissions = iroha::model::Account::Permissions();
  account.permissions = permissions;

  return commands.insertSignatory(create_account.pubkey) &&
         commands.insertAccount(account) &&
         commands.insertAccountSignatory(account.account_id,
                                         create_account.pubkey);
}

bool CreateAccountExecutor::hasPermissions(const Command &command,
                                           ametsuchi::WsvQuery &queries,
                                           const Account &creator) {
  // Creator must have permission to create account
  return creator.permissions.create_accounts;
}

bool CreateAccountExecutor::isValid(const Command &command,
                                    ametsuchi::WsvQuery &queries) {
  auto create_account = static_cast<const CreateAccount &>(command);

  return
      // Name is within some range
      not create_account.account_name.empty() &&
      create_account.account_name.size() < 8 &&
      // Account must be well-formed (no system symbols)
      std::all_of(std::begin(create_account.account_name),
                  std::end(create_account.account_name),
                  [](char c) { return std::isalnum(c); });
}

// ----------------- CreateAsset -----------------

bool CreateAssetExecutor::execute(const Command &command,
                                  ametsuchi::WsvQuery &queries,
                                  ametsuchi::WsvCommand &commands) {
  auto create_asset = static_cast<const CreateAsset &>(command);

  Asset new_asset;
  new_asset.asset_id = create_asset.asset_name + "#" + create_asset.domain_id;
  new_asset.domain_id = create_asset.domain_id;
  new_asset.precision = create_asset.precision;
  // The insert will fail if asset already exist
  return commands.insertAsset(new_asset);
}

bool CreateAssetExecutor::hasPermissions(const Command &command,
                                         ametsuchi::WsvQuery &queries,
                                         const Account &creator) {
  // Creator must have permission to create assets
  return creator.permissions.create_assets;
}

bool CreateAssetExecutor::isValid(const Command &command,
                                  ametsuchi::WsvQuery &queries) {
  auto create_asset = static_cast<const CreateAsset &>(command);

  return
      // Name is within some range
      not create_asset.asset_name.empty() &&
      create_asset.asset_name.size() < 10 &&
      // Account must be well-formed (no system symbols)
      std::all_of(std::begin(create_asset.asset_name),
                  std::end(create_asset.asset_name),
                  [](char c) { return std::isalnum(c); });
}

// ----------------- CreateDomain -----------------

bool CreateDomainExecutor::execute(const Command &command,
                                   ametsuchi::WsvQuery &queries,
                                   ametsuchi::WsvCommand &commands) {
  auto create_domain = static_cast<const CreateDomain &>(command);

  Domain new_domain;
  new_domain.domain_id = create_domain.domain_name;
  // The insert will fail if domain already exist
  return commands.insertDomain(new_domain);
}

bool CreateDomainExecutor::hasPermissions(const Command &command,
                                          ametsuchi::WsvQuery &queries,
                                          const Account &creator) {
  // Creator must have permission to create domains
  return creator.permissions.create_domains;
}

bool CreateDomainExecutor::isValid(const Command &command,
                                   ametsuchi::WsvQuery &queries) {
  auto create_domain = static_cast<const CreateDomain &>(command);

  return
      // Name is within some range
      not create_domain.domain_name.empty() &&
      create_domain.domain_name.size() < 10 &&
      // Account must be well-formed (no system symbols)
      std::all_of(std::begin(create_domain.domain_name),
                  std::end(create_domain.domain_name),
                  [](char c) { return std::isalnum(c); });
}

// ----------------- RemoveSignatory -----------------

bool RemoveSignatoryExecutor::execute(const Command &command,
                                      ametsuchi::WsvQuery &queries,
                                      ametsuchi::WsvCommand &commands) {
  auto remove_signatory = static_cast<const RemoveSignatory &>(command);

  // Delete will fail if account signatory doesn't exist
  return commands.deleteAccountSignatory(remove_signatory.account_id,
                                         remove_signatory.pubkey);
}

bool RemoveSignatoryExecutor::hasPermissions(const Command &command,
                                             ametsuchi::WsvQuery &queries,
                                             const Account &creator) {
  auto remove_signatory = static_cast<const RemoveSignatory &>(command);

  // Two cases possible.
  // 1. Creator removes signatory from their account
  // 2. System admin
  return creator.account_id == remove_signatory.account_id or
         creator.permissions.remove_signatory;
}

bool RemoveSignatoryExecutor::isValid(const Command &command,
                                      ametsuchi::WsvQuery &queries) {
  auto remove_signatory = static_cast<const RemoveSignatory &>(command);

  auto account = queries.getAccount(remove_signatory.account_id);
  return account.has_value() &&
         // You can't remove master key (first you should reassign it)
         remove_signatory.pubkey != account.value().master_key;
}

// ----------------- SetAccountPermissions -----------------

bool SetAccountPermissionsExecutor::execute(const Command &command,
                                            ametsuchi::WsvQuery &queries,
                                            ametsuchi::WsvCommand &commands) {
  auto set_account_permissions =
      static_cast<const SetAccountPermissions &>(command);

  auto account = queries.getAccount(set_account_permissions.account_id);
  if (not account.has_value()) {
    // There is no such account
    return false;
  }
  account.value().permissions = set_account_permissions.new_permissions;
  return commands.updateAccount(account.value());
}

bool SetAccountPermissionsExecutor::hasPermissions(const Command &command,
                                                   ametsuchi::WsvQuery &queries,
                                                   const Account &creator) {
  // check if creator has permission to set permissions
  return creator.permissions.set_permissions;
}

bool SetAccountPermissionsExecutor::isValid(const Command &command,
                                            ametsuchi::WsvQuery &queries) {
  return true;
}

// ----------------- SetQuorum -----------------

bool SetQuorumExecutor::execute(const Command &command,
                                ametsuchi::WsvQuery &queries,
                                ametsuchi::WsvCommand &commands) {
  auto set_quorum = static_cast<const SetQuorum &>(command);

  auto account = queries.getAccount(set_quorum.account_id);
  if (not account.has_value()) {
    // There is no such account
    return false;
  }
  account.value().quorum = set_quorum.new_quorum;
  return commands.updateAccount(account.value());
}

bool SetQuorumExecutor::hasPermissions(const Command &command,
                                       ametsuchi::WsvQuery &queries,
                                       const Account &creator) {
  auto set_quorum = static_cast<const SetQuorum &>(command);

  return
      // Case 1: creator sets quorum to their account
      // Case 2: system admin
      (creator.account_id == set_quorum.account_id ||
       creator.permissions.set_quorum);
}

bool SetQuorumExecutor::isValid(const Command &command,
                                ametsuchi::WsvQuery &queries) {
  auto set_quorum = static_cast<const SetQuorum &>(command);

  // Quorum must be from 1 to N
  return set_quorum.new_quorum > 0 && set_quorum.new_quorum < 10;
}

// ----------------- TransferAsset -----------------

bool TransferAssetExecutor::execute(const Command &command,
                                    ametsuchi::WsvQuery &queries,
                                    ametsuchi::WsvCommand &commands) {
  auto transfer_asset = static_cast<const TransferAsset &>(command);

  auto src_account_asset = queries.getAccountAsset(
      transfer_asset.src_account_id, transfer_asset.asset_id);
  if (not src_account_asset.has_value()) {
    // There is no src AccountAsset
    return false;
  }

  AccountAsset dest_AccountAsset;
  auto dest_account_asset = queries.getAccountAsset(
      transfer_asset.dest_account_id, transfer_asset.asset_id);
  auto asset = queries.getAsset(transfer_asset.asset_id);
  if (not asset.has_value()) {
    // No asset found
    return false;
  }
  // Precision for both wallets
  auto precision = asset.value().precision;
  if (transfer_asset.amount.get_frac_number() > precision) {
    // Precision is wrong
    return false;
  }
  // Get src balance
  auto src_balance = src_account_asset.value().balance;
  // TODO: handle non-trivial arithmetic
  src_balance -= transfer_asset.amount.get_joint_amount(precision);
  // Set new balance for source account
  src_account_asset.value().balance = src_balance;

  if (not dest_account_asset.has_value()) {
    // This assert is new for this account - create new AccountAsset
    dest_AccountAsset = AccountAsset();
    dest_AccountAsset.asset_id = transfer_asset.asset_id;
    dest_AccountAsset.account_id = transfer_asset.dest_account_id;
    // Set new balance for dest account
    dest_AccountAsset.balance =
        transfer_asset.amount.get_joint_amount(precision);

  } else {
    // Account already has such asset
    dest_AccountAsset = dest_account_asset.value();
    // Get balance dest account
    auto dest_balance = dest_account_asset.value().balance;

    dest_balance += transfer_asset.amount.get_joint_amount(precision);
    // Set new balance for dest
    dest_AccountAsset.balance = dest_balance;
  }

  return commands.upsertAccountAsset(dest_AccountAsset) &&
         commands.upsertAccountAsset(src_account_asset.value());
}

bool TransferAssetExecutor::hasPermissions(const Command &command,
                                           ametsuchi::WsvQuery &queries,
                                           const Account &creator) {
  auto transfer_asset = static_cast<const TransferAsset &>(command);

  // Can account transfer assets
  // Creator can transfer only from their account
  return creator.permissions.can_transfer and
         creator.account_id == transfer_asset.src_account_id;
}

bool TransferAssetExecutor::isValid(const Command &command,
                                    ametsuchi::WsvQuery &queries) {
  auto transfer_asset = static_cast<const TransferAsset &>(command);

  // Amount must be not zero
  if (not(transfer_asset.amount.frac_part > 0 or
          transfer_asset.amount.int_part > 0)) {
    return false;
  }

  auto asset = queries.getAsset(transfer_asset.asset_id);
  if (not asset.has_value()) {
    return false;
  }
  // Amount is formed wrong
  if (transfer_asset.amount.get_frac_number() > asset.value().precision) {
    return false;
  }
  auto account_asset = queries.getAccountAsset(transfer_asset.src_account_id,
                                               transfer_asset.asset_id);

  return account_asset.has_value() and
         // Check if dest account exist
         queries.getAccount(transfer_asset.dest_account_id) and
         // Balance in your wallet should be at least amount of transfer
         account_asset.value().balance >=
             transfer_asset.amount.get_joint_amount(asset.value().precision);
}
