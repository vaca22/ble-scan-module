file(GLOB_RECURSE SOURCES ./*.c)

idf_component_register(
        SRCS
        ${SOURCES}
        INCLUDE_DIRS
        "."
        "pages"
        "configs"
        "services/audio"
        "services/battery"
        "services/button"
        "services/lv_port"
        "services/sdcard"
        "services/setting")
