/*
Copyright 2017 Soramitsu Co., Ltd.
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

#include <grpc++/grpc++.h>
#include <block.pb.h>
#include "client.hpp"

namespace logger {
  namespace connection {

    using iroha::protocol::Ping;
    using iroha::protocol::Pong;

    bool check(const std::string& address){
      LoggerLivingConfirmationClient client(address);
      return client.check(Ping{});
    }

    LoggerLivingConfirmationClient::LoggerLivingConfirmationClient(const std::string& address) {
      auto channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
      stub_ = iroha::protocol::LoggerLivingConfirmationService::NewStub(channel);
    }

    bool LoggerLivingConfirmationClient::check(const iroha::protocol::Ping& ping) {
      Pong pong;
      auto status = stub_->check(&context_, ping, &pong);
      return status.ok();
    }

  }  // namespace connection
}  // namespace consensus
