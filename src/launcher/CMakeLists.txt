cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные и пути к исходникам ---
#

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( SOURCE_FILE launcher_win32.cpp )
	set( ICON resource.rc )
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
