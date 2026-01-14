set(CLIENT_SOURCES
    main.cpp
    mainwindow.cpp
    ../common/protocol.cpp
    ../common/validator.cpp
    mainwindow.ui
)

add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PRIVATE ../common)

if(QT_VERSION EQUAL 6)
    target_link_libraries(client Qt6::Core Qt6::Widgets Qt6::Network)
else()
    target_link_libraries(client Qt5::Core Qt5::Widgets Qt5::Network)
endif()
