set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

message("INFO survival_core_tests settings:")
message("INFO SURVIVAL_CORE_TESTS_INCLUDE_DIR = ${SURVIVAL_CORE_TESTS_INCLUDE_DIR}")
message("INFO SURVIVAL_CORE_TESTS_SRC_DIR = ${SURVIVAL_CORE_TESTS_SRC_DIR}")

file(GLOB SURVIVAL_CORE_TESTS_HEADER_FILES ${SURVIVAL_CORE_TESTS_INCLUDE_DIR}/*.h)
file(GLOB SURVIVAL_CORE_TESTS_SOURCE_FILES ${SURVIVAL_CORE_TESTS_SRC_DIR}/*.cpp)

add_definitions(-DLUABIND_DYNAMIC_LINK)

include_directories(
    ${Boost_INCLUDE_DIR})

link_directories(
    ${Boost_LIBRARY_DIRS}    
	${Lua_LIBRARY_DIRS}
    ${Luabind_LIBRARY_DIRS}
    ${Allegro_LIBRARY_DIRS})

add_executable(survival_core_tests ${SURVIVAL_CORE_TESTS_SOURCE_FILES} ${SURVIVAL_CORE_TESTS_HEADER_FILES})
target_link_libraries(survival_core_tests 
    survival_core)
