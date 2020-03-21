//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ITEXT_H
#define ITEXT_H

#include "common/types.h"
#include "engine/itransformable.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IFont;
    class IMesh;
    
    //---------------------------------------------------------------------//

    class IText : public ITransformable, public IReferenceObject
    {
    public:
        virtual ~IText() {}

        virtual void                    SetFont( IFont* Font ) = 0;
        virtual void                    SetCharacterSize( UInt32_t CharacterSize ) = 0;
        virtual void                    SetText( const char* Text ) = 0;
        virtual void                    SetColor( const Vector3D_t& Color ) = 0;
        virtual void                    SetLetterSpacingFactor( float LetterSpacingFactor ) = 0;
        virtual void                    SetLineSpacingFactor( float LineSpacingFactor ) = 0;

        virtual IFont*                  GetFont() const = 0;
        virtual UInt32_t                GetCharacterSize() const = 0;
        virtual const char*             GetText() const = 0;
        virtual const Vector3D_t&       GetColor() const = 0;
        virtual float                   GetLetterSpacingFactor() const = 0;
        virtual float                   GetLineSpacingFactor() const = 0;
        virtual IMesh*                  GetMesh() = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define TEXT_INTERFACE_VERSION "LE_Text001"

//---------------------------------------------------------------------//

#endif // !ITEXT_H
