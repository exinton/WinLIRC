cmake_minimum_required(VERSION 2.8)

# Use MFC in a shared DLL.
set(CMAKE_MFC_FLAG 2)

add_library(CommandIR SHARED
	CommandIR.cpp
	Globals.cpp
	hardware.cpp
	SendReceiveData.cpp
	Settings.cpp
	winlirc.cpp
	CommandIRUtils/libcmdir.c

    Globals.h
    SendReceiveData.h
    Settings.h
    usb.h
    winlirc.h
	)

add_definitions(
	-DUNICODE
	-D_UNICODE
	-DWIN
	)

target_link_libraries(CommandIR
	Common
	${CMAKE_CURRENT_SOURCE_DIR}/libusb.lib
	)

configure_plugin(CommandIR)