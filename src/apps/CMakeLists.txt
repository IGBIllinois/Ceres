

#******************************************************************************
# Ceres - The main data acquisition program
#******************************************************************************

message("Creating the main data acquisition application...")
add_subdirectory(Ceres)


#******************************************************************************
# Remote Client - The remote client that moves data acquisition to the dolly
#******************************************************************************

message("Creating the remote client application...")
add_subdirectory(RemoteClient)


#******************************************************************************
# Data Viewer
#******************************************************************************

message("Creating data viewer application...")
add_subdirectory(DataViewer)

