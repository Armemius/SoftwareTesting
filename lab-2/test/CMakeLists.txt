# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Math tests
add_executable(test_sin
    test_sin.cpp
)
target_link_libraries(test_sin PRIVATE GTest::gtest_main)
target_include_directories(test_sin PRIVATE "${CMAKE_SOURCE_DIR}/include")
gtest_discover_tests(test_sin)
