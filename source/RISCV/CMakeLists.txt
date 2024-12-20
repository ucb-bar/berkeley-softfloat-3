project(SoftFloat C)

add_library(softfloat-riscv
  OBJECT
  softfloat_raiseFlags.c
  s_f16UIToCommonNaN.c
  s_commonNaNToF16UI.c
  s_propagateNaNF16UI.c
  s_bf16UIToCommonNaN.c
  s_commonNaNToBF16UI.c
  s_f32UIToCommonNaN.c
  s_commonNaNToF32UI.c
  s_propagateNaNF32UI.c
  s_f64UIToCommonNaN.c
  s_commonNaNToF64UI.c
  s_propagateNaNF64UI.c
  extF80M_isSignalingNaN.c
  s_extF80UIToCommonNaN.c
  s_commonNaNToExtF80UI.c
  s_propagateNaNExtF80UI.c
  f128M_isSignalingNaN.c
  s_f128UIToCommonNaN.c
  s_commonNaNToF128UI.c
  s_propagateNaNF128UI.c
)
