cmake_minimum_required(VERSION 3.5)

project(cdcontainers-benchmarks)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

find_package(Threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -flto -fno-exceptions -fno-rtti")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -flto")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g3")

set(GOOGLE_BENCHMARK_PATH "${CMAKE_CURRENT_BINARY_DIR}/google-benchmark")
externalproject_add(ex_benchmark
  GIT_REPOSITORY "git@github.com:google/benchmark.git"
  GIT_TAG "v1.5.0"
  INSTALL_DIR "${GOOGLE_BENCHMARK_PATH}"
  CMAKE_ARGS
    -DBENCHMARK_ENABLE_TESTING=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${GOOGLE_BENCHMARK_PATH}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
    -DBENCHMARK_ENABLE_LTO=true
)
link_directories("${GOOGLE_BENCHMARK_PATH}/lib")
include_directories("${GOOGLE_BENCHMARK_PATH}/include")


set(CDCONTAINERS_PATH "${CMAKE_CURRENT_BINARY_DIR}/cdcontainers")
externalproject_add(ex_cdcontainers
  GIT_REPOSITORY "git@github.com:maksimandrianov/cdcontainers.git"
  GIT_TAG "master"
  INSTALL_DIR "${CDCONTAINERS_PATH}"
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${CDCONTAINERS_PATH}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
)
link_directories("${CDCONTAINERS_PATH}/lib")
include_directories("${CDCONTAINERS_PATH}/include")


set(COLLECTIONSC_PATH "${CMAKE_CURRENT_BINARY_DIR}/collectionsc")
externalproject_add(ex_collectc
  GIT_REPOSITORY "git@github.com:srdja/Collections-C.git"
  GIT_TAG "master"
  INSTALL_DIR "${COLLECTIONSC_PATH}"
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${COLLECTIONSC_PATH}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
)
link_directories("${COLLECTIONSC_PATH}/lib")
include_directories("${COLLECTIONSC_PATH}/include")

set(GLIBC_PATH "${CMAKE_CURRENT_BINARY_DIR}/glibc")
externalproject_add(ex_glibc
  GIT_REPOSITORY "git@github.com:GNOME/glib.git"
  GIT_TAG "2.63.0"
  CONFIGURE_COMMAND meson ../ex_glibc
    --libdir=lib
    --prefix=${GLIBC_PATH}
  BUILD_COMMAND ninja -j4
  INSTALL_COMMAND ninja install
)
link_directories("${GLIBC_PATH}/lib/")
include_directories("${GLIBC_PATH}/lib/glib-2.0/include/")
include_directories("${GLIBC_PATH}/include/glib-2.0")

function(link binary_name filename)
  include_directories(.)
  add_executable(
    ${binary_name}
    ${filename}
    benchmarks/utils.cpp
    benchmarks/utils.hpp
  )
  add_dependencies(${binary_name} ex_benchmark ex_cdcontainers ex_collectc ex_glibc)
  target_link_libraries(
    ${binary_name}
    benchmark
    cdcontainers
    collectc
    glib-2.0
    ${CMAKE_THREAD_LIBS_INIT}
  )
endfunction()

link(bench_map benchmarks/bench_map.cpp)
link(bench_list benchmarks/bench_list.cpp)
link(bench_deque benchmarks/bench_deque.cpp)
