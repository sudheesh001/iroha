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

#pragma once

#include <stdexcept>
#include <string>

namespace basic {

  template <typename Base_t>
  class Type {
   public:
    using BadFormatException = std::exception;

    Type() = default;

    explicit operator Base_t() const noexcept { return t_; }
    Base_t data() const noexcept { return t_; }

    virtual bool is_valid(Base_t t) const noexcept = 0;

   protected:
    Base_t t_;
  };
};
