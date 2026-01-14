add_executable(tst_protocol
    tst_protocol.cpp
    ../common/protocol.cpp
    ../common/validator.cpp
)

target_include_directories(tst_protocol PRIVATE ../common)

if(QT_VERSION EQUAL 6)
    target_link_libraries(tst_protocol Qt6::Core Qt6::Test)
else()
    target_link_libraries(tst_protocol Qt5::Core Qt5::Test)
endif()

enable_testing()
add_test(NAME ProtocolTest COMMAND tst_protocol)