cmake_minimum_required( VERSION 2.8 )

project( escape_groom_input )
add_executable( escape_groom_input src/INPUT.cpp )
target_link_libraries( escape_groom_input SDL2 )

project( escape_groom_output )
add_executable( escape_groom_output src/OUTPUT.cpp )
target_link_libraries( escape_groom_output ncurses pthread )
