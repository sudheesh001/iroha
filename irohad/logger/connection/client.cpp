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

#include <logger.pb.h>
#include <logger.grpc.pb.h>

#include <grpc++/grpc++.h>
#include <grpc/grpc.h>

#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

namespace logger {
  namespace connection {


      bool post(
        const std::string& address,
        const std::string& message,
        unsigned int timestamp) {
        LoggerServiceClient client(address);
        iroha::protocol::LoggerContents contents;
        contents.set_message(message);
        contents.set_timestamp(timestamp);
        return client.post(contents);
      }

      LoggerServiceClient::LoggerServiceClient(const std::string &address) {
        auto channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
        stub_ = iroha::protocol:: LoggerService::NewStub(channel);
      }

      bool LoggerServiceClient::post(const iroha::protocol::LoggerContents &contents) {
        iroha::protocol::LoggerResponse res;
        auto status = stub_->post(&context_, contents, &res);
        return status.ok();
      }

  }  // namespace connection
}  // namespace consensus
