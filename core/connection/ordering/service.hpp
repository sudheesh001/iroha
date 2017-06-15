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


#ifndef __IROHA_CONNECTION_ORDERING_SERVICE_HPP__
#define __IROHA_CONNECTION_ORDERING_SERVICE_HPP__

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>

namespace connection {
    namespace ordering {

        void receive(const std::function<void(const iroha::protocol::Transaction&)>&);

        class OrderingService final
            : public iroha::protocol::OrderingService::Service {
        public:
            grpc::Status QueueTransaction(
                grpc::ClientContext* context,
                const iroha::protocol::Transaction& request,
                iroha::protocol::QueueTransactionResponse* response);
        };

    }  // namespace consensus
}  // namespace connection

#endif //__IROHA_CONNECTION_ORDERING_SERVICE_HPP__