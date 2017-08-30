add_subdirectory(ed25519)

add_library(hash
    hash.cpp
    )

target_link_libraries(hash
    ed25519
    )

add_library(crypto
    crypto.hpp
    ed25519_impl.cpp
    )
target_link_libraries(crypto
    ed25519
    hash
    )

add_library(lookup3
    lookup3.h
    lookup3.c
    )


#add_library(model_crypto
#    model_crypto.cpp
#    )
#target_link_libraries(model_crypto
#    crypto
#    schema
#    optional
#    )
