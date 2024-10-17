cmake_minimum_required(VERSION 3.16)

project(cpuburnAVX)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source build not supported")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  endif()
endif()

find_package(OpenMP REQUIRED)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(FLAGS_AVX2 "/D__SSE2__;/D__SSE3__;/D__SSE4_1__;/D__AVX__;/D__AVX2__;/arch:AVX2")
  set(FLAGS_AVX512 "/D__SSE2__;/D__SSE3__;/D__SSE4_1__;/D__AVX__;/D__AVX2__;/D__AVX512F__;/arch:AVX2")
else()
  set(FLAGS_AVX2 "-mavx2;-mfma")
  set(FLAGS_AVX512 "-mavx512f")
endif()

if (WIN32)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1 _CRT_NONSTDC_NO_DEPRECATE=1)
endif()

add_executable(burnavx2 burnavx2.c main.c)
target_compile_options(burnavx2 PRIVATE ${FLAGS_AVX2} ${OpenMP_C_FLAGS})
target_link_options(burnavx2 PRIVATE ${OpenMP_C_FLAGS})

add_executable(burnavx512 burnavx512.c main.c)
target_compile_options(burnavx512 PRIVATE ${FLAGS_AVX512} ${OpenMP_C_FLAGS})
target_link_options(burnavx512 PRIVATE ${OpenMP_C_FLAGS})
