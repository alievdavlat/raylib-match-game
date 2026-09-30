cmake_minimum_required(VERSION 3.21)
project(match_game C)

set(CMAKE_C_STANDARD 17)

find_package(raylib CONFIG REQUIRED)

add_executable(match-game
  src/main.c
  src/game.c
  src/board.c
  src/score.c
  src/audio.c
  src/render.c
)
target_link_libraries(match-game PRIVATE raylib)
target_compile_options(match-game PRIVATE -Wall -Wextra)

add_custom_command(TARGET match-game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:match-game>/resources)

# MSYS2 ships raylib as a DLL, so copy libraylib.dll and glfw3.dll next to the executable
if(WIN32 AND NOT VCPKG_TOOLCHAIN)
  find_package(glfw3 CONFIG REQUIRED)
  add_custom_command(TARGET match-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:raylib> $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:match-game>)
endif()
