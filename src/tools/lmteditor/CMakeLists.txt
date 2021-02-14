cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB_RECURSE SOURCE_FILES "*.h" "*.cpp" )
file( GLOB_RECURSE FORMS_FILES "*.ui" )
file( GLOB_RECURSE RESOURCES "*.qrc" )

set( MODULE_NAME lmteditor )

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
add_executable( ${MODULE_NAME} WIN32 ${SOURCE_FILES} ${RCC_SOURCES} ${UI_HEADERS} ${FORMS_FILES} ${RESOURCE} )
set_target_properties (${MODULE_NAME} PROPERTIES FOLDER "Engine//Tools" )

target_link_libraries( ${MODULE_NAME} lmtloader )
target_link_libraries( ${MODULE_NAME} Qt5::Widgets Qt5::Core ${CMAKE_DL_LIBS} )

install( DIRECTORY resources/materials DESTINATION ${BUILD_DIR}/${BUILD_ENGINE_DIR} )
install( DIRECTORY resources/models DESTINATION ${BUILD_DIR}/${BUILD_ENGINE_DIR} )
install( DIRECTORY resources/styles DESTINATION ${BUILD_DIR}/${BUILD_ENGINE_DIR} )
install( DIRECTORY resources/textures DESTINATION ${BUILD_DIR}/${BUILD_ENGINE_DIR} )
install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/${BUILD_BIN_DIR} )

include_directories( ../lmtloader )
include_directories( ../../../public )
include_directories( ../../../private )
include_directories( ../../ )