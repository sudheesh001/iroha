//
// Created by Dumitru Savva on 13/08/2017.
//

#pragma once

#include <memory>
#include "model/proposal.hpp"

namespace iroha {
  namespace network {
    using namespace iroha::model;
    class OrderingServiceNotification {
    public:
      virtual void handleProposal(std::shared_ptr<Proposal>) = 0;

      virtual ~OrderingServiceNotification() = default;
    };
  }
}
