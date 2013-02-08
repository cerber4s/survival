set (SURVIVAL_CORE_VERSION_MAJOR 0)
set (SURVIVAL_CORE_VERSION_MINOR 1)
set (SURVIVAL_CORE_VERSION_PATCH 0)
set (SURVIVAL_CORE_VERSION_TWEAK 0-dev)

#configure_file(
#    "${MYLIB_SRC_DIR}/my_lib_config.h.in"
#    "${MYLIB_INCLUDE_DIR}/my_lib_config.h"
#)

message("INFO survival_core settings:")
message("INFO SURVIVAL_CORE_INCLUDE_DIR = ${SURVIVAL_CORE_INCLUDE_DIR}")
message("INFO SURVIVAL_CORE_SRC_DIR = ${SURVIVAL_CORE_SRC_DIR}")

add_definitions(-DLUABIND_DYNAMIC_LINK)

file(GLOB SURVIVAL_CORE_HEADER_FILES ${SURVIVAL_CORE_INCLUDE_DIR}/*.h)
file(GLOB SURVIVAL_CORE_SOURCE_FILES ${SURVIVAL_CORE_SRC_DIR}/*.cpp)

include_directories( 
    ${SURVIVAL_CORE_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${Lua_INCLUDE_DIR}
    ${Luabind_INCLUDE_DIR}
    ${Allegro_INCLUDE_DIR})

link_directories( 
	${Lua_LIBRARY_DIRS}
    ${Luabind_LIBRARY_DIRS}
    ${Allegro_LIBRARY_DIRS})

add_library(survival_core ${SURVIVAL_CORE_SOURCE_FILES} ${SURVIVAL_CORE_HEADER_FILES})
target_link_libraries(survival_core    
    debug ${Lua_DEBUG_LIBS}
    debug ${Luabind_DEBUG_LIBS}
    debug ${Allegro_DEBUG_LIBS}
    optimized ${Lua_OPTIMIZED_LIBS} 
    optimized ${Luabind_OPTIMIZED_LIBS}
    optimized ${Allegro_OPTIMIZED_LIBS})