add_library(irohalib_command
    add_asset_quantity.hpp
    add_peer.hpp
    add_signatory.hpp
    command.hpp
    commands.hpp
    create_account.hpp
    create_asset.hpp
    create_domain.hpp
    remove_signatory.hpp
    set_account_permissions.hpp
    set_account_quorum.hpp

    transfer_asset.hpp
    transfer_asset.cpp
    )

target_link_libraries(irohalib_command
    schema
)
