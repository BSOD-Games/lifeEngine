//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "studiorender/studiorenderinfo.h"

static const char*			g_typesTechniques[] =
{
	"deffered_shading"
};

static const char*			g_namesTechniques[] =
{
	"Deffered shading"
};

// ------------------------------------------------------------------------------------ //
// Get count types technique
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StudioRenderInfo::GetCountTypesTechnique() const
{
	return sizeof( g_typesTechniques ) / sizeof( const char* );
}

// ------------------------------------------------------------------------------------ //
// Get type technique
// ------------------------------------------------------------------------------------ //
const char* le::StudioRenderInfo::GetTypeTechnique( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( sizeof( g_typesTechniques ) / sizeof( const char* ) < Index );
	return g_typesTechniques[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get name technique
// ------------------------------------------------------------------------------------ //
const char* le::StudioRenderInfo::GetNameTechnique( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( sizeof( g_namesTechniques ) / sizeof( const char* ) < Index );
	return g_namesTechniques[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get types technique
// ------------------------------------------------------------------------------------ //
const char** le::StudioRenderInfo::GetTypesTechnique() const
{
	return g_typesTechniques;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::StudioRenderInfo()
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::~StudioRenderInfo()
{}
