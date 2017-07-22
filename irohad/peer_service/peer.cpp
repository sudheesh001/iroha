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

#include "peer.hpp"
#include "peer_service.hpp"

#ifndef SHORT_TIMER_LOW
// milliseconds
#define SHORT_TIMER_LOW (1000)
#endif

#ifndef SHORT_TIMER_HIGH
// milliseconds
#define SHORT_TIMER_HIGH (2000)
#endif

#ifndef LONG_TIMER_LOW
// milliseconds
#define LONG_TIMER_LOW (1000 * 60 * 1)
#endif

#ifndef LONG_TIMER_HIGH
// milliseconds
#define LONG_TIMER_HIGH (1000 * 60 * 60)
#endif

namespace peerservice {
  std::random_device Peer::random_device;
  // generator with random distribution
  std::default_random_engine Peer::generator(Peer::random_device());
  // uniform distribution for timer : from 1 sec to 2 secs
  std::uniform_int_distribution<uint32_t> Peer::next_short_timer(
      SHORT_TIMER_LOW, SHORT_TIMER_HIGH);

  // uniform distribution for timer: from 1 min to 1 hour
  std::uniform_int_distribution<uint32_t> Peer::next_long_timer(
      LONG_TIMER_LOW, LONG_TIMER_HIGH);  // ms

  Peer::Peer(const iroha::model::Peer &n, std::shared_ptr<uvw::Loop> loop) : peer(n) {
    if (loop == nullptr) throw std::invalid_argument("loop is null");

    this->online_ = false;
    this->timer = loop->resource<uvw::TimerHandle>();

    auto channel = grpc::CreateChannel(peer.address, grpc::InsecureChannelCredentials());
    stub_ = proto::PeerService::NewStub(channel);
  }

  Peer::~Peer() { this->timer->close(); }

  void Peer::start_timer(std::uniform_int_distribution<uint32_t> &distr) {
    if (timer->active()) timer->stop();

    auto timeout_ms = std::chrono::milliseconds(distr(Peer::generator));
    timer->start(timeout_ms, uvw::TimerHandle::Time{0});
  }

  void Peer::make_online() noexcept {
    this->online_ = true;
    this->start_timer(next_short_timer);
  }

  void Peer::make_offline() noexcept {
    this->online_ = false;
    this->start_timer(next_long_timer);
  }

  bool Peer::online() const {
    return online_;
  }
}