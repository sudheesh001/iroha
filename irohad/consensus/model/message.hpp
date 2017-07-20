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

#ifndef IROHA_MESSAGE_HPP
#define IROHA_MESSAGE_HPP

#include <common/byteutils.hpp>
#include <common/types.hpp>

using ts64_t = iroha::ts64_t;
using blob_t = iroha::blob_t;
using pubkey_t = iroha::ed25519::pubkey_t;
using signature_t = iroha::ed25519::sig_t;
using hash256_t = iroha::hash256_t;

namespace consensus {
  namespace model {

    template <typename T>
    class Message {
     public:
      explicit Message(T *ptr) : proto_{ptr} {}

      /**
       * Returns true if input message is valid according to internal schema.
       * @return
       */
      virtual bool is_schema_valid() const noexcept = 0;

      virtual ~Message() {}

     protected:
      T *proto_;
    };
  }
}

#endif  // IROHA_MESSAGE_HPP
