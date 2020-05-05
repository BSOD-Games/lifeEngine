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

// ------------------------------------------------------------------------------------ //
// Get count techniques
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StudioRenderInfo::GetCountTechniques() const
{
	return techniques.size();
}

// ------------------------------------------------------------------------------------ //
// Get count passes
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StudioRenderInfo::GetCountPasses( UInt32_t IDTechnique ) const
{
	if ( IDTechnique >= techniques.size() )	
		return 0;

	return passes[ IDTechnique ].size();
}

// ------------------------------------------------------------------------------------ //
// Get technique
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::TechniqueInfo le::StudioRenderInfo::GetTechnique( UInt32_t IDTechnique ) const
{
	if ( IDTechnique >= techniques.size() ) 
		return TechniqueInfo();

	return techniques[ IDTechnique ];
}

// ------------------------------------------------------------------------------------ //
// Get pass
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::PassInfo le::StudioRenderInfo::GetPass( UInt32_t IDTechnique, UInt32_t IDPass ) const
{
	if ( IDTechnique >= techniques.size() || IDPass >= passes.size() )
		return PassInfo();

	return passes[ IDTechnique ][ IDPass ];
}

// ------------------------------------------------------------------------------------ //
// Get techniques
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::TechniqueInfo* le::StudioRenderInfo::GetTechniques() const
{
	return ( TechniqueInfo* ) techniques.data();
}

// ------------------------------------------------------------------------------------ //
// Get passes
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::PassInfo* le::StudioRenderInfo::GetPasses( UInt32_t IDTechnique ) const
{
	if ( IDTechnique >= techniques.size() )
		return nullptr;

	return ( PassInfo* ) passes[ IDTechnique ].data();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::StudioRenderInfo() :
	techniques( 
	{
		// index: 0
		{ "deffered_shading", "Deffered shading" }
	} ),
	passes( 
	{
		// index: 0
		// deffered_shading
		{
			{ "gbuffer", "GBuffer" }
		},
	} )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::StudioRenderInfo::~StudioRenderInfo()
{}
