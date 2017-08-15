#pragma once

#include "model/peer.hpp"
#include "model/transaction.hpp"
#include "model/proposal.hpp"


namespace iroha {
    namespace network {
        using namespace iroha::model;

        class OrderingGateTransport {
        public:
            virtual void propagate(std::shared_ptr<const Transaction>)  = 0;

            virtual ~OrderingGateTransport() = default;
        };
    }
}