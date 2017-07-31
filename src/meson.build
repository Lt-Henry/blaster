
gcc = meson.get_compiler('c')
# libm = gcc.find_library('m')
libm = find_library('m')

libblaster=shared_library('blaster', ['math.c','color.c','texture.c','tga.c'],
    c_args:'-std=c11',
    include_directories: headers,
    dependencies: libm
    )
