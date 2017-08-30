add_library(libs_common
    files.cpp
    )

target_link_libraries(libs_common
    logger
    )


add_library(uint256_t
    uint128_t.cpp
    uint256_t.cpp
    )
