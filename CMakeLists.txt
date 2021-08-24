cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME BoardGames)

project(${PROJECT_NAME})

file(GLOB_RECURSE SRC main.cpp source/*.cpp include/*.cpp)

set(SFMLLinkers -lsfml-network -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system)

add_executable(${PROJECT_NAME} ${SRC})

include_directories(BoardGames include/chess)

target_link_libraries(${PROJECT_NAME} ${SFMLLinkers})

