cmake_minimum_required(VERSION 2.8)

# Don't use MFC
set(CMAKE_MFC_FLAG 0)

add_library(DirectInput SHARED
    directinput.cpp
    directinput.rc
    directinput.def
    )

if (MSVC)
    set_target_properties(DirectInput PROPERTIES LINK_FLAGS "/DEBUG")
endif()

add_definitions(
    -DUNICODE
    -D_UNICODE
    )

target_link_libraries(DirectInput dinput8.lib dxguid.lib)

configure_plugin(DirectInput)
