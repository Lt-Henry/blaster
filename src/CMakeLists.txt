
#main 
add_library(blaster STATIC math.c texture.c)


#add custom lib flags
add_definitions(-std=c11 -O3)
