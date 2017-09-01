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

namespace iroha {
  namespace builder {
    basic::Signable &Transaction::sign(iroha::keypair_t kp) {
      if (!final) {
        final = true;
        payload_hash =
            iroha::sha3_256(p.payload().SerializeAsString()).to_string();
      }

      const sig_t signature = iroha::sign(payload_hash, kp.pubkey, kp.privkey);

      auto sig = p.add_signature();
      sig->set_pubkey(kp.pubkey.to_string());
      sig->set_signature(signature.to_string());

      return *this;
    }
  }
}
