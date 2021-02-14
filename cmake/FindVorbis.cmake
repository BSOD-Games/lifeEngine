# 	---------------------------------
#	[in] 	VORBIS_PATH		- root dir Vorbis
#	[out] 	VORBIS_INCLUDE	- dir with includes
#	[out]	VORBIS_LIB		- lib Vorbis
#	[out]	VORBIS_FOUND	- is found Vorbis
# 	---------------------------------

SET( VORBIS_SEARCH_PATH
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${VORBIS_PATH}
)

find_path( 		VORBIS_INCLUDE
                NAMES "vorbisenc.h"
                PATH_SUFFIXES include src 
				PATHS ${VORBIS_SEARCH_PATH} )

find_library( 	VORBIS_LIB
                NAMES vorbis
			    PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${VORBIS_SEARCH_PATH} )
				
find_library( 	VORBISENC_LIB
                NAMES vorbisenc
			    PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${VORBIS_SEARCH_PATH} )
				
find_library( 	VORBISFILE_LIB
                NAMES vorbisfile
			    PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${VORBIS_SEARCH_PATH} )


if ( NOT VORBIS_INCLUDE OR NOT VORBIS_LIB OR NOT VORBISENC_LIB OR NOT VORBISFILE_LIB )
    message( SEND_ERROR "Failed to find Vorbis" )
    return()
else()
    set( VORBIS_FOUND true )
endif()
