cmake_minimum_required(VERSION 2.8)
project(survival)

set(SURVIVAL_VERSION_MAJOR 0)
set(SURVIVAL_VERSION_MINOR 1)
set(SURVIVAL_VERSION_PATCH 0)
set(SURVIVAL_VERSION_TWEAK 0-dev)

# 3rd party libraries
#set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Boost REQUIRED)

set(Lua_INCLUDE_DIR d:/projects/kevin/libs/lua-5.1.4-bin/include)
set(Lua_LIBRARY_DIRS d:/projects/kevin/libs/lua-5.1.4-bin)
set(Lua_DEBUG_LIBS lua5.1)
set(Lua_OPTIMIZED_LIBS lua5.1)

set(Luabind_INCLUDE_DIR d:/projects/kevin/libs/luabind-0.9.1)
set(Luabind_LIBRARY_DIRS d:/projects/kevin/libs/luabind-0.9.1/libs)
set(Luabind_DEBUG_LIBS luabindd)
set(Luabind_OPTIMIZED_LIBS luabind)
add_definitions(-DLUABIND_DYNAMIC_LINK)

set(Allegro_INCLUDE_DIR 
	D:/Projects/Kevin/libs/allegro-5.0.9/allegro/include 
	D:/Projects/Kevin/libs/allegro-5.0.9/allegro/bin/include
	D:/Projects/Kevin/libs/allegro-5.0.9/allegro/addons/font
	D:/Projects/Kevin/libs/allegro-5.0.9/allegro/addons/ttf
	D:/Projects/Kevin/libs/allegro-5.0.9/allegro/addons/image
	D:/Projects/Kevin/libs/allegro-5.0.9/allegro/addons/primitives)
set(Allegro_LIBRARY_DIRS D:/Projects/Kevin/libs/allegro-5.0.9/allegro/bin/lib/RelWithDebInfo)
set(Allegro_DEBUG_LIBS 
	allegro
	allegro_image
	allegro_primitives
	allegro_font
	allegro_ttf)
set(Allegro_OPTIMIZED_LIBS 
	allegro
	allegro_image
	allegro_primitives
	allegro_font
	allegro_ttf)

# project settings
set(SURVIVAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SURVIVAL_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SURVIVAL_HEADER_FILES ${SURVIVAL_INCLUDE_DIR}/*.h)
file(GLOB SURVIVAL_SOURCE_FILES ${SURVIVAL_SRC_DIR}/*.cpp)

#IF(WIN32)
#	add_definitions(-DLUABIND_DYNAMIC_LINK)
#ELSE()
#	add_definitions(-Wall -std=c++0x -O0 -g)
#ENDIF()

set(SURVIVAL_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/survival_core/include)
set(SURVIVAL_CORE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/survival_core/src)

set(SURVIVAL_CORE_TESTS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/survival_core_tests/include)
set(SURVIVAL_CORE_TESTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/survival_core_tests/src)

include_directories( 
    ${SURVIVAL_INCLUDE_DIR}
    ${SURVIVAL_CORE_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${Lua_INCLUDE_DIR}
    ${Luabind_INCLUDE_DIR}
    ${Allegro_INCLUDE_DIR})

link_directories(
    ${Lua_LIBRARY_DIRS}
    ${Luabind_LIBRARY_DIRS}
    ${Allegro_LIBRARY_DIRS})

add_subdirectory(survival_core) 
add_subdirectory(survival_core_tests) 
    
add_executable(survival ${SURVIVAL_SOURCE_FILES} ${SURVIVAL_HEADER_FILES})
target_link_libraries(survival
    survival_core
    debug ${Lua_DEBUG_LIBS}
    debug ${Luabind_DEBUG_LIBS}
    debug ${Allegro_DEBUG_LIBS}
    optimized ${Lua_OPTIMIZED_LIBS} 
    optimized ${Luabind_OPTIMIZED_LIBS}
    optimized ${Allegro_OPTIMIZED_LIBS})
    
enable_testing()
add_test(survival_core_tests survival_core_tests/survival_core_tests --log_level=all)
