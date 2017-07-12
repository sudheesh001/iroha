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

#ifndef IROHA_BLOCK_LOADER_HPP
#define IROHA_BLOCK_LOADER_HPP

#include "model/model.hpp"
#include "rxcpp/rx-observable.hpp"

namespace iroha {
  namespace network {

    /**
     * Interface for downloading blocks from a network
     */
    class SyncClient {
     public:
      /**
       * Method requests missed blocks from external peer starting from it's top
       * block.
       * Note, that blocks will be in order: from the newest
       * to your actual top block.
       * This order is required for verify blocks before storing in a ledger.
       * @param target_ip - peer's ip for requesting blocks
       * @param offset - your last actual block's height
       * @return observable with blocks
       */
      rxcpp::observable<model::Block> fetchBlocks(std::string target_ip,
                                                  uint64_t offset);
    };
  }  // namespace network
}  // namespace iroha
#endif  // IROHA_BLOCK_LOADER_HPP
