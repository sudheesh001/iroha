add_library(synchronization
    synchronization.cpp
)

target_link_libraries(synchronization PUBLIC
    time64
    rxcpp
    model
    ametsuchi
)
