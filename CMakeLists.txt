cmake_minimum_required(VERSION 2.8)
project(buildy)

include(common.cmake NO_POLICY_SCOPE)
include(packaging.cmake NO_POLICY_SCOPE)

set(SOURCES source/Main.cpp
            source/Config.cpp
            source/Option.cpp
            source/GIT.cpp
            source/Utils.cpp
            source/Build.cpp)

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
install(FILES ${devel_artifacts_path}/lib/libXSDK.so DESTINATION buildy/libs)
