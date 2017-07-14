/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <numeric>
#include <random>
#include <unordered_set>
#include <exception>

#include <peer_service/peer_service.hpp>

namespace peer_service {

  void initialize() {
    detail::self_ = SelfStatus();
    // TODO
    // detail::peers_ = ametsuchi::getPeers()
  }

  /**
   * @return List of peers that therefore permutation.
   */
  std::vector<std::shared_ptr<Peer>> getPermutationPeers() {
    std::vector<std::shared_ptr<Peer>> v;
    try {
      for (auto id : detail::permutation_)
        v.emplace_back(detail::peers_.at(id));
      return v;
    } catch( std::range_error() ) {
      return v;
    }
  }

  /**
   * @param i index
   * @return A i-th peer that therefore permutation.
   */
  std::shared_ptr<Peer> getPermutationAt(int i) {
    try {
      return detail::peers_.at(detail::permutation_[i]);
    } catch( std::range_error() ){
      return nullptr;
    }
  }

  /**
   * @return List of peers that is used by ordering service.
   */
  std::vector<std::shared_ptr<Peer>> getOrderingPeers() {
    std::vector<std::shared_ptr<Peer>> v;
    try {
      for (auto id : detail::ordering_peers_)
        v.emplace_back(detail::peers_.at(id));
      return v;
    } catch( std::range_error() ) {
      return v;
    }
  }

  /**
   * @return List of peers that is used by ordering service and is that will
   * be send sumeragi.
   */
  std::vector<std::shared_ptr<Peer>> getActiveOrderingPeers() {
    std::vector<std::shared_ptr<Peer>> v;
    try {
      for (auto id : detail::active_ordering_peers_)
        v.emplace_back(detail::peers_.at(id));
      return v;
    } catch( std::range_error() ) {
      return v;
    }
  }

  /**
   * @return self status
   */
  const SelfStatus& self() { return detail::self_; }

  /**
   * When on_porposal sends on_commit, it is called.
   * It check signs from Block, and identify dead peers which It throw to
   * issue Peer::Remove transaction.
   * @param commited_block commited block with signs
   */
  void RemoveDeadPeers(const iroha::model::Block& commited_block) {
    // TODO
  }

  /**
   * When on_commit, it is called.
   * It change peer oreder.
   */
  void changePermutation() {

    // memo previous used ordering_peers_
    std::unordered_set<uint8_t> prev;
    for( auto id : detail::ordering_peers_ )
      prev.insert(id);
    if( prev.empty() )
      for(uint8_t i=0;i<detail::peers_.size();i++) prev.insert(i);

    // make permutation from root hash
    detail::permutation_.clear();
    for( uint8_t i=0; i < detail::peers_.size(); ++i )
      if( detail::peers_.at(i)->status == Peer::Synced )
        detail::permutation_.emplace_back(i);

    uint64_t rootHash = 0;/* ametsuchi::merkleRoot */
    std::shuffle( detail::peers_.begin(), detail::peers_.end(), std::mt19937(rootHash) );

    // make ordering permutation
    uint8_t num_of_order = getMaxFaulty()*2;
    detail::ordering_peers_.clear();
    detail::active_ordering_peers_.clear();
    for(int i=0;i<num_of_order;i++) {
      detail::ordering_peers_.emplace_back(detail::permutation_[i]);
      if (prev.count(detail::permutation_[i]))
        detail::active_ordering_peers_.emplace_back(detail::permutation_[i]);
    }
  }

  /**
   * When commit fails, it is called.
   * It throw to issue Peer::Stop(Self) transaction.
   */
  void selfStop() {
    detail::self_.stop();
    detail::issueStop( self().getIp(), self().peer() );
  }

  /**
   * When commit successes and state of self peer is UnSynced, It is called.
   * It throw to issue Peer::Activate(self) transaction.
   */
  void selfActivate() {
    detail::self_.activate();
    detail::issueActivate( self().getIp(), self().peer() );
  }



  size_t getMaxFaulty() {
    return std::count_if( detail::peers_.begin(), detail::peers_.end(), [](auto &p){ return p->status == Peer::Synced; })/3;
  }


  // TODO
  /**
   * validate command
   */
  void validate(const Command::Peer::Add& cmd) {}
  void validate(const Command::Peer::Remove& cmd) {}
  void validate(const Command::Peer::Activate& cmd) {}
  void validate(const Command::Peer::Stop& cmd) {}
  void validate(const Command::Peer::ChangeRole& cmd) {}

  /**
   * execute command
   */
  void execute(const Command::Peer::Add& cmd) {}
  void execute(const Command::Peer::Remove& cmd) {}
  void execute(const Command::Peer::Activate& cmd) {}
  void execute(const Command::Peer::Stop& cmd) {}
  void execute(const Command::Peer::ChangeRole& cmd) {}

  namespace detail {
    void issueStop(const std::string& ip, const Peer& stop_peer) {
      // TODO
    }
    void issueActivate(const std::string& ip, const Peer& activate_peer) {
      // TODO
    }
  }  // namespace detail
}  // namespace peer_servince