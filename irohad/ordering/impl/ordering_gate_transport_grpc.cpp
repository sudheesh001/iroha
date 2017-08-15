//
// Created by Dumitru Savva on 12/08/2017.
//

#include "ordering_gate_transport_grpc.hpp"

void iroha::ordering::OrderingGateTransportgRPC::propagate(std::shared_ptr<const model::Transaction> transaction)  {
  auto call = new AsyncClientCall;

  call->response_reader = client_->AsyncSendTransaction(
          &call->context, factory_.serialize(*transaction), &cq_);

  call->response_reader->Finish(&call->reply, &call->status, call);
}

iroha::ordering::OrderingGateTransportgRPC::OrderingGateTransportgRPC(const std::string &server_address):
        client_(proto::OrderingService::NewStub(grpc::CreateChannel(
                server_address, grpc::InsecureChannelCredentials()))) {

}
