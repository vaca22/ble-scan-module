
set(COMPONENT_ADD_INCLUDEDIRS ./source)

set(COMPONENT_SRCS
    source/diskio.c
    source/ff.c
    source/ffsystem.c
    source/ffunicode.c
    )

set(COMPONENT_REQUIRES port)

register_component()


