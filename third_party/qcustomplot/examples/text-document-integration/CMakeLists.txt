

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
# Text Document Integration Example
#
add_executable(TextDocumentIntegration WIN32 MACOSX_BUNDLE)

target_compile_features(TextDocumentIntegration PRIVATE cxx_std_11)


target_sources(TextDocumentIntegration
PRIVATE

    ${ui_files}
    ${platform_resource_files}

    mainwindow.h
	qcpdocumentobject.h

#
# General Application files
#
    main.cpp
    mainwindow.cpp
	qcpdocumentobject.cpp
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET TextDocumentIntegration PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_link_libraries(TextDocumentIntegration PRIVATE qcustomplot_static)
target_link_libraries(TextDocumentIntegration PRIVATE ${Qt}::Core)
target_link_libraries(TextDocumentIntegration PRIVATE ${Qt}::Widgets)

set_target_properties(TextDocumentIntegration PROPERTIES FOLDER "Examples")


