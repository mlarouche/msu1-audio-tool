set(msu1_SRCS
Builder.cpp
BuildInfo.cpp
CFileWrapper.cpp
ConfigFile.cpp
GlobalSettings.cpp
main.cpp
StringFormat.cpp
TrackSettings.cpp
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(msu1 ${msu1_SRCS})
target_link_libraries(msu1 libsox gsm lpc10)
