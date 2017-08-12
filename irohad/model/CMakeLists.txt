add_subdirectory(converters)

add_library(model
    model_crypto_provider_impl.cpp
    model_hash_provider_impl.cpp
    impl/stateful_command_validation.cpp
    impl/command_execution.cpp
    impl/model_operators.cpp
    impl/query_execution.cpp
    )

target_link_libraries(model
    schema
    optional
    rxcpp
    hash
    crypto
    rapidjson
    logger
    )
