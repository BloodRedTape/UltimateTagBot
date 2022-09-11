cmake_minimum_required(VERSION 3.10)

project(UltimateTagBot)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_subdirectory(thirdparty/tgbot-cpp)
add_subdirectory(thirdparty/json)

add_executable(UltimateTagBot
    "./sources/main.cpp"
    "./sources/bot.cpp"
    "./sources/utils.cpp"
    "./sources/args_iterator.cpp"
    "./sources/tag_database.cpp"
    "./sources/keytag.cpp"
)

target_link_libraries(UltimateTagBot
    TgBot
    nlohmann_json
    boost_locale
)

target_include_directories(UltimateTagBot PRIVATE "./sources")