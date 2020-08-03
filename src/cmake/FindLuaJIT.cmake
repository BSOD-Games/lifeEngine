# 	---------------------------------
#	[in] 	LUAJIT_PATH		- root dir LuaJIT
#	[out] 	LUAJIT_INCLUDE	- dir with includes
#	[out]	LUAJIT_LIB		- lib LuaJIT
#	[out]	LUA_LIB			- lib Lua
#	[out]	LUAJIT_FOUND	- is found LuaJIT
# 	---------------------------------

SET( LUAJIT_SEARCH_PATH
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${LUAJIT_PATH}
)

find_path( 		LUAJIT_INCLUDE
                NAMES "lua.h"
                PATH_SUFFIXES include 
				PATHS ${LUAJIT_SEARCH_PATH} )
				
find_library( 	LUA_LIB
                NAMES lua51
			    PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LUAJIT_SEARCH_PATH} )

find_library( 	LUAJIT_LIB
                NAMES luajit
			    PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${LUAJIT_SEARCH_PATH} )

if ( NOT LUAJIT_INCLUDE OR NOT LUA_LIB OR NOT LUAJIT_LIB )
    message( SEND_ERROR "Failed to find LuaJIT" )
    return()
else()
    set( LUAJIT_FOUND true )
endif()
