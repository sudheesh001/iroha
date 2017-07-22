add_library(peerservice_model
        heartbeat.cpp
        heartbeat.hpp
        )
target_link_libraries(peerservice_model
        peerservice_grpc
)