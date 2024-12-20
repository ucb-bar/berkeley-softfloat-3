project(SoftFloat C)

set(SOURCE_DIR "source")
set(SPECIALIZE_TYPE "RISCV")

set(SOFTFLOAT_OPTS "-DSOFTFLOAT_ROUND_ODD -DINLINE_LEVEL=5 -DSOFTFLOAT_FAST_DIV32TO16 -DSOFTFLOAT_FAST_DIV64TO32")

include_directories("${SOURCE_DIR}/${SPECIALIZE_TYPE}")
include_directories("${SOURCE_DIR}/include")
include_directories("build/Linux-RISCV64-GCC")

set(CMAKE_C_FLAGS "-c -Werror-implicit-function-declaration -DSOFTFLOAT_FAST_INT64 ${SOFTFLOAT_OPTS} ${C_INCLUDES} -O2")

add_subdirectory(${SOURCE_DIR})
add_subdirectory(${SOURCE_DIR}/${SPECIALIZE_TYPE})

add_library(softfloat)
target_link_libraries(softfloat
    softfloat-prim
    softfloat-src
    softfloat-riscv
)
