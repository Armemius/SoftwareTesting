# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Math tests
add_executable(test_maths
    test_maths.cpp
)
target_link_libraries(test_maths PRIVATE GTest::gtest_main maths)

# Setup testing integration
enable_testing()
add_test(NAME MathTests COMMAND test_maths)
