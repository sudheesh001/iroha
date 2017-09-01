add_library(irohalib_builder
    transaction.hpp
    transaction.cpp
    )
target_link_libraries(irohalib_builder
    schema
    )
