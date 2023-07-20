

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
add_executable(AxisTags WIN32 MACOSX_BUNDLE)

target_compile_features(AxisTags PRIVATE cxx_std_11)


target_sources(AxisTags
PRIVATE

    ${ui_files}
    ${platform_resource_files}

    mainwindow.h
    axistag.h

#
# General Application files
#
    main.cpp
    mainwindow.cpp
    axistag.cpp
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET AxisTags PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_link_libraries(AxisTags PRIVATE qcustomplot_static)
target_link_libraries(AxisTags PRIVATE ${Qt}::Core)
target_link_libraries(AxisTags PRIVATE ${Qt}::Widgets)

set_target_properties(AxisTags PROPERTIES FOLDER "Examples")


