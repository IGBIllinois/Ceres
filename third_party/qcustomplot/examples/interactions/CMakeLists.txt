

#-------------------------------------------------------------------------------
# This executable will use the Qt GUI framework.
#-------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)



set(ui_files
  mainwindow.ui
)


source_group("Resources" FILES
  ${ui_files}
  ${platform_resource_files}
)

#
# Axis Tags Example
#
add_executable(Interactions WIN32 MACOSX_BUNDLE)

target_compile_features(Interactions PRIVATE cxx_std_11)


target_sources(Interactions
PRIVATE

    ${ui_files}
    ${platform_resource_files}

    mainwindow.h

#
# General Application files
#
    main.cpp
    mainwindow.cpp
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET Interactions PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_link_libraries(Interactions PRIVATE qcustomplot_static)
target_link_libraries(Interactions PRIVATE ${Qt}::Core)
target_link_libraries(Interactions PRIVATE ${Qt}::Widgets)

set_target_properties(Interactions PROPERTIES FOLDER "Examples")


