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

#include <peer_service/monitor.hpp>
#include <peer_service/self_state.hpp>


#include <algorithm>

namespace peer_service{
  namespace monitor{

    std::shared_ptr<Node> getCurrentLeader(){
      return getActivePeerAt(0);
    }
    std::string getCurrentLeaderIp(){
      return getActivePeerAt(0)->ip_;
    }


    void initialize(){
      if( !peer_list_.empty() ) return;
      // TODO Read config.json

      // At First myself only
      peer_list_.emplace_back(
          std::make_shared<Node>(
              self_state::getIp(),
              self_state::getPublicKey(),
              self_state::getName(),
              self_state::getTrust(),
              self_state::getState()
          )
      );
    }

    size_t getMaxFaulty(){
      return std::max(0, (getActivePeerSize() - 1) / 3);
    }

    Nodes getAllPeerList(){
      return peer_list_;
    }
    std::shared_ptr<Node> getPeerAt(unsigned int index){
      try {
        return peer_list_.at(index);
      } catch( const std::out_of_range& oor ){
        // TODO Out ot Range Exception
      }
    }
    std::vector<std::string> getAllIpList(){
      std::vector<std::string> ret;
      for( auto v : peer_list_ )
        ret.emplace_back( v->ip_ );
      return ret;
    }

    Nodes getActivePeerList(){
      return active_peer_list_;
    }
    std::shared_ptr<Node> getActivePeerAt(unsigned int index){
      try {
        return active_peer_list_.at(index);
      } catch( const std::out_of_range& oor ){
        // TODO Out ot Range Exception
      }
    }
    std::vector<std::string> getActiveIpList(){
      std::vector<std::string> ret;
      for( auto v : active_peer_list_ )
        ret.emplace_back( v->ip_ );
      return ret;
    }
    int getActivePeerSize(){
      return active_peer_list_.size();
    }



    bool isExistIP(const std::string &ip){
      return findPeerIP(ip) != peer_list_.end();
    }
    bool isExistPublicKey(const std::string &publicKey){
      return findPeerPublicKey(publicKey) != peer_list_.end();
    }

    Nodes::iterator findPeerIP(const std::string &ip){
      initialize();
      return std::find_if(peer_list_.begin(), peer_list_.end(),
                          [&ip](const auto &p) { return p->ip_ == ip; });
    }
    Nodes::iterator findPeerPublicKey(const std::string &publicKey){
      initialize();
      return std::find_if(
          peer_list_.begin(), peer_list_.end(),
          [&publicKey](const auto &p) { return p->public_key_ == publicKey; });
    }

  };  // namespace monitor
};