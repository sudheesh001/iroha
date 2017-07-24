add_library(networking
    impl/peer_communication_service.cpp
    )

target_link_libraries(networking PUBLIC
    rxcpp
    model
    ametsuchi
    )
