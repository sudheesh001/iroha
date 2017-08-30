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

namespace iroha {
  namespace model {
    namespace converters {

      class JsonCommandFactory {
       public:
        JsonCommandFactory();

        // AddAssetQuantity
        rapidjson::Document serializeAddAssetQuantity(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeAddAssetQuantity(
            const rapidjson::Document &command);

        // AddPeer
        rapidjson::Document serializeAddPeer(std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeAddPeer(
            const rapidjson::Document &command);

        // AddSignatory
        rapidjson::Document serializeAddSignatory(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeAddSignatory(
            const rapidjson::Document &command);

        // CreateAccount
        rapidjson::Document serializeCreateAccount(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeCreateAccount(
            const rapidjson::Document &command);

        // CreateAsset
        rapidjson::Document serializeCreateAsset(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeCreateAsset(
            const rapidjson::Document &command);

        // CreateDomain
        rapidjson::Document serializeCreateDomain(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeCreateDomain(
            const rapidjson::Document &command);

        // RemoveSignatory
        rapidjson::Document serializeRemoveSignatory(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeRemoveSignatory(
            const rapidjson::Document &command);

        // SetAccountPermissions
        rapidjson::Document serializeSetAccountPermissions(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeSetAccountPermissions(
            const rapidjson::Document &command);

        // SetQuorum
        rapidjson::Document serializeSetQuorum(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeSetQuorum(
            const rapidjson::Document &command);

        // TransferAsset
        rapidjson::Document serializeTransferAsset(
            std::shared_ptr<Command> command);
        std::shared_ptr<Command> deserializeTransferAsset(
            const rapidjson::Document &command);

        // Abstract
        rapidjson::Document serializeAbstractCommand(
            std::shared_ptr<Command> command);
        optional_ptr <model::Command> deserializeAbstractCommand(
            const rapidjson::Document &command);

       private:
        using Serializer = rapidjson::Document (JsonCommandFactory::*)(
            std::shared_ptr<Command>);
        using Deserializer = std::shared_ptr<model::Command> (
            JsonCommandFactory::*)(const rapidjson::Document &);

        std::unordered_map<std::type_index, Serializer> serializers_;
        std::unordered_map<std::string, Deserializer> deserializers_;
      };

    }  // namespace converters
  }    // namespace model
}  // namespace iroha

#endif  // IROHA_JSON_COMMAND_FACTORY_HPP
