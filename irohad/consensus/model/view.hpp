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

#ifndef IROHA_VIEW_HPP
#define IROHA_VIEW_HPP

#include <consensus.pb.h>
#include <model/message.hpp>
#include "peer.hpp"
#include "signature.hpp"

namespace consensus {
  namespace model {

    class View final : public ::model::Message<const proto::View> {
     public:
      View(const proto::View *ptr);

      bool is_schema_valid() const noexcept override;

      const std::vector<Peer> peer;

      uint64_t view_id() const noexcept;

      const Signature sig;

      bool is_signature_valid() const noexcept;
    };
  }
}

#endif  // IROHA_VIEW_HPP
