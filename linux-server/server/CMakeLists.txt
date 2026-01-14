add_executable(server
    main.cpp
    clienthandler.cpp
    ../common/protocol.cpp
    ../common/validator.cpp
)

target_include_directories(server PRIVATE ../common)

if(QT_VERSION EQUAL 6)
    target_link_libraries(server Qt6::Core Qt6::Network)
else()
    target_link_libraries(server Qt5::Core Qt5::Network)
endif()
