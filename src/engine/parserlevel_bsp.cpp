//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "consolesystem.h"
#include "parserlevel_bsp.h"
#include "level.h"

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserLevelBSP::GetVersion() const
{
	return "1.0";
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserLevelBSP::GetName() const
{
	return "Quake 3 BSP";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserLevelBSP::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserLevelBSP::GetFileExtensions() const
{
	static const char*			extensions[ 1 ] = { "bsp" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserLevelBSP::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::ILevel* le::ParserLevelBSP::Read( const char* Path, IFactory* GameFactory )
{
	Level*			level = new Level();
	if ( !level->Load( Path, GameFactory ) )
	{
		delete level;
		return nullptr;
	}

	return level;
}
