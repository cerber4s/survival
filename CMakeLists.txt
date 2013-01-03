project(survival)

set(SURVIVAL_VERSION 0.1.0)

cmake_minimum_required(VERSION 2.8)

add_definitions(-Wall -std=c++0x -O0 -g)

include_directories(include)

add_executable(survival 
  src/application 
  src/color
  src/entity 
  src/luastate
  src/main
  src/matrix3
  src/rendersystem
  src/vector2d
  )

target_link_libraries(survival allegro allegro_image allegro_primitives allegro_font allegro_ttf lua luabind dl)
