//
// Created by Takumi Yamashita on 2017/07/11.
//

#ifndef IROHA_NETWORK_SYNCGATE_HPP
#define IROHA_NETWORK_SYNCGATE_HPP
namespace iroha {
  namespace network {

    /**
     * Interface for downloading blocks from a network
     */
    class SyncGate {
    public:
      /**
       * This method is return response Blocks;
       * @param return_ip - peer's ip to return for responseblocks
       * @param offset - your return blocks after offset-th.
       */
      void responseBlocks( std::string return_ip, uint64_t offset );
    };
  } // namespace network
} // namespace iroha
#endif //IROHA_SYNCGATE_HPP
