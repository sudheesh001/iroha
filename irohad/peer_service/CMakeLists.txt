#ADD_LIBRARY(izanami STATIC
#    izanami.cpp
#)

#target_link_libraries(izanami
#    hash
#    thread_pool
#    config_manager
#    peer_service
#    executor
#    connection_with_grpc
#    transaction_repository
#)

#ADD_LIBRARY(peer_service STATIC
#    monitor.cpp
#    change_state.cpp
#    self_state.cpp
#    synchronizer/synchronizer.cpp
#)
#
#target_link_libraries(peer_service
#    timer
#    time64
#    logger
#    crypto
#    schema
#)

add_subdirectory(model)

add_library(peer
    peer.cpp
    peer.hpp
    )
target_link_libraries(peer
    consensus_grpc
    uvw
    peerservice_grpc
    optional
    )

add_library(peerservice
    peer_service.cpp
    peer_service.hpp
    )
target_link_libraries(peerservice
    peer
    lookup3
    optional
    )