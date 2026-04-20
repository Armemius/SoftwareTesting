# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
set(STUB_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/top_down")

# Math tests
add_executable(test_sin
    test_sin.cpp
)
target_link_libraries(test_sin PRIVATE GTest::gtest_main)
target_include_directories(test_sin PRIVATE "${CMAKE_SOURCE_DIR}/include")
gtest_discover_tests(test_sin)

add_executable(test_ln
    test_ln.cpp
)
target_link_libraries(test_ln PRIVATE GTest::gtest_main)
target_include_directories(test_ln PRIVATE "${CMAKE_SOURCE_DIR}/include")
gtest_discover_tests(test_ln)

add_executable(test_cos
    test_cos.cpp
)
target_link_libraries(test_cos PRIVATE GTest::gtest_main)
target_include_directories(test_cos PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(test_cos PRIVATE STUB_DATA_DIR="${STUB_DATA_DIR}")
gtest_discover_tests(test_cos)

add_executable(test_tan
    test_tan.cpp
)
target_link_libraries(test_tan PRIVATE GTest::gtest_main)
target_include_directories(test_tan PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(test_tan PRIVATE STUB_DATA_DIR="${STUB_DATA_DIR}")
gtest_discover_tests(test_tan)

add_executable(test_log
    test_log.cpp
)
target_link_libraries(test_log PRIVATE GTest::gtest_main)
target_include_directories(test_log PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(test_log PRIVATE STUB_DATA_DIR="${STUB_DATA_DIR}")
gtest_discover_tests(test_log)

add_executable(test_complex
    test_complex.cpp
)
target_link_libraries(test_complex PRIVATE GTest::gtest_main)
target_include_directories(test_complex PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(test_complex PRIVATE STUB_DATA_DIR="${STUB_DATA_DIR}")
gtest_discover_tests(test_complex)
