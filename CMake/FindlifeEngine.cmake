# 	---------------------------------
#	[in] 	LIFEENGINE_PATH					- root dir lifeEngine
#	[in]	PLATFORM_TYPE					- platform type
#	[in]	ARCH_TYPE						- architecture type
#	[out] 	LIFEENGINE_INCLUDE				- dir with includes
#	[out]	LIFEENGINE_ENGINE_LIB			- lib Engine module
#	[out]	LIFEENGINE_ENGINE_DEBUG_LIB		- lib Engine module for debug
#	[out]	LIFEENGINE_OPENGL4RHI_LIB		- lib OpenGL4RHI module
#	[out]	LIFEENGINE_OPENGL4RHI_DEBUG_LIB	- lib OpenGL4RHI module for debug
#	[out]	LIFEENGINE_LAUNCHER_LIB			- lib Launcher module
#	[out]	LIFEENGINE_LAUNCHER_DEBUG_LIB	- lib Launcher module for debug
#	[out]	LIFEENGINE_EXTLIBS_INCLUDE		- extlibs include
#	[out]	LIFEENGINE_EXTLIBS_LIB			- extlibs
#	[out]	LIFEENGINE_EXTLIBS_DEBUG_LIB	- extlibs for debug
#	[out]	LIFEENGINE_CONTENT				- dir with engine content
#	[out]	LIFEENGINE_FOUND				- is found lifeEngine
# 	---------------------------------

SET( LIFEENGINE_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${LIFEENGINE_PATH}
)

find_path( 		LIFEENGINE_INCLUDE_DIR
				NAMES "Engine/Engine.h"
				PATH_SUFFIXES Include include
				PATHS ${LIFEENGINE_SEARCH_PATHS} )	

find_path( 		LIFEENGINE_CONTENT
				NAMES "Engine"
				PATH_SUFFIXES Content
				PATHS ${LIFEENGINE_SEARCH_PATHS} )		
	
find_library( 	LIFEENGINE_ENGINE_LIB 
                NAMES Engine
                PATH_SUFFIXES Lib/${ARCH_TYPE} lib/${ARCH_TYPE} lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LIFEENGINE_SEARCH_PATHS} )			

find_library( 	LIFEENGINE_ENGINE_DEBUG_LIB 
                NAMES Engine-Debug
                PATH_SUFFIXES Lib/${ARCH_TYPE} lib/${ARCH_TYPE} lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LIFEENGINE_SEARCH_PATHS} )

find_library( 	LIFEENGINE_OPENGL4RHI_LIB 
                NAMES OpenGL4RHI
                PATH_SUFFIXES Lib/${ARCH_TYPE} lib/${ARCH_TYPE} lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LIFEENGINE_SEARCH_PATHS} )			

find_library( 	LIFEENGINE_OPENGL4RHI_DEBUG_LIB 
                NAMES OpenGL4RHI-Debug
                PATH_SUFFIXES Lib/${ARCH_TYPE} lib/${ARCH_TYPE} lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LIFEENGINE_SEARCH_PATHS} )

find_library( 	LIFEENGINE_LAUNCHER_LIB 
                NAMES Launcher
                PATH_SUFFIXES Lib/${ARCH_TYPE} lib/${ARCH_TYPE} lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LIFEENGINE_SEARCH_PATHS} )			

find_library( 	LIFEENGINE_LAUNCHER_DEBUG_LIB 
                NAMES Launcher-Debug
                PATH_SUFFIXES Lib/${ARCH_TYPE} lib/${ARCH_TYPE} lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LIFEENGINE_SEARCH_PATHS} )

if ( 	LIFEENGINE_INCLUDE_DIR AND LIFEENGINE_CONTENT AND LIFEENGINE_ENGINE_LIB AND 
		LIFEENGINE_ENGINE_DEBUG_LIB AND LIFEENGINE_OPENGL4RHI_LIB AND LIFEENGINE_OPENGL4RHI_DEBUG_LIB AND 
		LIFEENGINE_LAUNCHER_LIB AND LIFEENGINE_LAUNCHER_DEBUG_LIB )
	
	set( LIFEENGINE_FOUND true )
	
	# Set includ paths
	set( LIFEENGINE_INCLUDE ${LIFEENGINE_INCLUDE_DIR}/Engine ${LIFEENGINE_INCLUDE_DIR}/Launcher )
	
	# ================================================
	#
	# -- Founding extlibs for lifeEngine ---
	#
	
	
	set( LIFEENGINE_EXTLIBS_DIR ${LIFEENGINE_PATH}/Extlibs )
	set( TEMP_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} )
	set( CMAKE_MODULE_PATH ${LIFEENGINE_EXTLIBS_DIR}/CMake )
	
	# Set path to extlibs for find
	set( GLEW_PATH ${LIFEENGINE_EXTLIBS_DIR}/GLEW)
	set( SDL2_PATH ${LIFEENGINE_EXTLIBS_DIR}/SDL2 )
	set( TMXLITE_PATH ${LIFEENGINE_EXTLIBS_DIR}/TMXLite )
	
	# GLEW
	find_package( GLEW REQUIRED )
	if( NOT GLEW_FOUND )
		message( SEND_ERROR "Failed to find GLEW" )
		return()
	else()
		add_definitions( -DGLEW_STATIC )
		set( LIFEENGINE_EXTLIBS_INCLUDE ${LIFEENGINE_EXTLIBS_INCLUDE} ${GLEW_INCLUDE} )
		set( LIFEENGINE_EXTLIBS_LIB ${LIFEENGINE_EXTLIBS_LIB} ${GLEW_LIB} )
		set( LIFEENGINE_EXTLIBS_DEBUG_LIB ${LIFEENGINE_EXTLIBS_DEBUG_LIB} ${GLEW_LIB} )
	endif()
			
	# OpenGL
	find_package( OpenGL REQUIRED )
	if( NOT OPENGL_FOUND )
		message( SEND_ERROR "Failed to find OpenGL" )
		return()
	else()
		set( LIFEENGINE_EXTLIBS_INCLUDE ${LIFEENGINE_EXTLIBS_INCLUDE} ${OPENGL_INCLUDE_DIR} )
		set( LIFEENGINE_EXTLIBS_LIB ${LIFEENGINE_EXTLIBS_LIB} ${OPENGL_LIBRARIES} )
		set( LIFEENGINE_EXTLIBS_DEBUG_LIB ${LIFEENGINE_EXTLIBS_DEBUG_LIB} ${OPENGL_LIBRARIES} )
	endif()
	
	# SDL2
	find_package( SDL2 REQUIRED )
	if( NOT SDL2_FOUND )
		message( SEND_ERROR "Failed to find SDL2" )
		return()
	else()
		set( LIFEENGINE_EXTLIBS_INCLUDE ${LIFEENGINE_EXTLIBS_INCLUDE} ${SDL2_INCLUDE} )
		set( LIFEENGINE_EXTLIBS_LIB ${LIFEENGINE_EXTLIBS_LIB} ${SDL2_LIB} )
		set( LIFEENGINE_EXTLIBS_DEBUG_LIB ${LIFEENGINE_EXTLIBS_DEBUG_LIB} ${SDL2_DEBUG_LIB} )
	endif()

	# TMXLite
	find_package( TMXLite REQUIRED )
	if( NOT TMXLITE_FOUND )
		message( SEND_ERROR "Failed to find TMXLite" )
		return()
	else()
		set( LIFEENGINE_EXTLIBS_INCLUDE ${LIFEENGINE_EXTLIBS_INCLUDE} ${TMXLITE_INCLUDE} )
		set( LIFEENGINE_EXTLIBS_LIB ${LIFEENGINE_EXTLIBS_LIB} ${TMXLITE_LIB} )
		set( LIFEENGINE_EXTLIBS_DEBUG_LIB ${LIFEENGINE_EXTLIBS_DEBUG_LIB} ${TMXLITE_DEBUG_LIB} )
	endif()
	
	# Other default extlibs
	if ( ${PLATFORM_TYPE} MATCHES "Windows" )		
		set( LIFEENGINE_EXTLIBS_LIB ${LIFEENGINE_EXTLIBS_LIB} winmm version imm32 setupapi )
		set( LIFEENGINE_EXTLIBS_DEBUG_LIB ${LIFEENGINE_EXTLIBS_DEBUG_LIB} winmm version imm32 setupapi )
	endif()
	
	set( CMAKE_MODULE_PATH ${TEMP_CMAKE_MODULE_PATH} )
endif()
