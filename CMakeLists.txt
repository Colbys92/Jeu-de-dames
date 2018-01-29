cmake_minimum_required(VERSION 2.6)



project (MOPSI)

add_executable (MOPSI arbre.cpp arbre.h tools.h)
add_executable (algoGenetique arbre.h tools.h algogenetique.cpp)


