# 	---------------------------------
#	[in] 	NODEEDITOR_PATH		- root dir nodeeditor
#	[out] 	NODEEDITOR_INCLUDE	- dir with includes
#	[out]	NODEEDITOR_LIB		- lib nodeeditor
#	[out]	NODEEDITOR_FOUND	- is found nodeeditor
# 	---------------------------------

SET( NODEEDITOR_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${NODEEDITOR_PATH}
)

find_path( 		NODEEDITOR_INCLUDE
				NAMES "nodes/FlowScene"
				PATH_SUFFIXES include
				PATHS ${NODEEDITOR_SEARCH_PATHS} )
		
find_library( 	NODEEDITOR_LIB 
                NAMES nodes
                PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${NODEEDITOR_SEARCH_PATHS} )

if ( NOT NODEEDITOR_INCLUDE OR NOT NODEEDITOR_LIB )
    message( SEND_ERROR "Failed to find NodeEditor" )
else()
	set( NODEEDITOR_FOUND true )
	add_definitions( -DNODE_EDITOR_SHARED )
endif()
