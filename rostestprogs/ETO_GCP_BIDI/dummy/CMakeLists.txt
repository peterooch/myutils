
list(APPEND SOURCE
    WinMain.c
    AboutDialog.c
    MainWindow.c)

add_rc_deps(resource.rc ${CMAKE_CURRENT_SOURCE_DIR}/Application.ico)
add_executable(dummy ${SOURCE} resource.rc)
set_module_type(dummy win32gui UNICODE)
add_importlibs(dummy gdi32 user32 shell32 comctl32 msvcrt kernel32 ntdll)
add_cd_file(TARGET dummy DESTINATION reactos NO_CAB FOR bootcd)
