//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_TEXT_H
#define API_TEXT_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

/* Create text */
extern text_t					Text_Create();

/* Delete text */
extern void						Text_Delete( text_t Object );

/* Set font */
extern void						Text_SetFont( text_t Object, font_t Font );

/* Set character size */
extern void						Text_SetCharacterSize( text_t Object, uint32_t CharacterSize );

/* Set text */
extern void						Text_SetText( text_t Object, const char* Text );

/* Set material */
extern void						Text_SetMaterial( text_t Object, material_t Material );

/* Set letter spacing factor */
extern void						Text_SetLetterSpacingFactor( text_t Object, float LetterSpacingFactor );

/* Set line spacing factor */
extern void						Text_SetLineSpacingFactor( text_t Object, float LineSpacingFactor );

/* Get font */
extern font_t					Text_GetFont( text_t Object );

/* Get character size */
extern uint32_t					Text_GetCharacterSize( text_t Object );

/* Get text */
extern const char*				Text_GetText( text_t Object );

/* Get material */
extern material_t				Text_GetMaterial( text_t Object );

/* Get letter spacing factor */
extern float					Text_GetLetterSpacingFactor( text_t Object );

/* Get line spacing factor */
extern float					Text_GetLineSpacingFactor( text_t Object );

/* Move */
extern void						Text_Move( text_t Object, vec3f_t FactorMove );

/* Rotate text by euler angles */
extern void						Text_EulerRotate( text_t Object, vec3f_t FactorRotate );

/* Rotate text by quaternion */
extern void						Text_QuatRotate( text_t Object, quat_t FactorRotate );

/* Scale text */
extern void						Text_Scale( text_t Object, vec3f_t FactorScale );

/* Set position text */
extern void						Text_SetPosition( text_t Object, vec3f_t Position );

/* Set rotation by euler angles text */
extern void						Text_SetEulerRotation( text_t Object, vec3f_t Rotation );

/* Set rotation by quaternion text */
extern void						Text_SetQuatRotation( text_t Object, quat_t Rotation );

/* Set text scale */
extern void						Text_SetScale( text_t Object, vec3f_t Scale );

/* Get text position */
extern vec3f_t					Text_GetPosition( text_t Object );

/* Get quaternion text rotation */
extern quat_t					Text_GetRotation( text_t Object );

/* Get scale text */
extern vec3f_t					Text_GetScale( text_t Object );

//---------------------------------------------------------------------//

#endif // API_TEXT_H
