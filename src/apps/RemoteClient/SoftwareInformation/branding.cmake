set(SOFTWARE_NAME "Ceres Remote Client")
set(VENDOR "University of Illinois at Urbana-Champaign")
set(PROJECT_NAME "${SOFTWARE_NAME}")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${SOFTWARE_NAME}")
set(CPACK_PACKAGE_NAME "${SOFTWARE_NAME}")
set(CPACK_PACKAGE_VENDOR "${VENDOR}")

set(CERES_SPLASH_IMAGE Splash.jpg)		# (image display when the software is loading)
set(CERES_BUNDLE_ICON ripe.icns)		# (bunbled app icon)
set(CERES_APPLICATION_ICON ripe.ico)	# (app icon)

add_definitions( -DPROJECT_NAME="${PROJECT_NAME}" )
add_definitions( -DSOFTWARE_NAME="${SOFTWARE_NAME}" )
add_definitions( -DVENDOR="UIUC" ) # spaces here confuses the vtkWrap
