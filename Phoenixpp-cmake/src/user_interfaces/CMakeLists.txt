add_subdirectory(command_line_ui)
add_subdirectory(graphical_ui)
add_library(BaseUi base_ui.cpp base_ui.h)
target_include_directories(Cli PUBLIC ${CMAKE_SOURCE_DIR}/src/factories)
target_include_directories(Cli PUBLIC ${CMAKE_SOURCE_DIR}/src/components)
target_link_libraries(BaseUi PUBLIC nlohmann_json)
target_link_libraries(BaseUi PUBLIC Factories)
target_link_libraries(BaseUi PUBLIC Components)
