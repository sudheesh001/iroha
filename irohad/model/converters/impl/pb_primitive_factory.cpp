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

#include <model/converters/generic_factory.hpp>

#include <common/uint256_t.h>
#include "primitive.pb.h"

namespace iroha {
  namespace model {
    namespace converters {

      // prefix m = model, prefix p = proto
      using m_uint128 = uint128_t;
      using p_uint128 = iroha::protocol::uint128;
      using m_uint256 = uint256_t;
      using p_uint256 = iroha::protocol::uint256;

      m_uint128 Factory<m_uint128, p_uint128>::deserialize(
          const p_uint128 &arg) const {
        uint64_t upper = arg.a();
        uint64_t lower = arg.b();

        m_uint128 n128(upper, lower);
        return n128;
      }

      m_uint256 Factory<m_uint256, p_uint256>::deserialize(
          const p_uint256 &arg) const {
        m_uint128 upper = Factory<m_uint128, p_uint128>::deserialize(arg.a());
        m_uint128 lower = Factory<m_uint128, p_uint128>::deserialize(arg.b());

        m_uint256 n256(upper, lower);
        return n256;
      }
    }
  }
}
