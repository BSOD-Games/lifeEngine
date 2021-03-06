# 	---------------------------------
#	[in] 	LIFEENGINE_SDK_PATH			- root dir
#	[out] 	LIFEENGINE_SDK_INCLUDE		- dir with includes
#	[out]	LIFEENGINE_SDK_LAUNCHER 	- path to launcher engine
#	[out]	LIFEENGINE_SDK_LAUNCHER_DIR - dir launcher engine
#	[out]	LIFEENGINE_SDK_FOUND		- is found lifeEngine SDK
# 	---------------------------------

find_path( 		LIFEENGINE_SDK_INCLUDE
				NAMES "engine/lifeEngine.h"
				PATH_SUFFIXES src/public
				PATHS ${LIFEENGINE_SDK_PATH} )		
		
find_file(		LIFEENGINE_SDK_LAUNCHER
				NAMES "launcher.exe"
				PATH_SUFFIXES build
				PATHS ${LIFEENGINE_SDK_PATH} )
		
find_path(		LIFEENGINE_SDK_LAUNCHER_DIR
				NAMES "launcher.exe"
				PATH_SUFFIXES build
				PATHS ${LIFEENGINE_SDK_PATH} )
	
if ( NOT LIFEENGINE_SDK_INCLUDE OR NOT LIFEENGINE_SDK_LAUNCHER )
    message( SEND_ERROR "Failed to find lifeEngine SDK" )
    return()
else()
	set( LIFEENGINE_SDK_FOUND true )
endif()