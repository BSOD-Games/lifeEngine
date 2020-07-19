//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <sstream>

#include "engine/consolesystem.h"

#include "global.h"
#include "parserfont_freetype.h"
#include "fontfreetype.h"

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::ParserFontFreeType::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserFontFreeType::GetVersion() const
{
	static std::string			version;
	if ( version.empty() )
	{
		UInt32_t		major = 0;
		UInt32_t		minor = 0;
		UInt32_t		path = 0;

		FontFreeType::GetFreeTypeVersion( major, minor, path );
		std::stringstream		strStream;

		strStream << major << "." << minor << "." << path;
		version = strStream.str();
	}
	
	return version.c_str();
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserFontFreeType::GetName() const
{
	return "FreeType Font";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserFontFreeType::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserFontFreeType::GetFileExtensions() const
{
	static const char*			extensions[ 1 ] = { "ttf" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserFontFreeType::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::IFont* le::ParserFontFreeType::Read( const char* Path )
{
	FontFreeType*		fontFreeType = new FontFreeType();
	if ( !fontFreeType->Load( Path ) )
	{
		delete fontFreeType;
		return nullptr;
	}

	return fontFreeType;
}

// ------------------------------------------------------------------------------------ //
// Initialize FreeType
// ------------------------------------------------------------------------------------ //
bool le::ParserFontFreeType::InitializeFreeType()
{
	return FontFreeType::InitializeFreeType();
}

// ------------------------------------------------------------------------------------ //
// Uninitialize FreeType
// ------------------------------------------------------------------------------------ //
void le::ParserFontFreeType::UninitializeFreeType()
{
	FontFreeType::UninitializeFreeType();
}
