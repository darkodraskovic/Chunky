project(chunky)

add_library(chunky STATIC
  display.c
  )

if (WIN32)
  target_link_libraries(chunky raylib opengl32 winmm)
else()
  target_link_libraries(chunky PUBLIC raylib m dl pthread)
endif()
