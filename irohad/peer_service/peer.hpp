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

#ifndef IROHA_NETWORK_NODE_HPP
#define IROHA_NETWORK_NODE_HPP

#include <consensus.grpc.pb.h>
#include <grpc++/grpc++.h>
#include <peer_service.grpc.pb.h>
#include <common/types.hpp>
#include <memory>
#include <model/peer.hpp>
#include <random>
#include <uvw.hpp>
#include "model/heartbeat.hpp"

namespace peerservice {

  using pubkey_t = iroha::ed25519::pubkey_t;

  class Peer : public uvw::Emitter<Peer> {
   public:
    explicit Peer(const iroha::model::Peer& n, std::shared_ptr<uvw::Loop> loop);
    Peer(const Peer&) = delete;
    Peer(const Peer&&) = delete;
    virtual ~Peer();

    const iroha::model::Peer& peer;
    std::shared_ptr<uvw::TimerHandle> timer;

    void make_online() noexcept;
    void make_offline() noexcept;

    bool online() const;

   private:
    bool online_;
    std::shared_ptr<model::Heartbeat> cachedHeartbeat;
    std::unique_ptr<proto::PeerService::Stub> stub_;

   private:
    /**
     * Stops timer if it is running, then starts new non-repetitive timer with
     * given delay. Whenever we receive any message from peer X, we reset its
     * timer with short timer. If peer is dead, then we restart long timer.
     * @param distr
     */
    void start_timer(std::uniform_int_distribution<uint32_t>& distr);

   private:
    static std::random_device random_device;
    static std::default_random_engine generator;

   public:
    static std::uniform_int_distribution<uint32_t> next_short_timer;
    static std::uniform_int_distribution<uint32_t> next_long_timer;
  };
}

#endif  // IROHA_NETWORK_NODE_HPP
