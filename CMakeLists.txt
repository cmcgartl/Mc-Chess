cmake_minimum_required(VERSION 3.16)
project(chess-web VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
include(FetchContent)

FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(crow)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(json)

# Chess engine library (shared between server and tests)
add_library(chess_engine
    backend/src/engine/board.cpp
    backend/src/engine/position.cpp
    backend/src/game/game.cpp
)
target_include_directories(chess_engine PUBLIC
    backend/src/engine
    backend/src/game
)

# Main executable
add_executable(chess_server backend/src/main.cpp)
target_link_libraries(chess_server PRIVATE chess_engine Crow::Crow nlohmann_json::nlohmann_json)

# Tests
enable_testing()
add_executable(tests_runner
    backend/tests/test_main.cpp
    backend/tests/test_board.cpp
)
target_link_libraries(tests_runner PRIVATE chess_engine)
target_include_directories(tests_runner PRIVATE backend/third_party)
add_test(NAME chess_tests COMMAND tests_runner)
