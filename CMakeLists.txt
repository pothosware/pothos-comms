########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.1.0)
project(PothosComms CXX C)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    find_package(Pothos "0.6.0" CONFIG REQUIRED)
else()
    find_package(Pothos CONFIG REQUIRED) #in-tree build
endif()

########################################################################
## Feature registration
########################################################################
include(FeatureSummary)
include(CMakeDependentOption)
cmake_dependent_option(ENABLE_COMMS "Enable Pothos Comms component" ON "Pothos_FOUND" OFF)
add_feature_info(Comms ENABLE_COMMS "A collection of DSP/communications blocks")

########################################################################
# json.hpp header
########################################################################
find_path(JSON_HPP_INCLUDE_DIR NAMES json.hpp PATH_SUFFIXES nlohmann)

if (NOT JSON_HPP_INCLUDE_DIR)
    message(WARNING "Pothos Comms toolkit requires json.hpp, skipping...")
endif (NOT JSON_HPP_INCLUDE_DIR)

########################################################################
# SIMD dynamic dispatch, if supported by this Pothos build
########################################################################
include(PothosConfigSIMD OPTIONAL RESULT_VARIABLE SIMD_SUPPORTED)
if(SIMD_SUPPORTED)
    include(CheckIncludeFileCXX)
    set(CMAKE_REQUIRED_INCLUDES ${Pothos_INCLUDE_DIRS})
    check_include_file_cxx(Pothos/Util/XSIMDTraits.hpp HAS_POTHOS_XSIMD_TRAITS_HPP)

    if(HAS_POTHOS_XSIMD_TRAITS_HPP)
        find_package(xsimd)
        if(xsimd_FOUND)
            include_directories(${CMAKE_CURRENT_SOURCE_DIR})
            add_definitions(-DPOTHOS_XSIMD)
        endif()
    endif()
endif()

########################################################################
# Spuce library
########################################################################
if(NOT SPUCE_IN_TREE)
    find_package(Spuce CONFIG)
endif(NOT SPUCE_IN_TREE)

if (Spuce_FOUND)
    message(STATUS "Spuce_VERSION: ${Spuce_VERSION}")
    message(STATUS "Spuce_INCLUDE_DIRS: ${Spuce_INCLUDE_DIRS}")
    message(STATUS "Spuce_LIBRARIES: ${Spuce_LIBRARIES}")
    if (NOT TARGET spuce)
    add_library(spuce INTERFACE)
    target_link_libraries(spuce INTERFACE "${Spuce_LIBRARIES}")
    target_include_directories(spuce INTERFACE "${Spuce_INCLUDE_DIRS}")
    endif()
else (Spuce_FOUND)
    message(WARNING "Spuce filter designer library not found...")
endif (Spuce_FOUND)

########################################################################
# Common testing library
########################################################################
add_library(CommsTests INTERFACE)
target_include_directories(CommsTests INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

########################################################################
# Build subdirectories
########################################################################
add_subdirectory(functions)
add_subdirectory(demod)
add_subdirectory(digital)
add_subdirectory(fft)
add_subdirectory(filter)
add_subdirectory(mac)
add_subdirectory(math)
add_subdirectory(utility)
add_subdirectory(waveform)
