//
// Created by Dumitru Savva on 12/08/2017.
//

#pragma once

#include <ordering.grpc.pb.h>
#include <model/converters/pb_transaction_factory.hpp>
#include "network/ordering_gate_transport.hpp"
#include "network/impl/async_grpc_client.hpp"

namespace iroha {
    using namespace iroha::network;
    using namespace iroha::model;
    namespace ordering {
        class OrderingGateTransportgRPC : public OrderingGateTransport,
                                          public network::AsyncGrpcClient<google::protobuf::Empty>  {
        private:
          std::unique_ptr<proto::OrderingService::Stub> client_;
          model::converters::PbTransactionFactory factory_;

        public:

            explicit OrderingGateTransportgRPC(const std::string& server_address);

            void propagate(std::shared_ptr<const Transaction> transaction)  override;

          ~OrderingGateTransportgRPC() override = default;
        };
    }
}

