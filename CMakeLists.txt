project(survival) 

set(SURVIVAL_VERSION 0.1.0)

cmake_minimum_required(VERSION 2.8)

IF(WIN32)
	add_definitions(-DLUABIND_DYNAMIC_LINK)
ELSE()
	add_definitions(-Wall -std=c++0x -O0 -g)
ENDIF()

include_directories(include $ENV{BOOST_ROOT} $ENV{LUA_INCLUDE} $ENV{LUABIND_INCLUDE} $ENV{ALLEGRO_INCLUDE})

link_directories($ENV{LUA_LIB} $ENV{LUABIND_LIB} $ENV{ALLEGRO_LIB})

FILE(GLOB SOURCE src/*.cpp)
FILE(GLOB HEADER include/*.h)

add_executable(survival ${SOURCE} ${HEADER})

if (WIN32)
	target_link_libraries(survival lua5.1 
		debug allegro-5.0.8-monolith-md-debug
		debug luabindd
		optimized allegro-5.0.8-monolith-md
		optimized luabind)
ELSE()
	target_link_libraries(survival allegro allegro_image allegro_primitives allegro_font allegro_ttf lua luabind dl)
ENDIF()