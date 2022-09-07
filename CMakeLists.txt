cmake_minimum_required(VERSION 3.10)

project(UltimateTagBot)

add_subdirectory(thirdparty/tgbot-cpp)

add_executable(UltimateTagBot
    "./main.cpp"
)

target_link_libraries(UltimateTagBot
    TgBot
)