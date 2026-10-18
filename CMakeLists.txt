cmake_minimum_required(VERSION 3.16)

project(CppCharacterCustomizationRuntime
    VERSION 1.0.0
    DESCRIPTION "A small C++17 runtime for 3D character customization and scene transforms"
    LANGUAGES CXX
)

include(CTest)
include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

add_library(character_runtime
    src/BoundingSphere.cpp
    src/Character.cpp
    src/Component.cpp
    src/Entity.cpp
    src/Matrix4.cpp
    src/RuntimeLoop.cpp
    src/Scene.cpp
    src/Transform.cpp
    src/TransformProfiler.cpp
    src/Vector3.cpp
)
add_library(CharacterRuntime::character_runtime ALIAS character_runtime)

target_include_directories(character_runtime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(character_runtime PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(character_runtime PRIVATE /W4 /permissive-)
else()
    target_compile_options(character_runtime PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(character_runtime_benchmark benchmark/main.cpp)
target_link_libraries(character_runtime_benchmark PRIVATE CharacterRuntime::character_runtime)

if(BUILD_TESTING)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(character_runtime_tests
        tests/BoundingSphereTests.cpp
        tests/CharacterSceneTests.cpp
        tests/Matrix4Tests.cpp
        tests/TransformTests.cpp
        tests/Vector3Tests.cpp
    )
    target_link_libraries(character_runtime_tests
        PRIVATE
            CharacterRuntime::character_runtime
            GTest::gtest_main
    )
    target_compile_features(character_runtime_tests PRIVATE cxx_std_17)
    if(MSVC)
        target_compile_options(character_runtime_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(character_runtime_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    include(GoogleTest)
    gtest_discover_tests(character_runtime_tests)
endif()
