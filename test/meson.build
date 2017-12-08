
gcc = meson.get_compiler('c')
libm = gcc.find_library('m')

executable('test-blaster', ['main.c'],
    c_args:'-std=c11',
    include_directories: blaster_headers,
    link_with: libblaster,
    dependencies: libm
    )
