add_library(chunky SHARED
  Display.c
  )
target_link_libraries(chunky PUBLIC raylib)
