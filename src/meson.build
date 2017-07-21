
libm = find_library('m')

libblaster=shared_library('blaster', ['math.c','color.c'],
    c_args:'-std=c11',
    include_directories: headers,
    dependencies: libm
    )
