
#main 
add_library(blaster SHARED math.c texture.c texturepool.c color.c tile.c command.c raster.c)


#add custom lib flags
add_definitions(-std=c11 -O3)
