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

#ifndef IROHA_JSON_COMMAND_FACTORY_HPP
#define IROHA_JSON_COMMAND_FACTORY_HPP

#include "model/common.hpp"
#include <rapidjson/document.h>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "model/command.hpp"
#include "model/converters/json_common.hpp"

namespace iroha {
  namespace model {
    namespace converters {

      template <typename T>
      struct Transform<T, Amount> {
        auto operator()(T x) {
          auto des = makeFieldDeserializer(x);
          return nonstd::make_optional<Amount>() |
                 des.Uint64(&Amount::int_part, "int_part") |
                 des.Uint64(&Amount::frac_part, "frac_part");
        }
      };

      template <typename T>
      struct Transform<T, Account::Permissions> {
        auto operator()(T x) {
          auto des = makeFieldDeserializer(x);
          return nonstd::make_optional<Account::Permissions>() |
                 des.Bool(&Account::Permissions::add_signatory,
                          "add_signatory") |
                 des.Bool(&Account::Permissions::can_transfer, "can_transfer") |
                 des.Bool(&Account::Permissions::create_accounts,
                          "create_accounts") |
                 des.Bool(&Account::Permissions::create_assets,
                          "create_assets") |
                 des.Bool(&Account::Permissions::create_domains,
                          "create_domains") |
                 des.Bool(&Account::Permissions::issue_assets, "issue_assets") |
                 des.Bool(&Account::Permissions::read_all_accounts,
                          "read_all_accounts") |
                 des.Bool(&Account::Permissions::remove_signatory,
                          "remove_signatory") |
                 des.Bool(&Account::Permissions::set_permissions,
                          "set_permissions") |
                 des.Bool(&Account::Permissions::set_quorum, "set_quorum");
        }
      };

      class JsonCommandFactory {
       public:
        JsonCommandFactory();

        // AddAssetQuantity
        rapidjson::Document serializeAddAssetQuantity(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeAddAssetQuantity(
            const rapidjson::Value &document);

        // AddPeer
        rapidjson::Document serializeAddPeer(std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeAddPeer(
            const rapidjson::Value &document);

        // AddSignatory
        rapidjson::Document serializeAddSignatory(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeAddSignatory(
            const rapidjson::Value &document);

        // AssignMasterKey
        rapidjson::Document serializeAssignMasterKey(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeAssignMasterKey(
            const rapidjson::Value &document);

        // CreateAccount
        rapidjson::Document serializeCreateAccount(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeCreateAccount(
            const rapidjson::Value &document);

        // CreateAsset
        rapidjson::Document serializeCreateAsset(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeCreateAsset(
            const rapidjson::Value &document);

        // CreateDomain
        rapidjson::Document serializeCreateDomain(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeCreateDomain(
            const rapidjson::Value &document);

        // RemoveSignatory
        rapidjson::Document serializeRemoveSignatory(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeRemoveSignatory(
            const rapidjson::Value &document);

        // SetAccountPermissions
        rapidjson::Document serializeSetAccountPermissions(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeSetAccountPermissions(
            const rapidjson::Value &document);

        // SetQuorum
        rapidjson::Document serializeSetQuorum(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeSetQuorum(
            const rapidjson::Value &document);

        // TransferAsset
        rapidjson::Document serializeTransferAsset(
            std::shared_ptr<Command> command);
        optional_ptr<Command> deserializeTransferAsset(
            const rapidjson::Value &document);

        // Abstract
        rapidjson::Document serializeAbstractCommand(
            std::shared_ptr<Command> command);
        optional_ptr <model::Command> deserializeAbstractCommand(
            const rapidjson::Value &document);

       private:
        Convert<std::shared_ptr<Command>> transform;

        using Serializer = rapidjson::Document (JsonCommandFactory::*)(
            std::shared_ptr<Command>);
        using Deserializer = optional_ptr<Command> (
            JsonCommandFactory::*)(const rapidjson::Value &);

        std::unordered_map<std::type_index, Serializer> serializers_;
        std::unordered_map<std::string, Deserializer> deserializers_;
      };

    }  // namespace converters
  }    // namespace model
}  // namespace iroha

#endif  // IROHA_JSON_COMMAND_FACTORY_HPP
