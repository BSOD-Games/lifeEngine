// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef ACTORVAR_H
#define ACTORVAR_H

#include <string>

#include "Misc/Object.h"
#include "Misc/Types.h"
#include "Math/Math.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "Resources/Material.h"

namespace le
{
	enum EActorVarType
	{
		AVT_Unknown,
		AVT_Int,
		AVT_Float,
		AVT_Bool,
		AVT_Vector2D,
		AVT_Vector3D,
		AVT_Vector4D,
		AVT_RectInt,
		AVT_RectFloat,
		AVT_Color,
		AVT_Material
	};

	class ActorVar : public Object
	{
	public:
		/* Constructor */
		ActorVar();

		/* Constructor of copy */
		ActorVar( const ActorVar& InCopy );

		/* Destructor */
		~ActorVar();

		/* Clear value */
		void Clear();

		/* Set name */
		FORCEINLINE void SetName( const std::string& InName )		{ name = InName; }

		/* Set value int */
		void SetValueInt( int InValue );

		/* Set value float */
		void SetValueFloat( float InValue );

		/* Set value bool */
		void SetValueBool( bool InValue );

		/* Set value vector 2D */
		void SetValueVector2D( const FVector2D& InValue );

		/* Set value vector 3D */
		void SetValueVector3D( const FVector3D& InValue );

		/* Set value vector 4D */
		void SetValueVector4D( const FVector4D& InValue );

		/* Set value rect int */
		void SetValueRectInt( const FSRectInt& InValue );

		/* Set value rect float */
		void SetValueRectFloat( const FSRectFloat& InValue );

		/* Set value color */
		void SetValueColor( const SColor& InValue );

		/* Set value material */
		void SetValueMaterial( FMaterialConstRef& InValue );

		/* Is defined */
		FORCEINLINE bool IsDefined() const				{ return value; }

		/* Get name */
		FORCEINLINE const std::string& GetName() const	{ return name; }

		/* Get type */
		FORCEINLINE EActorVarType GetType() const		{ return type; }

		/* Get value int */
		int GetValueInt() const;

		/* Get value float */
		float GetValueFloat() const;

		/* Get value bool */
		bool GetValueBool() const;

		/* Get value vector 2D */
		FVector2D GetValueVector2D() const;

		/* Get value vector 3D */
		FVector3D GetValueVector3D() const;

		/* Get value vector 4D */
		FVector4D GetValueVector4D() const;

		/* Get value rect int */
		FSRectInt GetValueRectInt() const;

		/* Get value rect float */
		FSRectFloat GetValueRectFloat() const;

		/* Get value color */
		SColor GetValueColor() const;

		/* Get value material */
		FMaterialRef GetValueMaterial() const;

		/* Operator = */
		FORCEINLINE ActorVar& operator=( const ActorVar& InRight )
		{
			name = InRight.name;
			switch ( InRight.type )
			{
			case AVT_Int:		SetValueInt( InRight.GetValueInt() );				break;
			case AVT_Float:		SetValueFloat( InRight.GetValueFloat() );			break;
			case AVT_Bool:		SetValueBool( InRight.GetValueBool() );				break;
			case AVT_Vector2D:	SetValueVector2D( InRight.GetValueVector2D() );		break;
			case AVT_Vector3D:	SetValueVector3D( InRight.GetValueVector3D() );		break;
			case AVT_Vector4D:	SetValueVector4D( InRight.GetValueVector4D() );		break;
			case AVT_RectInt:	SetValueRectInt( InRight.GetValueRectInt() );		break;
			case AVT_RectFloat:	SetValueRectFloat( InRight.GetValueRectFloat() );	break;
			case AVT_Color:		SetValueColor( InRight.GetValueColor() );			break;
			case AVT_Material:	SetValueMaterial( InRight.GetValueMaterial() );		break;

			default:
				type = AVT_Unknown;
				value = nullptr;
				break;
			}

			return *this;
		}

	private:
		std::string			name;
		EActorVarType		type;
		void*				value;
	};
}

#endif // !ACTORVAR_H
