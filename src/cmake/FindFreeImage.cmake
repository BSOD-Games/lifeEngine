# 	---------------------------------
#	[in] 	FREEIMAGE_PATH			- root dir FreeImage
#	[out] 	FREEIMAGE_INCLUDE		- dir with includes
#	[out]	FREEIMAGE_LIB			- lib FreeImage
#	[out]	FREEIMAGE_FOUND			- is found FreeImage
# 	---------------------------------

SET( FREEIMAGE_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
        /lib
	${FREEIMAGE_PATH}
)

find_path( 		FREEIMAGE_INCLUDE
                                NAMES "FreeImage.h"
				PATH_SUFFIXES include
				PATHS ${FREEIMAGE_SEARCH_PATHS} )		
find_library( 	FREEIMAGE_LIB 
                NAMES FreeImage freeimage
                PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${FREEIMAGE_SEARCH_PATHS} )
		
if ( NOT FREEIMAGE_INCLUDE OR NOT FREEIMAGE_LIB )
    message( SEND_ERROR "Failed to find FreeImage" )
    return()
else()
	set( FREEIMAGE_FOUND true )
endif()
