cmake_minimum_required(VERSION 3.28)
project(std VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type options
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Sanitizer options
option(ENABLE_ASAN "Enable Address Sanitizer" ON)
option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" ON)
option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)
option(ENABLE_MSAN "Enable Memory Sanitizer" OFF)
option(ENABLE_LSAN "Enable Leak Sanitizer" OFF)
option(ENABLE_GEN_DOCS_ON_BUILD "Generate doxygen documentation on build" OFF)

# Create an interface library (header-only)
add_library(${PROJECT_NAME} INTERFACE)

# Define compiler-specific warning flags
if(MSVC)
    target_compile_options(
        ${PROJECT_NAME} INTERFACE
        /W4             # Warning level 4
        /WX             # Treat warnings as errors
        /permissive-    # Strict standards compliance
        /w14242        # Conversion possible loss of data
        /w14254        # Larger bit field type conversion
        /w14263        # Member function does not override
        /w14265        # Class has virtual functions, but destructor is not virtual
        /w14287        # Unsigned/negative constant mismatch
        /we4289        # Loop control variable declared in the for-loop is used outside
        /w14296        # Expression is always false/true
        /w14311        # Pointer truncation
        /w14545        # Expression before comma evaluates to a function
        /w14546        # Function call before comma missing argument list
        /w14547        # Operator before comma has no effect
        /w14549        # Operator before comma has no effect
        /w14555        # Expression has no effect
        /w14619        # #pragma warning: there is no warning number
        /w14640        # Enable warning on thread un-safe static member initialization
        /w14826        # Conversion from 'type1' to 'type2' is sign-extended
        /w14905        # Wide string literal cast to 'LPSTR'
        /w14906        # String literal cast to 'LPWSTR'
        /w14928        # Illegal copy-initialization
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(
        ${PROJECT_NAME} INTERFACE
            /RTC1        # Runtime error checks
            /Od         # Disable optimization
            /Zi         # Generate debug info
            /fsanitize=address # MSVC ASan support
            /JMC        # Just My Code debugging
            /sdl        # Additional security checks
        )
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(
        ${PROJECT_NAME} INTERFACE
            /O2         # Maximum optimization
            /Ob3        # Aggressive inline expansion
            /GL         # Whole program optimization
            /Gw         # Optimize global data
            /Gy         # Function-level linking
            /Qpar       # Auto-parallelizer
            /DNDEBUG    # Define NDEBUG
        )
        add_link_options(
            /LTCG       # Link-time code generation
            /OPT:REF    # Eliminate unused functions
            /OPT:ICF    # Identical COMDAT folding
        )
    endif()
else()
        target_compile_options(
        ${PROJECT_NAME} INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wduplicated-branches
        -Wduplicated-cond
        -Wextra-semi
        -Wfloat-equal
        -Wlogical-op
        -Wmissing-include-dirs
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wshadow
        -Wsign-conversion
        -Wsign-promo
        -Wundef
        -Wunused
        -Wuseless-cast
        -Wzero-as-null-pointer-constant
        -Wformat=2
        -Wdouble-promotion
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(
        ${PROJECT_NAME} INTERFACE
            -O0                         # No optimization
            -g3                         # Maximum debug info
            -fno-omit-frame-pointer     # Keep frame pointer
            -fno-inline                 # Disable inlining
            -fno-eliminate-unused-debug-types
            -fstack-protector-strong    # Stack protection
            -fstack-clash-protection    # Stack clash protection
            -fexceptions               # Enable exceptions
            -D_GLIBCXX_DEBUG           # STL debug mode
            -D_GLIBCXX_DEBUG_PEDANTIC  # Extra STL debug checks
        )

        # Sanitizer configuration
        if(ENABLE_ASAN)
                target_compile_options(
        ${PROJECT_NAME} INTERFACE -fsanitize=address -fno-optimize-sibling-calls)
            add_link_options(-fsanitize=address)
        endif()
        if(ENABLE_UBSAN)
                target_compile_options(
        ${PROJECT_NAME} INTERFACE
                -fsanitize=undefined
                -fsanitize=float-divide-by-zero
                -fsanitize=float-cast-overflow
                -fno-sanitize-recover=all
            )
            add_link_options(-fsanitize=undefined)
        endif()
        if(ENABLE_TSAN)
                target_compile_options(
        ${PROJECT_NAME} INTERFACE-fsanitize=thread)
            add_link_options(-fsanitize=thread)
        endif()
        if(ENABLE_MSAN)
                target_compile_options(
        ${PROJECT_NAME} -fsanitize=memory -fPIE -pie)
            add_link_options(-fsanitize=memory -fPIE -pie)
        endif()
        if(ENABLE_LSAN)
                target_compile_options(
        ${PROJECT_NAME} INTERFACE-fsanitize=leak)
            add_link_options(-fsanitize=leak)
        endif()
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(
        ${PROJECT_NAME} INTERFACE
            -O3                     # Maximum optimization
            -DNDEBUG               # Define NDEBUG
            -march=native          # Optimize for local machine
            -flto                  # Link-time optimization
            -ffunction-sections    # Place each function in its own section
            -fdata-sections        # Place each data item in its own section
            -ffast-math            # Aggressive math optimizations
            -fdevirtualize-at-ltrans # Devirtualize during link-time optimization
            -fipa-pta             # Interprocedural pointer analysis
            -fno-plt              # Disable PLT indirection
            $<$<CXX_COMPILER_ID:GNU>:-fgraphite-identity> # Loop optimizations
        )
        add_link_options(
            -Wl,-O3               # Linker optimizations
            -Wl,--gc-sections     # Remove unused sections
            -Wl,--as-needed       # Only link needed libraries
            -Wl,--strip-all       # Strip all symbols
        )
    endif()
endif()
# Enable testing
enable_testing()

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/std
)

file(GLOB_RECURSE TESTS_SOURCES
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
)

# Add tests
add_executable(${PROJECT_NAME}_test
    ${TESTS_SOURCES}
)
if(!MSVC)
    target_link_libraries(${PROJECT_NAME}_test pthread c)
endif()

target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Sanitizers:")
message(STATUS "  ASan: ${ENABLE_ASAN}")
message(STATUS "  UBSan: ${ENABLE_UBSAN}")
message(STATUS "  TSan: ${ENABLE_TSAN}")
message(STATUS "  MSan: ${ENABLE_MSAN}")
message(STATUS "  LSan: ${ENABLE_LSAN}")
message(STATUS "Documentation generation: ${ENABLE_GEN_DOCS_ON_BUILD}")

function(generate_docs_from_headers)
    find_package(Doxygen REQUIRED)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to generate documentation")
    endif()

    get_filename_component(source_dir_abs ${CMAKE_SOURCE_DIR} ABSOLUTE)
    get_filename_component(output_dir_abs ${CMAKE_BINARY_DIR}/docs ABSOLUTE)

    file(GLOB_RECURSE HEADER_FILES"${source_dir_abs}/*.h")

    string(REPLACE ";" " " HEADER_FILES_STR "${HEADER_FILES}")

    file(MAKE_DIRECTORY ${output_dir_abs})

    add_custom_target(docs ALL
        # Copy template Doxyfile
        COMMAND ${CMAKE_COMMAND} -E copy ${source_dir_abs}/docs/template_Doxyfile ${output_dir_abs}/Doxyfile
        COMMAND ${CMAKE_COMMAND} -E echo "INPUT = ${HEADER_FILES_STR}" >> ${output_dir_abs}/Doxyfile
        COMMAND ${CMAKE_COMMAND} -E echo "OUTPUT_DIRECTORY = ${output_dir_abs}" >> ${output_dir_abs}/Doxyfile
        COMMAND "${DOXYGEN_EXECUTABLE}" ${output_dir_abs}/Doxyfile
        WORKING_DIRECTORY ${output_dir_abs}
        COMMENT "Generating documentation in ${output_dir_abs}"
        DEPENDS ${HEADER_FILES}
        VERBATIM
    )

    add_custom_target(docs_clean
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${output_dir_abs}
        COMMENT "Cleaning documentation in ${output_dir_abs}"
    )
endfunction()

if(ENABLE_GEN_DOCS_ON_BUILD)
    generate_docs_from_headers()
endif(ENABLE_GEN_DOCS_ON_BUILD)