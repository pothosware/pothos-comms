########################################################################
## Comms function library
########################################################################
set(SOURCES
    fxpt_atan2.cpp
)

add_library(CommsFunctions STATIC ${SOURCES})
set_property(TARGET CommsFunctions PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_include_directories(CommsFunctions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
