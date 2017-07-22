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

#ifndef IROHA_SERVICE_HPP
#define IROHA_SERVICE_HPP

#include <grpc++/create_channel.h>
#include <peer_service.grpc.pb.h>
#include <ametsuchi/wsv_query.hpp>
#include <common/byteutils.hpp>
#include <common/types.hpp>
#include <consensus/model/view.hpp>
#include <memory>
#include <random>
#include <unordered_map>
#include <uvw.hpp>
#include "model/heartbeat.hpp"
#include "peer.hpp"

namespace peerservice {

  /**
   * The task of peer service is to send heartbeat messages with given interval
   * of time to every node in the ledger.
   *
   * In future, every node may have certain topology -- "neighbours" -- a subset
   * of full network.
   */

  class PeerServiceImpl : public uvw::Emitter<PeerServiceImpl>,
                          public proto::PeerService::Service {
   public:
    /**
     * Service constructor, which MUST be registered to grpc server builder.
     * @param wsvQuery instance of WsvQuery interface
     * @param self this node's public key
     * @param my latest known ledger state (my state)
     * @param loop uvw::Loop instance
     */
    PeerServiceImpl(iroha::ametsuchi::WsvQuery& wsvQuery, const pubkey_t self,
                    std::shared_ptr<uvw::Loop> loop = uvw::Loop::getDefault());

    /**
     * Returns latest state among all available peers
     * @return
     */
    std::shared_ptr<uvw::Loop> getLoop() noexcept;

    size_t max_faulty() const noexcept;
    std::shared_ptr<Peer> leader() noexcept;
    std::shared_ptr<Peer> proxy_tail() noexcept;
    std::shared_ptr<consensus::proto::View> toProto() noexcept;


   public:
    /** GRPC SERVICE IMPLEMENTATION **/
    virtual grpc::Status RequestHeartbeat(grpc::ServerContext* context,
                                          const proto::Heartbeat* request,
                                          proto::Heartbeat* response) override;

   public:
    /** operators **/
    std::shared_ptr<Peer> operator[](size_t index);
    const std::shared_ptr<Peer>& operator[](size_t index) const;
    operator std::shared_ptr<consensus::proto::View>() noexcept;

   private:
    /** members **/
    std::shared_ptr<uvw::Loop> loop_;
    model::Peer self_node_;
    std::vector<std::shared_ptr<Peer>> chain;
    uint64_t view_id_;
    std::shared_ptr<consensus::proto::View> cachedView;

   private:
    /** ametsuchi **/
    iroha::ametsuchi::WsvQuery wsvQuery_;
  };
}

#endif  // IROHA_SERVICE_HPP
