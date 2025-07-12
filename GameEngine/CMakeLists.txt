find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(GameEngine "GameEngine.cpp" "timer.h" "animation.h")

if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(GameEngine PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)