//
// Created by Takumi Yamashita on 2017/07/13.
//

#ifndef IROHA_COMPARATOR_HPP
#define IROHA_COMPARATOR_HPP

#include <block.pb.h>

namespace comparator {
  using Block = iroha::protocol::Block;
  class BlockHeightComparator : public std::binary_function<Block, Block, bool> {
   public:
    bool operator()(const Block &a, const Block &b) {
      return a.header().height() < b.header().height();
    }
  };
}


#endif  // IROHA_COMPARATOR_HPP
