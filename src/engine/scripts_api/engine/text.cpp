//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/text.h"

namespace scripts_api
{
#include "text.h"
}

#define TEXT( Object )			( static_cast< le::Text* >( Object ) )

// ------------------------------------------------------------------------------------ //
// Create text
// ------------------------------------------------------------------------------------ //
scripts_api::text_t scripts_api::Text_Create()
{
	return new le::Text();
}

// ------------------------------------------------------------------------------------ //
// Delete text
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_Delete( text_t Object )
{
	if ( !Object ) return;

	if ( TEXT( Object )->GetCountReferences() <= 1 )
		TEXT( Object )->Release();
	else
		TEXT( Object )->DecrementReference();

	Object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Set font
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetFont( text_t Object, font_t Font )
{
	if ( !Object || !Font ) return;
	TEXT( Object )->SetFont( ( le::IFont* ) Font );
}

// ------------------------------------------------------------------------------------ //
// Set character size
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetCharacterSize( text_t Object, uint32_t CharacterSize )
{
	if ( !Object ) return;
	TEXT( Object )->SetCharacterSize( CharacterSize );
}

// ------------------------------------------------------------------------------------ //
// Set text
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetText( text_t Object, const char* Text )
{
	if ( !Object || !Text ) return;
	TEXT( Object )->SetText( Text );
}

// ------------------------------------------------------------------------------------ //
// Set color
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetColor( text_t Object, vec3f_t Color )
{
	if ( !Object ) return;
	TEXT( Object )->SetColor( { Color.x, Color.y, Color.z } );
}

// ------------------------------------------------------------------------------------ //
// Set letter spacing factor
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetLetterSpacingFactor( text_t Object, float LetterSpacingFactor )
{
	if ( !Object ) return;
	TEXT( Object )->SetLetterSpacingFactor( LetterSpacingFactor );
}

// ------------------------------------------------------------------------------------ //
// Set line spacing factor
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetLineSpacingFactor( text_t Object, float LineSpacingFactor )
{
	if ( !Object ) return;
	TEXT( Object )->SetLineSpacingFactor( LineSpacingFactor );
}

// ------------------------------------------------------------------------------------ //
// Get font
// ------------------------------------------------------------------------------------ //
scripts_api::font_t scripts_api::Text_GetFont( text_t Object )
{
	if ( !Object ) return nullptr;
	return TEXT( Object )->GetFont();
}

// ------------------------------------------------------------------------------------ //
// Get character size
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::Text_GetCharacterSize( text_t Object )
{
	if ( !Object ) return 0;
	return TEXT( Object )->GetCharacterSize();
}

// ------------------------------------------------------------------------------------ //
// Get text
// ------------------------------------------------------------------------------------ //
const char* scripts_api::Text_GetText( text_t Object )
{
	if ( !Object ) return "";
	return TEXT( Object )->GetText();
}

// ------------------------------------------------------------------------------------ //
// Gett color
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Text_GetColor( text_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&		vec3 = TEXT( Object )->GetColor();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get letter spacing factor
// ------------------------------------------------------------------------------------ //
float scripts_api::Text_GetLetterSpacingFactor( text_t Object )
{
	if ( !Object ) return 0.f;
	return TEXT( Object )->GetLetterSpacingFactor();
}

// ------------------------------------------------------------------------------------ //
// Get line spacing factor
// ------------------------------------------------------------------------------------ //
float scripts_api::Text_GetLineSpacingFactor( text_t Object )
{
	if ( !Object ) return 0.f;
	return TEXT( Object )->GetLineSpacingFactor();
}

// ------------------------------------------------------------------------------------ //
// Move
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_Move( text_t Object, vec3f_t FactorMove )
{
	if ( !Object ) return;
	TEXT( Object )->Move( { FactorMove.x, FactorMove.y, FactorMove.z } );
}

// ------------------------------------------------------------------------------------ //
// Rotate
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_EulerRotate( text_t Object, vec3f_t FactorRotate )
{
	if ( !Object ) return;
	TEXT( Object )->Rotate( { FactorRotate.x, FactorRotate.y, FactorRotate.z } );
}

// ------------------------------------------------------------------------------------ //
// Rotate
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_QuatRotate( text_t Object, quat_t FactorRotate )
{
	if ( !Object ) return;
	TEXT( Object )->Rotate( { FactorRotate.w, FactorRotate.x, FactorRotate.y, FactorRotate.z } );
}

// ------------------------------------------------------------------------------------ //
// Scale
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_Scale( text_t Object, vec3f_t FactorScale )
{
	if ( !Object ) return;
	TEXT( Object )->Scale( { FactorScale.x, FactorScale.y, FactorScale.z } );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetPosition( text_t Object, vec3f_t Position )
{
	if ( !Object ) return;
	TEXT( Object )->SetPosition( { Position.x, Position.y, Position.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetEulerRotation( text_t Object, vec3f_t Rotation )
{
	if ( !Object ) return;
	TEXT( Object )->SetRotation( { Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetQuatRotation( text_t Object, quat_t Rotation )
{
	if ( !Object ) return;
	TEXT( Object )->Rotate( { Rotation.w, Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set scale
// ------------------------------------------------------------------------------------ //
void scripts_api::Text_SetScale( text_t Object, vec3f_t Scale )
{
	if ( !Object ) return;
	TEXT( Object )->SetScale( { Scale.x, Scale.y, Scale.z } );
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Text_GetPosition( text_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&		vec3 = TEXT( Object )->GetPosition();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
scripts_api::quat_t scripts_api::Text_GetRotation( text_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f, 0.f };

	const le::Quaternion_t&		quat = TEXT( Object )->GetRotation();
	return { quat.x, quat.y, quat.z, quat.w };
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Text_GetScale( text_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&		vec3 = TEXT( Object )->GetScale();
	return { vec3.x, vec3.y, vec3.z };
}
