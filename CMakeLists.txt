cmake_minimum_required(VERSION 3.15)
project(CREN)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(
        src
        include

        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        )

add_executable(${PROJECT_NAME} src/main.cpp include/cren.hpp include/util.hpp include/s_gameplay.hpp src/s_gameplay.cpp src/util.cpp src/cren.cpp include/s_exit.hpp src/s_exit.cpp include/game.hpp src/game.cpp include/gut.hpp src/gut.cpp include/s_temp.hpp src/s_temp.cpp include/UI.hpp src/UI.cpp include/s_UI.hpp src/s_UI.cpp src/items.cpp include/items.hpp include/global.hpp src/global.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        )