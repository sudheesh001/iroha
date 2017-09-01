#include <common/uint256_t.h>
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

#include "transfer_asset.hpp"

namespace iroha {
  TransferAsset::TransferAsset(const std::string &from, const std::string &to,
                               const std::string &asset_id,
                               const uint256_t &amount) {
    t = std::make_unique<protocol::TransferAsset>();

    // validate input and create proto object
    t->set_src_account_id(from);
    t->set_dest_account_id(to);
    t->set_asset_id(asset_id);
    // TODO:  amount
  }
}
