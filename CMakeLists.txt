cmake_minimum_required(VERSION 3.10)

project(UltimateTagBot)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_subdirectory(thirdparty/tgbot-cpp)
add_subdirectory(thirdparty/json)

add_executable(UltimateTagBot
    "./main.cpp"
    "./bot.cpp"
    "./args_iterator.cpp"
    "./tag_database.cpp"
)

target_link_libraries(UltimateTagBot
    PUBLIC TgBot
    PUBLIC nlohmann_json
)