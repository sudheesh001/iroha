add_library(synchronization
        peer_synchronization.cpp
)

target_link_libraries(synchronization PUBLIC
    time64
    rxcpp
    model
    ametsuchi
)
