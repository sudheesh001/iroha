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

#include "peer_service.hpp"

namespace peerservice {

  PeerServiceImpl::PeerServiceImpl(iroha::ametsuchi::WsvQuery &wsvQuery,
                                   const pubkey_t self,
                                   std::shared_ptr<uvw::Loop> loop)
      : wsvQuery_(wsvQuery), loop_{loop} {
    using namespace iroha;

    // TODO
    auto &&peers = wsvQuery_.getPeers();
    if (!peers) {
      for (auto &&peer : (*peers)) {
        this->chain.push_back(std::make_shared<peerservice::Peer>(peer, loop_));
      }
    } else {
      throw std::invalid_argument("there are no peers in database or ???");
    }
  }

  grpc::Status PeerServiceImpl::RequestHeartbeat(
      grpc::ServerContext *context, const proto::Heartbeat *request,
      proto::Heartbeat *response) {
    model::Heartbeat hb(request);
    if (!hb.is_schema_valid()) return grpc::Status::CANCELLED;
  }

  std::shared_ptr<uvw::Loop> PeerServiceImpl::getLoop() noexcept {
    return loop_;
  }

  std::shared_ptr<Peer> PeerServiceImpl::operator[](size_t index) {
    return chain[index];
  }

  const std::shared_ptr<Peer> &PeerServiceImpl::operator[](size_t index) const {
    return chain[index];
  }

  std::shared_ptr<Peer> PeerServiceImpl::leader() { return chain[0]; }

  std::shared_ptr<Peer> PeerServiceImpl::proxy_tail() {
    return chain[2 * this->max_faulty() + 1];
  }

  size_t PeerServiceImpl::max_faulty() const noexcept {
    return (chain.size() - 1) / 3;
  }

  std::shared_ptr<consensus::proto::View> PeerServiceImpl::toProto() noexcept {
    if (cachedView && view_id_ == cachedView->view_id()) return cachedView;

    cachedView->Clear();

    auto view = std::make_shared<consensus::proto::View>();
    view->set_view_id(view_id_);

    for (auto &&p : chain) {
      auto peer = view->add_peer();
      peer->set_pubkey(p->peer.pubkey.to_string());
      peer->set_address(p->peer.address);
    }

    cachedView = view;

    return cachedView;
  }

  PeerServiceImpl::operator std::shared_ptr<consensus::proto::View>() noexcept {
    return toProto();
  }
}
