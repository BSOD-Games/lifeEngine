# 	---------------------------------
#	[in] 	OPENAL_PATH		- root dir OpenAL
#	[out] 	OPENAL_INCLUDE		- dir with includes
#	[out]	OPENAL_LIB		- lib OpenAL
#	[out]	OPENAL_FOUND		- is found OpenAL
# 	---------------------------------

SET( OPENAL_SEARCH_PATH
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${OPENAL_PATH}
)

find_path( 		OPENAL_INCLUDE
				NAMES "al.h"
				PATH_SUFFIXES include src include
				PATHS ${OPENAL_SEARCH_PATH} )

find_library( 	OPENAL_LIB
                NAMES openal32
				PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${OPENAL_SEARCH_PATH} )


if ( NOT OPENAL_INCLUDE OR NOT OPENAL_LIB )
    message( SEND_ERROR "Failed to find OpenAL" )
    return()
else()
    set( OPENAL_FOUND true )
endif()
