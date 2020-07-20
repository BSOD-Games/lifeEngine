# 	---------------------------------
#	[in] 	STEAMAUDIO_PATH			- root dir SteamAudio
#	[out] 	STEAMAUDIO_INCLUDE		- dir with includes
#	[out]	STEAMAUDIO_LIB			- lib SteamAudio
#	[out]	STEAMAUDIO_FOUND		- is found SteamAudio
# 	---------------------------------

SET( STEAMAUDIO_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${STEAMAUDIO_PATH}
)

find_path( 		STEAMAUDIO_INCLUDE
                NAMES "phonon.h" "phonon_version.h"
				PATH_SUFFIXES include
				PATHS ${STEAMAUDIO_SEARCH_PATHS} )
find_library( 	STEAMAUDIO_LIB 
                NAMES phonon
                PATH_SUFFIXES lib/Windows/x86 lib/Linux/x86 lib/OSX
                PATHS ${STEAMAUDIO_SEARCH_PATHS} )

if ( NOT STEAMAUDIO_INCLUDE OR NOT STEAMAUDIO_LIB )
    message( SEND_ERROR "Failed to find SteamAudio" )
    return()
else()
	set( STEAMAUDIO_FOUND true )
endif()
