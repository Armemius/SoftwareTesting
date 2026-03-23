# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Math tests
add_executable(test_sec
    test_sec.cpp
)
target_link_libraries(test_sec PRIVATE GTest::gtest_main)
target_include_directories(test_sec PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Sorting tests
add_executable(test_bubble_sorting
    test_bubble_sorting.cpp
)
target_link_libraries(test_bubble_sorting PRIVATE GTest::gtest_main)
target_include_directories(test_bubble_sorting PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Domain model tests
add_executable(test_domain_model
    test_domain_model.cpp
)
target_link_libraries(test_domain_model PRIVATE GTest::gtest_main)
target_include_directories(test_domain_model PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Setup testing integration
enable_testing()
add_test(NAME SecTest COMMAND test_sec)
add_test(NAME BubbleSortingTests COMMAND test_bubble_sorting)
add_test(NAME DomainModelTests COMMAND test_domain_model)
