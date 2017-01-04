
#main 
add_library(blaster STATIC math.c texture.c texturepool.c color.c tile.c raster.c vbo.c)


#add custom lib flags
add_definitions(-std=c11 -O3)
