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

#include <model/transaction.hpp>
#include "block.pb.h"
#include "crypto.hpp"

namespace iroha {
  namespace model {

    using PbTx = iroha::protocol::Transaction;

    void sign(PbTx &tx, pubkey_t pub, privkey_t priv) {
      std::string blob = tx.payload().SerializeAsString();

      sig_t sig = iroha::sign(blob, pub, priv);

      auto pbsig = tx.add_signature();
      pbsig->set_pubkey(pub.to_string());
      pbsig->set_signature(sig.to_string());
    }

    template <>
    bool verify<PbTx>(const PbTx &tx) {
      if (tx.signature_size() == 0)
        throw BadFormatException("transaction has no signatures");

      const std::string blob = tx.payload().SerializeAsString();

      for (auto &&pbsig : tx.signature()) {
        const pubkey_t pub = pubkey_t::from_string(pbsig.pubkey());
        const sig_t sig = sig_t::from_string(pbsig.signature());

        if (!iroha::verify(blob, pub, sig)) return false;
      }

      return true;
    }

  }
}
