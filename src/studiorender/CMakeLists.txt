cmake_minimum_required( VERSION 3.7 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" ) 
file( GLOB PUBLIC_FILES "../public/studiorender/*.h" )
file( GLOB SHADERS_FILES "shaders/*.h" "shaders/*.cpp" )
file( GLOB PRIVATE_FILES "../private/studiorender/*.h" )

set( MODULE_NAME studiorender )

set( GLEW_PATH ${EXTLIBS_DIR}/GLEW CACHE PATH "Path to GLEW" )
set( SDL2_PATH ${EXTLIBS_DIR}/SDL2 CACHE PATH "Path to SDL2" )

#
#   --- Указываем платформозависимые исходники ---
#

set( STDSHADERS_DLL "stdshaders" )
set( PATHS_H paths.h )

#   Платформозависимые исходники для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( SOURCE_FILES ${SOURCE_FILES}
                        win32/wglcontext.cpp
                        win32/wglcontext.h )

    set( ICON resource.rc )
    set( STDSHADERS_DLL ${STDSHADERS_DLL}.dll )

#   Платформозависимые исходники для Linux
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( SOURCE_FILES ${SOURCE_FILES}
                        unix/glxcontext.cpp
                        unix/glxcontext.h )

    set( ICON "" )
    set( STDSHADERS_DLL ${STDSHADERS_DLL}.so )
else()
    message( SEND_ERROR "Unknow platform")
endif()

#
#   --- Задаем комманды препроцессора ---
#

add_definitions( -DLIFEENGINE_EXPORT )

#
#   --- Настройки проекта ---
#

source_group( "Public Files" FILES ${PUBLIC_FILES} )
source_group( "Shaders Files" FILES ${SHADERS_FILES} )
source_group( "Private Files" FILES ${PRIVATE_FILES} )

configure_file( ${PATHS_H}.in ${PATHS_H} )
add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} ${PUBLIC_FILES} ${PRIVATE_FILES} ${SHADERS_FILES} ${ICON} )

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/binaries )
install( DIRECTORY "shaders/shaders" DESTINATION ${BUILD_DIR}/engine )

set_target_properties (${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
include_directories( ../public )
include_directories( ../private )
include_directories( ../ )

#
#   --- Ищим и подключаем зависимости ---
#

#---------------
#   OpenGL

find_package( OpenGL REQUIRED )
if( NOT OPENGL_FOUND )
    message( SEND_ERROR "Failed to find OpenGL" )
    return()
else()
    include_directories( ${OPENGL_INCLUDE_DIR} )
    target_link_libraries( ${MODULE_NAME} ${OPENGL_LIBRARIES} )
endif()

#---------------
#   GLEW

find_package( GLEW REQUIRED )
if( NOT GLEW_FOUND )
    message( SEND_ERROR "Failed to find GLEW" )
    return()
else()
    include_directories( ${GLEW_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${GLEW_LIB} )
endif()   

#---------------
#   SDL2

find_package( SDL2 REQUIRED )
if( NOT SDL2_FOUND )
    message( SEND_ERROR "Failed to find SDL2" )
    return()
else()
    include_directories( ${SDL2_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${SDL2_LIB} )
endif()

#---------------
