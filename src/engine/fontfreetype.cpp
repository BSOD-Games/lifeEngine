//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <exception>
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "engine/lifeengine.h"
#include "engine/ifactory.h"
#include "studiorender/istudiorender.h"
#include "studiorender/itexture.h"
#include "studiorender/studiorendersampler.h"
#include "studiorender/studiorenderdeviceconfigurations.h"

#include "global.h"
#include "engine/consolesystem.h"
#include "engine/fontfreetype.h"

//---------------------------------------------------------------------//

void*               le::FontFreeType::ftLibrary = nullptr;

//---------------------------------------------------------------------//

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::FontFreeType::FontFreeType() :
    ftFace( nullptr ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::FontFreeType::~FontFreeType()
{
    if ( IsLoaded() )       Unload();
}

// ------------------------------------------------------------------------------------ //
// Initialize freeType library
// ------------------------------------------------------------------------------------ //
bool le::FontFreeType::InitializeFreeType()
{
    if ( ftLibrary ) return true;

    FT_Library          ftLibrary;
    if ( FT_Init_FreeType( &ftLibrary ) )
    {
        g_consoleSystem->PrintError( "Could not init FreeType library" );
        return false;
    }

    g_consoleSystem->PrintInfo( "FreeType library intialized" );
    FontFreeType::ftLibrary = ftLibrary;
    return true;
}

// ------------------------------------------------------------------------------------ //
// Uninitialize freeType library
// ------------------------------------------------------------------------------------ //
void le::FontFreeType::UninitializeFreeType()
{
    if ( !ftLibrary )    return;

    FT_Done_FreeType( ( FT_Library ) ftLibrary );
    ftLibrary = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Get glyph
// ------------------------------------------------------------------------------------ //
const le::FontFreeType::Glyph& le::FontFreeType::GetGlyph( UInt32_t CodePoint, UInt32_t CharacterSize )
{
	// Get the page that matches given size
	std::unordered_map< UInt32_t, Glyph >&				glyphs = pages[ CharacterSize ].glyphs;

	// Get the glyph index based on the Unicode character
	UInt32_t		key = FT_Get_Char_Index( static_cast< FT_Face >( ftFace ), CodePoint );

	// Looking For A Glyph In The Cache
	auto		itGlyph = glyphs.find( key );
	if ( itGlyph != glyphs.end() )
		return itGlyph->second;

	// If not found, then download from FreeType
	Glyph		newGlyph = LoadGlyph( CodePoint, CharacterSize );
	return glyphs.insert( std::make_pair( key, newGlyph ) ).first->second;
}

// ------------------------------------------------------------------------------------ //
//  Get line spacing
// ------------------------------------------------------------------------------------ //
float le::FontFreeType::GetLineSpacing( UInt32_t CharacterSize ) const
{
	FT_Face			face = static_cast< FT_Face >( ftFace );

	if ( face && SetCurrentSize( CharacterSize ) )
		return static_cast< float >( face->size->metrics.height ) / static_cast< float >( 1 << 6 );

	return 0.f;
}

// ------------------------------------------------------------------------------------ //
//  Get texture font
// ------------------------------------------------------------------------------------ //
le::ITexture* le::FontFreeType::GetTexture( UInt32_t CharacterSize ) const
{
    auto        itFind = pages.find( CharacterSize );
    if ( itFind == pages.end() )
        return nullptr;

    return itFind->second.texture;
}

// ------------------------------------------------------------------------------------ //
//  Get family name
// ------------------------------------------------------------------------------------ //
const char* le::FontFreeType::GetFamilyName() const
{
    return familyName.c_str();
}

// ------------------------------------------------------------------------------------ //
//  Load font
// ------------------------------------------------------------------------------------ //
bool le::FontFreeType::Load( const char* Path )
{
    try
    {
        // If not initialized freeType library - return error
        if ( !ftLibrary )       throw std::runtime_error( "FreeType library not initialized" );

        // If the font was previously loaded - delete 
        if ( IsLoaded() )       Unload();

        // Loading font
        FT_Face         ftFace;
        if ( FT_New_Face( ( FT_Library ) ftLibrary, Path, 0, &ftFace ) )    
            throw std::runtime_error( "Font not loaded" );

        // Select Unicode character map
        if ( FT_Select_Charmap( ftFace, FT_ENCODING_UNICODE ) )
        {
            if ( ftFace )       FT_Done_Face( ftFace );
            throw std::runtime_error( "Failed to set the Unicode charcter set" );
        }

        this->ftFace = ftFace;
        familyName = ftFace->family_name ? ftFace->family_name : "";
    }
    catch( const std::exception& Exception )
    {
        g_consoleSystem->PrintError( "Fail loading font [%s]: ", Path, Exception.what() );        
        return false;
    }
    
    return true;
}

// ------------------------------------------------------------------------------------ //
//  Unload font
// ------------------------------------------------------------------------------------ //
void le::FontFreeType::Unload()
{
    if ( !IsLoaded() ) return;
    LIFEENGINE_ASSERT( g_studioRender && g_studioRender->GetFactory() );

    // Delete loaded font
    FT_Done_Face( ( FT_Face ) ftFace );

    // Destroy all created textures
    for ( auto it = pages.begin(), itEnd = pages.end(); it != itEnd; ++it )
        if ( it->second.texture )
        {
            if ( it->second.texture->GetCountReferences() <= 1 )
                it->second.texture->Release();
            else
                it->second.texture->DecrementReference();
        }

    ftFace = nullptr;
    familyName = "";
    pages.clear();
}

// ------------------------------------------------------------------------------------ //
//  Load glyph from font
// ------------------------------------------------------------------------------------ //
le::FontFreeType::Glyph le::FontFreeType::LoadGlyph( UInt32_t CodePoint, UInt32_t CharacterSize )
{
    LIFEENGINE_ASSERT( g_studioRender && g_studioRender->GetFactory() );

    Glyph           glyph;
    SetCurrentSize( CharacterSize );

    // Loading glyph char
    if ( FT_Load_Char( ( FT_Face ) ftFace, CodePoint, FT_LOAD_RENDER ) )
    {
        g_consoleSystem->PrintError( "Failed to load glyph for char with code [%i]", CodePoint );
        return glyph;
    }

    // Remember the offset to the next glyph
    glyph.advance = static_cast< float >( static_cast< FT_Face >( ftFace )->glyph->advance.x >> 6 );

    // Get size glyph
    int             width = static_cast< FT_Face >( ftFace )->glyph->bitmap.width;
    int             height = static_cast< FT_Face >( ftFace )->glyph->bitmap.rows;

    if ( width <= 0 || height <= 0 )        return glyph;

    // Leave a little bit of clipping around the characters so that the filtering doesn't
    // clogged them with neighboring pixels
    const UInt32_t          padding = 1;
    width += 2 * padding;
    height += 2 * padding;

    // We get the font page for the desired character size
    Page&       page = pages[ CharacterSize ];

    // If the texture on the page is empty - create a texture with a minimum size
    if ( !page.texture )
    {
        page.texture = ( ITexture* ) g_studioRender->GetFactory()->Create( TEXTURE_INTERFACE_VERSION );
        LIFEENGINE_ASSERT( page.texture );

        le::StudioRenderSampler			sampler;
	    sampler.minFilter = sampler.magFilter = le::SF_LINEAR;
        sampler.addressU = sampler.addressV = le::SAM_CLAMP;

        page.texture->Initialize( le::TT_2D, le::IF_R_8UNORM, 1, 1 );    
        page.texture->Bind();
        page.texture->Append( nullptr );
        page.texture->SetSampler( sampler );
        page.texture->Unbind();
        page.texture->IncrementReference();
    }

    // Find the position where to place the new glyph
  	glyph.textureRect = FindGlyphRect( page, width, height );

	glyph.textureRect.left		+= padding;
	glyph.textureRect.top		+= padding;
	glyph.textureRect.width		-= 2 * padding;
	glyph.textureRect.height	-= 2 * padding;   

    // Calculate the bounding box of the glyph  
    glyph.bounds.left = static_cast< float >( static_cast< FT_Face >( ftFace )->glyph->bitmap_left );
	glyph.bounds.top = static_cast< float >( static_cast< FT_Face >( ftFace )->glyph->bitmap_top );
	glyph.bounds.width = static_cast< float >( static_cast< FT_Face >( ftFace )->glyph->bitmap.width );
	glyph.bounds.height = static_cast< float >( static_cast< FT_Face >( ftFace )->glyph->bitmap.rows ); 

    // Write the glyph to the texture atlas
    page.texture->Bind();
    page.texture->Update( 
        glyph.textureRect.left,
		glyph.textureRect.top,
		width - 2 * padding,
		height - 2 * padding,
        &static_cast< FT_Face >( ftFace )->glyph->bitmap.buffer[ 0 ] 
        );
    page.texture->Unbind();

    return glyph;

    // *****************************
    // Here lies the self-esteem of Egor Pogulyaka (zombiHello), who was trying to write a render of the text
    // and was sad because of a pile of code and two hours of coding without compilation :)
    //
    // R.I.P...
    // ********************************
}

// ------------------------------------------------------------------------------------ //
//  Find glyph rect
// ------------------------------------------------------------------------------------ //
le::IntRect_t le::FontFreeType::FindGlyphRect( Page& Page, UInt32_t Width, UInt32_t Height )
{
    LIFEENGINE_ASSERT( g_studioRender && Page.texture );

    // Look for a line that fits well in the glyph
    Row*		row = nullptr;
	float		bestRation = 0;

    for ( auto it = Page.rows.begin(), itEnd = Page.rows.end(); it != itEnd && !row; ++it )
    {
        float		ratio = static_cast< float >( Height ) / it->height;

        // Ignore rows that are too small or too high
        if ( ( ratio < 0.7f ) || ( ratio > 1.f ) )
            continue;
        
        // Check if enough horizontal space is left in the line
        if ( Width > Page.texture->GetWidth() - it->width )
			continue;

        // Make sure this new line is the best found so far
		if ( ratio < bestRation )
			continue;

        // The current line has passed all the tests: we can select it
        row = &*it;
		bestRation = ratio;
    }

    // If we did not find the corresponding line, create a new texture (10% more)
    if ( !row )
    {
        LIFEENGINE_ASSERT( Page.texture );
        const StudioRenderDeviceConfigurations&             deviceConfigurations = g_studioRender->GetDeviceConfigurations();

        int			rowHeight = Height + Height / 10;
        while ( Page.nextRow + rowHeight >= Page.texture->GetHeight() || Width >= Page.texture->GetWidth() )
        {
            // Not enough space: resize the texture if possible
            Vector2DInt_t			textureSize( Page.texture->GetWidth(), Page.texture->GetHeight() );

            if ( ( UInt32_t ) textureSize.x * 2 <= deviceConfigurations.maxTextureSize && ( UInt32_t ) textureSize.y * 2 <= deviceConfigurations.maxTextureSize )
            {    
                Page.texture->Bind();
                Page.texture->Resize( textureSize.x * 2, textureSize.y * 2, true );
                Page.texture->Unbind();
            }
            else
            {
                g_consoleSystem->PrintError( "Failed to add a new character to the font: the maximum texture size has been reached" );
                return IntRect_t( 0, 0, 2, 2 );
            }
        }

        // Now we can create a new line
	    Page.rows.push_back( Row( Page.nextRow, rowHeight ) );
	    Page.nextRow += rowHeight;
	    row = &Page.rows.back();
    }

    // Calculate the glyph rectangle in the selected line
    IntRect_t		rect( row->width, row->top, Width, Height );

    // Update string length
    row->width += Width;

    return rect;
}

// ------------------------------------------------------------------------------------ //
//  Set current size font
// ------------------------------------------------------------------------------------ //
bool le::FontFreeType::SetCurrentSize( UInt32_t CharacterSize ) const
{
    FT_UShort			currentSize = static_cast< FT_Face >( ftFace )->size->metrics.x_ppem;

	if ( CharacterSize != currentSize )
	{
		FT_Error		result = FT_Set_Pixel_Sizes( static_cast< FT_Face >( ftFace ), 0, CharacterSize );

		if ( result == FT_Err_Invalid_Pixel_Size )
		{
            // In the case of bitmap fonts, resizing may be
            // crash if requested size is not available
			if ( !FT_IS_SCALABLE( static_cast< FT_Face >( ftFace ) ) )
			{
                g_consoleSystem->PrintError( "Failed to set bitmap font size to %i", CharacterSize );
                g_consoleSystem->PrintError( "Available sizes are: " );

				for ( int index = 0; index < static_cast< FT_Face >( ftFace )->num_fixed_sizes; ++index )
                     g_consoleSystem->PrintError( "%i", ( static_cast< FT_Face >( ftFace )->available_sizes[ index ].y_ppem + 32 ) >> 6 );
			}
			else
                g_consoleSystem->PrintError( "Failed to set font size to %i", CharacterSize );

			return result == FT_Err_Ok;
		}
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
//  Constructor structure Row
// ------------------------------------------------------------------------------------ //
le::FontFreeType::Row::Row( UInt32_t RowTop, UInt32_t RowHeight ) :
    width( 0 ),
	top( RowTop ),
	height( RowHeight )
{}

// ------------------------------------------------------------------------------------ //
//  Constructor structure Page
// ------------------------------------------------------------------------------------ //
le::FontFreeType::Page::Page() :
    texture( nullptr ),
    nextRow( 0 )
{}

// ------------------------------------------------------------------------------------ //
//  Increment reference
// ------------------------------------------------------------------------------------ //
void le::FontFreeType::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
//  Decrement referencee
// ------------------------------------------------------------------------------------ //
void le::FontFreeType::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
//  Delete
// ------------------------------------------------------------------------------------ //
void le::FontFreeType::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
//  Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::FontFreeType::GetCountReferences() const
{
    return countReferences;
}
