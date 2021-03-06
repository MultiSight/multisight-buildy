cmake_minimum_required(VERSION 2.8)
project(buildy)

include(common.cmake NO_POLICY_SCOPE)
include(packaging.cmake NO_POLICY_SCOPE)

set(SOURCES source/Main.cpp
            source/Config.cpp
            source/Option.cpp
            source/GIT.cpp
            source/Utils.cpp
            source/Build.cpp
            source/Test.cpp)

set(WINDOWS_LIBS XSDK)
set(LINUX_LIBS XSDK)

if(CMAKE_SYSTEM MATCHES "Windows")
    set(RC_FILE "buildy.rc")
    set(APPLICATION_TYPE "WINDOWS_CONSOLE")
elseif(CMAKE_SYSTEM MATCHES "Linux")
    set(APPLICATION_TYPE "NORMAL")
endif(CMAKE_SYSTEM MATCHES "Windows")

include("${devel_artifacts_path}/build/base_app.cmake" NO_POLICY_SCOPE)

# Installation...

install(TARGETS buildy DESTINATION buildy)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_scripts DESTINATION buildy)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/example_config DESTINATION buildy)

if(CMAKE_SYSTEM MATCHES "Windows")
    install(FILES ${devel_artifacts_path}/lib/XSDK.dll DESTINATION buildy)
    if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
        install(FILES "c:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/redist/Debug_NonRedist/x64/Microsoft.VC120.DebugCRT/msvcp120d.dll" DESTINATION buildy)
        install(FILES "c:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/redist/Debug_NonRedist/x64/Microsoft.VC120.DebugCRT/msvcr120d.dll" DESTINATION buildy)
    else()
        install(FILES "c:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/redist/x64/Microsoft.VC120.CRT/msvcp120.dll" DESTINATION buildy)
        install(FILES "c:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/redist/x64/Microsoft.VC120.CRT/msvcr120.dll" DESTINATION buildy)
    endif()

elseif(CMAKE_SYSTEM MATCHES "Linux")
    install(FILES ${devel_artifacts_path}/lib/libXSDK.so DESTINATION buildy/libs)
    install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/utils/ssh_cmd DESTINATION buildy)
    install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/utils/scp_cmd DESTINATION buildy)
endif()
