cmake_minimum_required (VERSION 2.8)
# YI_REL_SRC_DIR
macro(YiGetSubDirs result basedir)
    file(GLOB children RELATIVE ${basedir} ${basedir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${basedir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

set(YI_REL_PATH_TO_ROOT ../youiengine)
set(YI_APP_BUILD_DIR ${PROJECT_SOURCE_DIR}/../build)
set(YI_REL_SRC_DIR .)
set(YI_PROJECT_NAME Lovecraft)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${YI_REL_PATH_TO_ROOT}/cmake ${CMAKE_MODULE_PATH})

project (${YI_PROJECT_NAME}) 

file(GLOB_RECURSE YI_SRC_FILES 
    "${YI_REL_SRC_DIR}/*.h"
    "${YI_REL_SRC_DIR}/*.cpp")

set(YI_APP_COMMON_DIR ${YI_REL_PATH_TO_ROOT}/AppCommon)

set(YI_SRC_FILES_APPCOMMON ${YI_APP_COMMON_DIR}/WinMainDefault.cpp)

set(YI_SRC_FILES
	${YI_SRC_FILES}
	${YI_SRC_FILES_APPCOMMON}
)

include(${YI_REL_PATH_TO_ROOT}/AppCommon/build/AppCommon.cmake)
include(YiPaths)
YiInitPaths(${YI_REL_PATH_TO_ROOT})
include(YiToolChain)
include(YiIncludeLibs)
include(YiAppHarness)

include_directories(${YI_REL_SRC_DIR})
include_directories(${YI_APP_COMMON_DIR})

source_group("Source Files\\AppCommon"  FILES ${YI_SRC_FILES_APPCOMMON} )
source_group("Header Files\\AppCommon"  FILES ${YI_HDR_FILES_APPCOMMON} )
source_group("App"          REGULAR_EXPRESSION "app/.*")
source_group("Custom Views" REGULAR_EXPRESSION "customviews/.*")
source_group("Utilities"    REGULAR_EXPRESSION "utilities/.*")
source_group("Quest"    REGULAR_EXPRESSION "quest/.*")

YiAddAppHarness()
YiAddPlayerYouiEngineLibraries("INCLUDE_HEADERS_AND_LINK")
YiAddAllYouiEngineLibraries("INCLUDE_HEADERS_AND_LINK")
YiAddPlatformYouiEngineLibraries("INCLUDE_HEADERS_AND_LINK")
YiAddAllThirdpartyLibraries("LINK_ONLY")
YiAddNetworkYouiEngineLibraries("INCLUDE_HEADERS_AND_LINK")
YiAddNetworkThirdpartyLibraries("INCLUDE_HEADERS_AND_LINK")
YiAddLibrary("YI_LIB_DEP_GLFW" "INCLUDE_HEADERS_ONLY")
YiGetSubDirs(YI_REL_INCLUDE_SUB_DIRS "${YI_YOUIENGINE_DIR}/core/dist/include")

foreach(dir ${YI_REL_INCLUDE_SUB_DIRS})
	file(GLOB_RECURSE HEADERS "${YI_YOUIENGINE_DIR}/core/dist/include/${dir}/*.h")
	file(GLOB_RECURSE INLINED "${YI_YOUIENGINE_DIR}/core/dist/include/${dir}/*.inl")

	source_group("You.I Engine Header Files\\${dir}" FILES ${HEADERS} ${INLINED})
	list(APPEND YI_HEADER_FILES_PUBLIC ${HEADERS} ${INLINED})
endforeach()

set(YI_EXE_NAME ${YI_PROJECT_NAME})

# Adds a .vcxproj.user file which sets $(TargetDir) as the working directory for
# the debugger. This needs to be set in order to debug an application in Visual Studio.
if(CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013 Win64" OR
   CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013")
   configure_file (
      ${YI_APP_COMMON_DIR}/build/vs2013/vcxproj.user.in
      ${CMAKE_CURRENT_BINARY_DIR}/${YI_PROJECT_NAME}.vcxproj.user
      )
endif()
