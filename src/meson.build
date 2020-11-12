
gcc = meson.get_compiler('c')
libm = gcc.find_library('m')
#libm = find_library('m')
thread_dep = dependency('threads')

libblaster=shared_library('blaster', ['vector.c','matrix.c','matrix_stack.c','color.c','texture.c','tga.c','vbo.c','raster.c','queue.c','worker.c'],
    c_args:'-std=c11',
    include_directories: blaster_headers,
    dependencies: [libm,thread_dep]
    )
    
blaster=declare_dependency(link_with:libblaster,include_directories: blaster_headers)
