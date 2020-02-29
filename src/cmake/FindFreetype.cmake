# 	---------------------------------
#	[in] 	FREETYPE_PATH		- root dir freetype
#	[out] 	FREETYPE_INCLUDE	- dir with includes
#	[out]	FREETYPE_LIB		- lib freetype
#	[out]	FREETYPE_FOUND		- is found freetype
# 	---------------------------------

SET( FREETYPE_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${FREETYPE_PATH}
)

find_path( 		FREETYPE_INCLUDE
				NAMES "ft2build.h" "freetype.h"
				PATH_SUFFIXES include/freetype include
				PATHS ${FREETYPE_SEARCH_PATHS} )
find_library( 	FREETYPE_LIB 
                NAMES freetype
                PATH_SUFFIXES lib
                PATHS ${FREETYPE_SEARCH_PATHS} )

if ( NOT FREETYPE_INCLUDE OR NOT FREETYPE_LIB )
    message( SEND_ERROR "Failed to find Freetype" )
    return()
else()
	set( FREETYPE_FOUND true )
endif()
