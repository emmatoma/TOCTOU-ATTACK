cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  file_copier
  VERSION 1.0
  LANGUAGES C)

# Define the executable and its dependencies
add_executable(${CMAKE_PROJECT_NAME} sources/main.c)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE sources/functions)
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE sources/functions/functions.c
          sources/functions/hidden_functions/hidden_functions.c)

# Install command for safe privilege management
install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE WORLD_EXECUTE)

# For using clangd with VSCode
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_SOURCE_DIR}/compile_commands.json)
add_custom_target(database ALL
                  DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)
