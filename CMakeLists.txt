cmake_minimum_required(VERSION 3.11)
project(VISCOMCore)
option(ASSIMP_NO_EXPORT ON)
# Disable all unused importers
option(ASSIMP_BUILD_3DS_IMPORTER OFF)
option(ASSIMP_BUILD_3D_IMPORTER OFF)
option(ASSIMP_BUILD_3MF_IMPORTER OFF)
option(ASSIMP_BUILD_AC_IMPORTER OFF)
option(ASSIMP_BUILD_AMF_IMPORTER OFF)
option(ASSIMP_BUILD_AMF_IMPORTER OFF)
option(ASSIMP_BUILD_ASE_IMPORTER OFF)
option(ASSIMP_BUILD_ASSBIN_IMPORTER OFF)
option(ASSIMP_BUILD_ASSXML_IMPORTER OFF)
option(ASSIMP_BUILD_B3D_IMPORTER OFF)
option(ASSIMP_BUILD_BLEND_IMPORTER OFF)
option(ASSIMP_BUILD_BVH_IMPORTER OFF)
option(ASSIMP_BUILD_COB_IMPORTER OFF)
option(ASSIMP_BUILD_CSM_IMPORTER OFF)
option(ASSIMP_BUILD_DXF_IMPORTER OFF)
option(ASSIMP_BUILD_GLTF_IMPORTER OFF)
option(ASSIMP_BUILD_HMP_IMPORTER OFF)
option(ASSIMP_BUILD_IFC_IMPORTER OFF)
option(ASSIMP_BUILD_IRRMESH_IMPORTER OFF)
option(ASSIMP_BUILD_IRR_IMPORTER OFF)
option(ASSIMP_BUILD_LWO_IMPORTER OFF)
option(ASSIMP_BUILD_LWS_IMPORTER OFF)
option(ASSIMP_BUILD_MD2_IMPORTER OFF)
option(ASSIMP_BUILD_MD3_IMPORTER OFF)
option(ASSIMP_BUILD_MD5_IMPORTER OFF)
option(ASSIMP_BUILD_MDC_IMPORTER OFF)
option(ASSIMP_BUILD_MDL_IMPORTER OFF)
option(ASSIMP_BUILD_MS3D_IMPORTER OFF)
option(ASSIMP_BUILD_NDO_IMPORTER OFF)
option(ASSIMP_BUILD_NFF_IMPORTER OFF)
option(ASSIMP_BUILD_NONFREE_C4D_IMPORTER OFF)
option(ASSIMP_BUILD_OFF_IMPORTER OFF)
option(ASSIMP_BUILD_OGRE_IMPORTER OFF)
option(ASSIMP_BUILD_OPENGEX_IMPORTER OFF)
option(ASSIMP_BUILD_Q3BSP_IMPORTER OFF)
option(ASSIMP_BUILD_Q3D_IMPORTER OFF)
option(ASSIMP_BUILD_RAW_IMPORTER OFF)
option(ASSIMP_BUILD_SIB_IMPORTER OFF)
option(ASSIMP_BUILD_SMD_IMPORTER OFF)
option(ASSIMP_BUILD_TERRAGEN_IMPORTER OFF)
option(ASSIMP_BUILD_X3D_IMPORTER OFF)
option(ASSIMP_BUILD_XGL_IMPORTER OFF)
option(ASSIMP_BUILD_X_IMPORTER OFF)

get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(GENERATOR_IS_MULTI_CONFIG)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugWorker" CACHE TYPE INTERNAL FORCE)
    set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS "Debug;DebugWorker")
    set(CMAKE_C_FLAGS_DEBUGWORKER ${CMAKE_C_FLAGS_DEBUG})
    set(CMAKE_CXX_FLAGS_DEBUGWORKER ${CMAKE_CXX_FLAGS_DEBUG})
    set(CMAKE_EXE_LINKER_FLAGS_DEBUGWORKER ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUGWORKER ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
    set(COMPILE_DEFINITIONS_DEBUGWORKER ${COMPILE_DEFINITIONS_DEBUG})
endif()


set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows.")
set(CMAKE_DEBUGWORKER_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows.")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows.")

set(G3_SHARED_LIB OFF CACHE BOOL "Build shared library." FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force compiler to create static libs." FORCE)
set(ADD_FATAL_EXAMPLE OFF CACHE BOOL "Fatal (fatal-crashes/contract) examples " FORCE)
set(ADD_BUILD_WIN_SHARED OFF CACHE BOOL "Build shared library on Windows" FORCE)
set(USE_DYNAMIC_LOGGING_LEVELS ON CACHE BOOL "Turn ON/OFF log levels. An disabled level will not push logs of that level to the sink. By default dynamic logging is disabled" FORCE)
set(CHANGE_G3LOG_DEBUG_TO_DBUG ON CACHE BOOL "Use DBUG logging level instead of DEBUG. By default DEBUG is the debugging level" FORCE)
set(VERSION "1.3.160-0")
add_subdirectory(extern/g3log EXCLUDE_FROM_ALL)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/assimp EXCLUDE_FROM_ALL)
set(OpenGL_GL_PREFERENCE LEGACY)

find_package(Doxygen)

set(VISCOM_CLIENTGUI ON CACHE BOOL "Use ImGui on clients.")
set(VISCOM_SYNCINPUT ON CACHE BOOL "Synchronize input from master to clients.")
set(VISCOM_CLIENTMOUSECURSOR ON CACHE BOOL "Show the mouse cursor on clients.")
set(VISCOM_APP_NAME "${APP_NAME}" CACHE STRING "Name of the application to be build.")
set(VISCOM_LOCAL_ONLY ON CACHE BOOL "Only do a local build without calibration information.")
set(VISCOM_USE_SGCT ON CACHE BOOL "Use SGCT for local builds.")
set(VISCOM_CONFIG_NAME "single" CACHE STRING "Name/directory of the configuration files to be used.")
set(VISCOM_VIRTUAL_SCREEN_X 3840 CACHE INTEGER "Virtual screen size in x direction.")
set(VISCOM_VIRTUAL_SCREEN_Y 2160 CACHE INTEGER "Virtual screen size in y direction.")
set(VISCOM_NEAR_PLANE_SIZE_X "1.7778" CACHE STRING "Size of the near plane in x direction.")
set(VISCOM_NEAR_PLANE_SIZE_Y "1.0" CACHE STRING "Size of the near plane in y direction.")
set(VISCOM_INSTALL_BASE_PATH "D:/LabShare/cluster/apps/" CACHE PATH "Path to install the project to (should be the shared apps directory).")
set(VISCOM_OPENGL_PROFILE "4.5" CACHE STRING "OpenGL profile version to use.")
# This could be changed to "off" if the default target doesn't need the tuio library
set(VISCOM_USE_TUIO ON CACHE BOOL "Use TUIO input library")
set(VISCOM_TUIO_PORT 3333 CACHE STRING "UDP Port for TUIO to listen on")

# Build-flags.
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -Wno-unused-function -Wno-unused-parameter -Wextra -Wpedantic")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

  # add compiler-options for AppleClang to ignore linker warnings from assimp on macOS
  if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    add_compile_options(-fvisibility=hidden -fvisibility-inlines-hidden)
  else()
    list(APPEND CORE_LIBS stdc++fs)
  endif()
elseif(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /EHsc /MP")
endif()

file(GLOB EXTERN_SOURCES_CORE
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_demo.cpp)

if (VISCOM_USE_SGCT)
    add_subdirectory(extern/sgct)
    include_directories(extern/sgct/include)
    list(APPEND COMPILE_TIME_DEFS VISCOM_USE_SGCT)
    set(VISCOM_SGCT_TAG "sgct")
    set(VISCOM_SGCT_WRAPPER_DIR ${PROJECT_SOURCE_DIR}/src_sgct/)
else()
    option(GLFW_BUILD_DOCS OFF)
    option(GLFW_BUILD_EXAMPLES OFF)
    option(GLFW_BUILD_TESTS OFF)
    option(GLFW_INSTALL OFF)
    add_subdirectory(extern/glfw EXCLUDE_FROM_ALL)

    set(VISCOM_SGCT_TAG "nosgct")
    list(APPEND COMPILE_TIME_DEFS GLEW_STATIC)
    list(APPEND EXTERN_SOURCES_CORE src_nosgct/glew/src/glew.c)
    set(VISCOM_SGCT_WRAPPER_DIR ${PROJECT_SOURCE_DIR}/src_nosgct/)
endif()


file(GLOB_RECURSE SHADER_FILES_CORE ${PROJECT_SOURCE_DIR}/resources/shader/*.*)
file(GLOB_RECURSE SRC_FILES_CORE
    ${PROJECT_SOURCE_DIR}/src/core/*.h
    ${PROJECT_SOURCE_DIR}/src/core/*.cpp
    ${PROJECT_SOURCE_DIR}/src/core/*.inl
    ${VISCOM_SGCT_WRAPPER_DIR}/core/*.h
    ${VISCOM_SGCT_WRAPPER_DIR}/core/*.cpp
    ${VISCOM_SGCT_WRAPPER_DIR}/core/*.inl)
foreach(f ${VISCOM_SGCT_WRAPPER_DIR})
    message(${f})
endforeach()
source_group("extern\\core" FILES ${EXTERN_SOURCES_CORE})
source_group("shader\\core" FILES ${SHADER_FILES_CORE})

if (VISCOM_LOCAL_ONLY)
    list(REMOVE_ITEM SRC_FILES_CORE "${PROJECT_SOURCE_DIR}/src/core/CalibrationVertices.h")
    list(REMOVE_ITEM SRC_FILES_CORE "${VISCOM_SGCT_WRAPPER_DIR}/core/app_internal/WorkerNodeCalibratedInternal.h")
    list(REMOVE_ITEM SRC_FILES_CORE "${VISCOM_SGCT_WRAPPER_DIR}/core/app_internal/WorkerNodeCalibratedInternal.cpp")
    list(REMOVE_ITEM SRC_FILES_CORE "${PROJECT_SOURCE_DIR}/src/core/OpenCVParserHelper.h")
    list(REMOVE_ITEM SRC_FILES_CORE "${PROJECT_SOURCE_DIR}/src/core/OpenCVParserHelper.cpp")
endif()


foreach(f ${SRC_FILES_CORE})
    file(RELATIVE_PATH SRCGR ${PROJECT_SOURCE_DIR}/extern/fwcore ${f})
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
    string(REPLACE / \\ SRCGR ${SRCGR})
    string(REPLACE src_sgct src SRCGR ${SRCGR})
    string(REPLACE src_nosgct src SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
if (VISCOM_USE_SGCT)
    #find_library(SGCT_RELEASE_LIBRARY NAMES sgct libsgct PATHS $ENV{SGCT_ROOT_DIR}/lib REQUIRED)
    #find_library(SGCT_DEBUG_LIBRARY NAMES sgctd libsgctd PATHS $ENV{SGCT_ROOT_DIR}/lib REQUIRED)
    find_path(SGCT_INCLUDE_DIRECTORY NAMES sgct PATHS $ENV{SGCT_ROOT_DIR}/include NO_DEFAULT_PATH REQUIRED)

    set(SGCT_LIBS
        debug ${SGCT_DEBUG_LIBRARY}
        optimized ${SGCT_RELEASE_LIBRARY})

    list(APPEND CORE_LIBS sgct_light freetype glew glfw png16_static turbojpeg-static tinyxml2static zlibstatic miniziplibstatic vrpn OpenGL::OpenGL OpenGL::GLX )
else()
	list(APPEND CORE_LIBS glfw ${GLFW_LIBRARIES} OpenGL::OpenGL)
    list(APPEND CORE_INCLUDE_DIRS extern/glm extern/glfw/include)
endif()

list(APPEND CORE_INCLUDE_DIRS
    src
    ${VISCOM_SGCT_WRAPPER_DIR}
    extern/g3log/src
    ${CMAKE_CURRENT_BINARY_DIR}/extern/g3log/include
    extern/imgui
    extern/stb
    extern/assimp/include
    ${CMAKE_CURRENT_BINARY_DIR}/extern/assimp/include)

list(APPEND CORE_LIBS g3logger assimp)

if(MSVC)
    list(APPEND COMPILE_TIME_DEFS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN NOMINMAX)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
endif()

if (VISCOM_CLIENTMOUSECURSOR)
    list(APPEND COMPILE_TIME_DEFS VISCOM_CLIENTGUI VISCOM_SYNCINPUT VISCOM_CLIENTMOUSECURSOR)
else()
    if(VISCOM_SYNCINPUT)
        list(APPEND COMPILE_TIME_DEFS VISCOM_SYNCINPUT)
    endif()
    if(VISCOM_CLIENTGUI)
        list(APPEND COMPILE_TIME_DEFS VISCOM_CLIENTGUI)
    endif()
endif()

if (VISCOM_LOCAL_ONLY)
    list(APPEND COMPILE_TIME_DEFS VISCOM_LOCAL_ONLY)
endif()

if(VISCOM_USE_TUIO)
    add_subdirectory(extern/tuio EXCLUDE_FROM_ALL)
    list(APPEND COMPILE_TIME_DEFS VISCOM_USE_TUIO)
    list(APPEND CORE_LIBS libTUIO)
    list(APPEND CORE_INCLUDE_DIRS extern/tuio/TUIO extern/tuio/oscpack)
endif()

if(TARGET Doxygen::doxygen)
    get_property(DOXYGEN_EXECUTABLE TARGET Doxygen::doxygen PROPERTY IMPORTED_LOCATION)

    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    doxygen_add_docs(doc_doxygen ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc COMMENT "Generating API documentation with Doxygen")
    ## doxygen_add_docs(VISCOMCoreDoc src)
endif()

add_library(VISCOMCore ${SRC_FILES_CORE} ${SHADER_FILES_CORE} ${EXTERN_SOURCES_CORE})
set_property(TARGET VISCOMCore PROPERTY CXX_STANDARD 17)
target_include_directories(VISCOMCore PUBLIC ${CORE_INCLUDE_DIRS})
target_link_libraries(VISCOMCore ${CORE_LIBS})
target_compile_definitions(VISCOMCore PUBLIC ${COMPILE_TIME_DEFS})

macro(copy_core_lib_dlls APP_NAME)
    if (${VISCOM_USE_TUIO})
        add_custom_command(TARGET ${APP_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:libTUIO> ${PROJECT_BINARY_DIR})
        install(FILES $<TARGET_FILE:libTUIO> DESTINATION ${VISCOM_INSTALL_BASE_PATH}/${VISCOM_APP_NAME})
    endif()
endmacro()

install(DIRECTORY resources/ DESTINATION ${VISCOM_INSTALL_BASE_PATH}/${VISCOM_APP_NAME}/resources)
