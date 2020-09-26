// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef SHADERVAR_H
#define SHADERVAR_H

#include <string>

#include "Misc/EngineDefines.h"
#include "Math/Vector2D.h"
#include "Math/Color.h"

namespace le
{
	struct SColor;
	struct SVector2D;
	class Texture2D;

	enum EShaderVarType
	{
		SVT_Unknown,
		SVT_Int,
		SVT_Float,
		SVT_Bool,
		SVT_Vector2D,
		SVT_Color,
		SVT_Texture2D
	};

	class ShaderVar
	{
	public:
		/* Constructor */
		ShaderVar();

		/* Constructor of copy */
		ShaderVar( const ShaderVar& InCopy );

		/* Destructor */
		~ShaderVar();

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
		void SetValueVector2D( const SVector2D& InValue );

		/* Set value color */
		void SetValueColor( const SColor& InValue );

		/* Set value texture 2D */
		void SetValueTexture2D( Texture2D* InValue );

		/* Is defined */
		FORCEINLINE bool IsDefined() const							{ return value; }

		/* Get name */
		FORCEINLINE const std::string& GetName() const				{ return name; }

		/* Get type */
		FORCEINLINE EShaderVarType GetType() const					{ return type; }

		/* Get value int */
		int GetValueInt() const;

		/* Get value float */
		float GetValueFloat() const;

		/* Get value bool */
		bool GetValueBool() const;

		/* Get value vector 2D */
		SVector2D GetValueVector2D() const;

		/* Get value color */
		SColor GetValueColor() const;

		/* Get value texture 2D */
		Texture2D* GetValueTexture2D() const;

		/* Operator = */
		FORCEINLINE ShaderVar& operator=( const ShaderVar& InRight )
		{
			name = InRight.name;
			switch ( InRight.type )
			{
			case SVT_Int:		SetValueInt( InRight.GetValueInt() );				break;
			case SVT_Float:		SetValueFloat( InRight.GetValueFloat() );			break;
			case SVT_Bool:		SetValueBool( InRight.GetValueBool() );				break;
			case SVT_Vector2D:	SetValueVector2D( InRight.GetValueVector2D() );		break;
			case SVT_Color:		SetValueColor( InRight.GetValueColor() );			break;
			case SVT_Texture2D:	SetValueTexture2D( InRight.GetValueTexture2D() );	break;
			
			default:			
				type = SVT_Unknown;
				value = nullptr;									
				break;
			}

			return *this;
		}

	private:
		std::string			name;
		EShaderVarType		type;
		void*				value;
	};
}

#endif // !SHADERVAR_H
