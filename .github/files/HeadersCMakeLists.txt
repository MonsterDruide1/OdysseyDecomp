add_library(OdysseyHeaders INTERFACE)
target_compile_definitions(OdysseyHeaders INTERFACE
        NN_SDK_MAJOR=3
        NN_SDK_MINOR=5
        NN_SDK_PATCH=1
        NN_WARE_MAJOR=3
        NN_WARE_MINOR=5
        NN_WARE_PATCH=1
        NNSDK
        SEAD_VERSION=SEAD_VERSION_SMO
)
target_include_directories(OdysseyHeaders INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/aarch64
        ${CMAKE_CURRENT_LIST_DIR}/agl
        ${CMAKE_CURRENT_LIST_DIR}/al
        ${CMAKE_CURRENT_LIST_DIR}/eui
        ${CMAKE_CURRENT_LIST_DIR}/game
        ${CMAKE_CURRENT_LIST_DIR}/NintendoSDK
        ${CMAKE_CURRENT_LIST_DIR}/NintendoSDK-NEX/OnlineCore
        ${CMAKE_CURRENT_LIST_DIR}/NintendoSDK-NEX/RendezVous
        ${CMAKE_CURRENT_LIST_DIR}/NintendoSDK-NEX/RendezVous/Core
        ${CMAKE_CURRENT_LIST_DIR}/sead
)
