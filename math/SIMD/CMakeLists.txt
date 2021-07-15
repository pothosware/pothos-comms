########################################################################
## Make a static library with the SIMD implementations because MSVC
## doesn't like MathBlocksDocs.cpp depending on too many things.
########################################################################

set(SIMDInputs
    Abs.cpp
    Arithmetic.cpp
    Beta.cpp
    Comparator.cpp
    Conjugate.cpp
    ConstArithmetic.cpp
    ConstComparator.cpp
    ErrorFunction.cpp
    Gamma.cpp
    Exp.cpp
    Log.cpp
    ModF.cpp
    Pow.cpp
    Root.cpp
    RSqrt.cpp
    Sigmoid.cpp
    Sinc.cpp
    Trigonometric.cpp)

PothosGenerateSIMDSources(
    SIMDSources
    MathBlocks.json
    ${SIMDInputs})

add_library(CommsMathSIMD STATIC ${SIMDSources})
target_link_libraries(CommsMathSIMD PRIVATE xsimd)
target_link_libraries(CommsMathSIMD PRIVATE Pothos)
target_include_directories(CommsMathSIMD PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
add_dependencies(CommsMathSIMD MathBlocks_SIMDDispatcher)
set_property(TARGET CommsMathSIMD PROPERTY POSITION_INDEPENDENT_CODE TRUE)

# This library is pure templates, so expect large object files.
if(MSVC)
    set_property(TARGET CommsMathSIMD PROPERTY COMPILE_FLAGS /bigobj)
endif()
