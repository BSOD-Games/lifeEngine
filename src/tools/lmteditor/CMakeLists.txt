cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB_RECURSE SOURCE_FILES "*.h" "*.cpp" )
file( GLOB_RECURSE FORMS_FILES "*.ui" )
file( GLOB_RECURSE RESOURCES "*.qrc" )

set( MODULE_NAME lmteditor )
set( NODEEDITOR_PATH ${EXTLIBS_DIR}/nodeeditor CACHE PATH "Path to NodeEditor" )

#
#   --- Платформозависимые настройки ---
#

set( ENGINE_DLL "engine" )
set( PATHS_H paths.h )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( RESOURCE resource.rc )
    set( ENGINE_DLL ${ENGINE_DLL}.dll )

elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
     set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie" )
     set( RESOURCE "" )
     set( ENGINE_DLL ${ENGINE_DLL}.so )
else()
    message( SEND_ERROR "Unknow platform" )
endif()

#
#   --- Ищим и подключаем зависимости ---
#

find_package( Qt5 COMPONENTS Widgets REQUIRED )

#
#   --- Настройки проекта ---
#

qt5_wrap_ui( UI_HEADERS ${FORMS_FILES} )
qt5_add_resources ( RCC_SOURCES ${RESOURCES} )

configure_file( ${PATHS_H}.in ${PATHS_H} )
add_executable( ${MODULE_NAME} ${SOURCE_FILES} ${RCC_SOURCES} ${UI_HEADERS} ${FORMS_FILES} ${RESOURCE} )
target_link_libraries( ${MODULE_NAME} Qt5::Widgets Qt5::Core ${CMAKE_DL_LIBS} )

install( DIRECTORY styles DESTINATION ${BUILD_DIR}/engine )
install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )

include_directories( ../../public )
include_directories( ../../ )
include_directories( ./ )

#
#   --- Подключаем зависимости ---
#

#---------------
#   NodeEditor

find_package( NodeEditor REQUIRED )
if( NOT NODEEDITOR_FOUND )
    message( SEND_ERROR "Failed to find NodeEditor" )
    return()
else()
    include_directories( ${NODEEDITOR_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${NODEEDITOR_LIB} )
endif()