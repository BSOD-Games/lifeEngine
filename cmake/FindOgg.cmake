# 	---------------------------------
#	[in] 	OGG_PATH		- root dir ogg
#	[out] 	OGG_INCLUDE		- dir with includes
#	[out]	OGG_LIB			- lib ogg
#	[out]	OGG_FOUND		- is found ogg
# 	---------------------------------

SET( OGG_SEARCH_PATH
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${OGG_PATH}
)

find_path( 		OGG_INCLUDE
                NAMES "ogg/ogg.h"
                PATH_SUFFIXES include 
				PATHS ${OGG_SEARCH_PATH} )

find_library( 	OGG_LIB
                NAMES ogg
			    PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${OGG_SEARCH_PATH} )

if ( NOT OGG_INCLUDE OR NOT OGG_LIB )
    message( SEND_ERROR "Failed to find Ogg" )
    return()
else()
    set( OGG_FOUND true )
endif()
