cmake_minimum_required( VERSION 2.6 )
set( MODULE_NAME lifeEditor )

#
#   --- Set values ---
#
# ================================================

# Root files
file( GLOB ROOT_FILES "*.h" "*.cpp" )
source_group( "Source Files" FILES ${ROOT_FILES} )

# Window files
file( GLOB WINDOW_FILES "Window/*.h" "Window/*.cpp" )
source_group( "Source Files\\Window Files" FILES ${WINDOW_FILES} )

# Forms files
file( GLOB FORMS_FILES "Window/*.ui" )
source_group( "Source Files\\Window Files\\UI" FILES ${FORMS_FILES} )

#file( GLOB_RECURSE RESOURCES "*.qrc" )

set( QT5_DIR "QT5_DIR" CACHE PATH "Qt5 dir (ex. D:/Soft/Qt/5.12.2/mingw73_64)" )
set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT5_DIR} )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )

# ================================================
#
#   --- Find extlibs ---
#
find_package( Qt5 COMPONENTS Widgets REQUIRED )
#
#   --- Project settings ---
#
# ================================================

qt5_wrap_ui( UI_HEADERS ${FORMS_FILES} )
qt5_add_resources ( RCC_SOURCES ${RESOURCES} )
set( SOURCE_FILES ${UI_HEADERS} ${RCC_SOURCES} ${ROOT_FILES} ${WINDOW_FILES} ${FORMS_FILES} )

add_executable( ${MODULE_NAME} WIN32 ${SOURCE_FILES} )
set_target_properties( ${MODULE_NAME} PROPERTIES DEBUG_POSTFIX "-Debug" )
set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER Editor )
set_target_properties( ${MODULE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${BUILD_DIR}/Binaries/${ARCH_TYPE}
												 VS_DEBUGGER_ENVIRONMENT ${BUILD_DIR}/Binaries/${ARCH_TYPE} )

target_link_libraries( ${MODULE_NAME} Engine )
target_link_libraries( ${MODULE_NAME} Qt5::Widgets Qt5::Core ${CMAKE_DL_LIBS} )
include_directories( ../Runtime/Core/Public )
include_directories( ../Runtime/Engine/Public )

# ================================================
#
#   --- Install settings ---
#

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} )

# For Window copy Qt libs to BUILD_DIR
if( ${PLATFORM_TYPE} MATCHES "Windows" )
	# Install Qt plugins
	install( DIRECTORY ${QT5_DIR}/plugins/platforms DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} )
	install( DIRECTORY ${QT5_DIR}/plugins/styles DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} )
	
	# Install Qt DLL's for Debug
	install( FILES ${QT5_DIR}/bin/Qt5Cored.dll DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} CONFIGURATIONS Debug )
	install( FILES ${QT5_DIR}/bin/Qt5Guid.dll DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} CONFIGURATIONS Debug )
	install( FILES ${QT5_DIR}/bin/Qt5Widgetsd.dll DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} CONFIGURATIONS Debug )

	# Install Qt DLL's for Release
	install( FILES ${QT5_DIR}/bin/Qt5Core.dll DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} CONFIGURATIONS Release )
	install( FILES ${QT5_DIR}/bin/Qt5Gui.dll DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} CONFIGURATIONS Release )
	install( FILES ${QT5_DIR}/bin/Qt5Widgets.dll DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} CONFIGURATIONS Release )
endif()