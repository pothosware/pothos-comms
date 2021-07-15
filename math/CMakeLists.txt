########################################################################
## Feature registration
########################################################################
cmake_dependent_option(ENABLE_COMMS_MATH "Enable Pothos Comms.Math component" ON "ENABLE_COMMS" OFF)
add_feature_info("  Math" ENABLE_COMMS_MATH "Mathematical operators for sample streams")
if (NOT ENABLE_COMMS_MATH)
    return()
endif()

########################################################################
# Math blocks module
########################################################################
if(MSVC)
    add_definitions(/bigobj) #arithmetic too large in debug mode
endif(MSVC)

# exp10 and exp10f are GNU extensions, so we know MSVC won't have them.
if(NOT MSVC)
    include(CheckSymbolExists)
    set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
    set(CMAKE_REQUIRED_LIBRARIES m)
    check_symbol_exists(exp10f "math.h" HAVE_EXP10F)
    check_symbol_exists(exp10 "math.h" HAVE_EXP10)
endif()

configure_file(
    Exp10.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/Exp10.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

POTHOS_MODULE_UTIL(
    TARGET MathBlocks
    SOURCES
        Abs.cpp
        TestAbs.cpp
        Angle.cpp
        TestAngle.cpp
        Arithmetic.cpp
        TestComparatorBlocks.cpp
        Comparator.cpp
        ConstComparator.cpp
        Conjugate.cpp
        TestConjugate.cpp
        Log.cpp
        TestLog.cpp
        TestArithmeticBlocks.cpp
        Scale.cpp
        TestScale.cpp
        Rotate.cpp
        TestRotate.cpp
        ConstArithmetic.cpp
        Sinc.cpp
        TestSinc.cpp
        Trigonometric.cpp
        TestTrigonometric.cpp
        Pow.cpp
        Root.cpp
        TestPowRoot.cpp
        RSqrt.cpp
        TestRSqrt.cpp
        Beta.cpp
        Gamma.cpp
        TestBeta.cpp
        TestGamma.cpp
        ErrorFunction.cpp
        TestErrorFunction.cpp
        Sigmoid.cpp
        TestSigmoid.cpp
        Exp.cpp
        TestExp.cpp
        ModF.cpp
        TestModF.cpp
    LIBRARIES
        CommsFunctions
        CommsTests
    DESTINATION comms
    ENABLE_DOCS
)

if(xsimd_FOUND)
    add_subdirectory(SIMD)
    target_link_libraries(MathBlocks PRIVATE CommsMathSIMD)
endif()
