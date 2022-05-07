project(chunky)

add_library(chunky SHARED
  display.c
  )
target_link_libraries(chunky PUBLIC raylib m dl pthread)
