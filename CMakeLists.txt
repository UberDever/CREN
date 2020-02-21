cmake_minimum_required(VERSION 3.15)
project(CREN)

set(CMAKE_CXX_STANDARD 14)
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

add_executable(${PROJECT_NAME} src/main.cpp include/cren.hpp include/math.hpp include/util.hpp include/s_gameplay.hpp src/s_gameplay.cpp src/math.cpp src/util.cpp src/cren.cpp include/s_mainmenu.hpp src/s_mainmenu.cpp include/s_exit.hpp src/s_exit.cpp include/gut.hpp src/gut.cpp include/res.hpp src/res.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        )