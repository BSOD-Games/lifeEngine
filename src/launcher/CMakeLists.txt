cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные и пути к исходникам ---
#

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( SOURCE_FILE launcher_win32.cpp )
    set( ICON resource.rc )

elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
     set( SOURCE_FILE launcher_unix.cpp )   
     set( SDL2_PATH ${EXTLIBS_DIR}/SDL2 CACHE PATH "Path to SDL2" )
     set( ICON "" )
else()
    message( SEND_ERROR "Unknow platform" )
endif()

set( MODULE_NAME launcher )

#
#   --- Настройки проекта ---
#

add_executable( ${MODULE_NAME} WIN32 ${SOURCE_FILE} ${ICON} )
target_link_libraries( ${MODULE_NAME} ${CMAKE_DL_LIBS} )
install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR} )
include_directories( ../ )
include_directories( ../public )

#
#   --- Ищим и подключаем зависимости ---
#

#---------------
#   SDL2

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    find_package( SDL2 REQUIRED )
    if( NOT SDL2_FOUND )
        message( SEND_ERROR "Failed to find SDL2" )
        return()
    else()
        include_directories( ${SDL2_INCLUDE} )
        target_link_libraries( ${MODULE_NAME} ${SDL2_LIB} ${SDL2MAIN_LIB} )
    endif()
endif()
