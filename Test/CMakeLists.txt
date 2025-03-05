# Create an executable based on the main.cpp
add_executable(L_LOGGER_TEST "main.cpp")

target_link_libraries(L_LOGGER_TEST PRIVATE L_LOGGER)

set_target_properties(L_LOGGER_TEST PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")