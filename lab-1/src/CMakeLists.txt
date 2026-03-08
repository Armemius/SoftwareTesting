# Math library
file(GLOB_RECURSE MATH_HEADER_LIST CONFIGURE_DEPENDS
    ${TestingLab1_SOURCE_DIR}/include/maths/*.hpp
)
file(GLOB_RECURSE MATH_SOURCES_LIST CONFIGURE_DEPENDS
    ${TestingLab1_SOURCE_DIR}/src/maths/*.cpp
)
add_library(maths ${MATH_SOURCES_LIST} ${MATH_HEADER_LIST})
target_include_directories(maths PUBLIC ${TestingLab1_SOURCE_DIR}/include)
