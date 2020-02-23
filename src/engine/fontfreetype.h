//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef FONT_H
#define FONT_H

#include <vector>
#include <unordered_map>
#include <string>

#include "engine/ifont.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class FontFreeType : public IFont
    {
    public:
        // IFont
        virtual const Glyph&            GetGlyph( UInt32_t CodePoint, UInt32_t CharacterSize );
        virtual float                   GetLineSpacing( UInt32_t CharacterSize ) const;
        virtual ITexture*               GetTexture( UInt32_t CharacterSize ) const;
        virtual const char*             GetFamilyName() const;

        // FontFreeType
        FontFreeType();
        ~FontFreeType();

        static bool                     InitializeFreeType();
        static void                     UninitializeFreeType();
        bool                            Load( const char* Path );
        void                            Unload();

        inline bool                     IsLoaded() const
        {
            return ftFace;
        }

    private:

        //---------------------------------------------------------------------//

        struct Row
        {
            Row( UInt32_t RowTop, UInt32_t RowHeight );

            UInt32_t            width;
            UInt32_t            top;
            UInt32_t            height;
        };

        //---------------------------------------------------------------------//

        struct Page
        {
            Page();

            UInt32_t                                    nextRow;
            ITexture*                                   texture;
            std::vector< Row >                          rows;
            std::unordered_map< UInt32_t, Glyph >       glyphs;
        };
        

        //---------------------------------------------------------------------//

        Glyph                       LoadGlyph( UInt32_t CodePoint, UInt32_t CharacterSize );
        IntRect_t                   FindGlyphRect( Page& Page, UInt32_t Width, UInt32_t Height );

        bool                        SetCurrentSize( UInt32_t CharacterSize ) const;

        static void*                                    ftLibrary;
        void*                                           ftFace;
        std::string                                     familyName;
        std::unordered_map< UInt32_t, Page >            pages;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !FONT_H