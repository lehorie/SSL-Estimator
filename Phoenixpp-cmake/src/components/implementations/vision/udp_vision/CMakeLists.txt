add_library(UdpVision grsim_vision.cpp grsim_vision.h real_game_vision.cpp real_game_vision.h)
target_include_directories(TestVision PUBLIC ${CMAKE_SOURCE_DIR}/src/components)
target_include_directories(TestVision PUBLIC ${CMAKE_SOURCE_DIR}/src/components/messages)
target_link_libraries(UdpVision PUBLIC Components)
target_link_libraries(UdpVision PUBLIC Messages)
target_link_libraries(UdpVision PUBLIC Poco::Net)
target_link_libraries(UdpVision PUBLIC ssl_proto)
target_link_libraries(UdpVision PUBLIC protobuf_files)
