find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(GameEngine "GameEngine.cpp" "timer.h" "animation.h" "ext/glm/glm/glm.cppm" "ext/glm/glm/detail/glm.cpp" "ext/glm/glm/simd/common.h" "ext/glm/glm/simd/exponential.h" "ext/glm/glm/simd/geometric.h" "ext/glm/glm/simd/integer.h" "ext/glm/glm/simd/matrix.h" "ext/glm/glm/simd/neon.h" "ext/glm/glm/simd/packing.h" "ext/glm/glm/simd/platform.h" "ext/glm/glm/simd/trigonometric.h" "ext/glm/glm/simd/vector_relational.h" "gameObject.h")

if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(GameEngine PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
target_include_directories(GameEngine PRIVATE "ext/glm/")