add_library(irohalib_view
    transaction.hpp
    transaction.cpp
    )
target_link_libraries(irohalib_view
    schema
)
