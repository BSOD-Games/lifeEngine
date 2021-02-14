//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IFONT_H
#define IFONT_H

#include "common/rect.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class ITexture;

    //---------------------------------------------------------------------//

    class IFont : public IReferenceObject
    {
    public:

        //---------------------------------------------------------------------//

        struct Glyph
        {
            float           advance;
            FloatRect_t     bounds;
            IntRect_t       textureRect;
        };

        //---------------------------------------------------------------------//

        virtual ~IFont() {}

        virtual const Glyph&            GetGlyph( UInt32_t CodePoint, UInt32_t CharacterSize ) = 0;
        virtual float                   GetLineSpacing( UInt32_t CharacterSize ) const = 0;
        virtual ITexture*               GetTexture( UInt32_t CharacterSize ) const = 0;
        virtual const char*             GetFamilyName() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IFONT_H
