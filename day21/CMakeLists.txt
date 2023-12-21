
cmake_minimum_required(VERSION 3.5)

add_executable(day21-exe
    main.cpp
)

target_include_directories(day21-exe
    PRIVATE
        ../include/
)

set_target_properties(day21-exe
    PROPERTIES
        OUTPUT_NAME day21
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

#
# Setup warnings
#
# https://www.foonathan.net/2018/10/cmake-warnings/
#
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
target_compile_options(day21-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_compile_options(day21-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_compile_options(day21-exe PRIVATE
    #
    # Zc:preprocessor is needed for handling __VA_OPT__(,)
    #
    /Zc:preprocessor /WX /W4
)
endif()

configure_file(input.txt input.txt COPYONLY)
configure_file(input2.txt input2.txt COPYONLY)



