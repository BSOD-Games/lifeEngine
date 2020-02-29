//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONVAR_H
#define CONVAR_H

#include <string>

#include "engine/iconvar.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ConVar : public IConVar
	{
	public:
		// IConVar
		virtual void				Initialize( const char* Name, const char* DefaultValue, CONVAR_TYPE VarType, const char* HelpString, ChangeCallbackFn_t ChangeCallback, bool IsReadOnly = false );
		virtual void				Initialize( const char* Name, const char* DefaultValue, CONVAR_TYPE VarType, const char* HelpString, bool HasMin, float Min, bool HasMax, float Max, ChangeCallbackFn_t ChangeCallback, bool IsReadOnly = false );
		virtual void				Revert();

		virtual void				SetChangeCallback( ChangeCallbackFn_t ChangeCallback );
		virtual void				SetName( const char* Name );
		virtual void				SetMin( bool HasMin, float Min = 0 );
		virtual void				SetMax( bool HasMax, float Max = 0 );
		virtual void				SetValue( const char* Value, CONVAR_TYPE VarType );
		virtual void				SetValueInt( int Value );
		virtual void				SetValueFloat( float Value );
		virtual void				SetValueBool( bool Value );
		virtual void				SetValueString( const char* Value );

		virtual const char*			GetName() const;
		virtual CONVAR_TYPE			GetType() const;
		virtual int					GetValueInt() const;
		virtual float				GetValueFloat() const;
		virtual bool				GetValueBool() const;
		virtual const char*			GetValueString() const;
		virtual const char*			GetHelpText() const;
		virtual const char*			GetValueDefault() const;
		virtual float				GetMin() const;
		virtual float				GetMax() const;

		virtual bool				IsReadOnly() const;
		virtual bool				HasMin() const;
		virtual bool				HasMax() const;

		// ConVar
		ConVar();
		~ConVar();

	private:

        union Values
        {
            Values();
            ~Values();

            int				value_int;
            float			value_float;
            bool			value_bool;
            std::string		value_string;
        };

		bool				isReadOnly;
		bool				hasMin;	
		bool				hasMax;	

		float				min;	
		float				max;	

		CONVAR_TYPE			type;
		ChangeCallbackFn_t	changeCallback;
		std::string			defaultValue;
		std::string			helpString;
		std::string			name;
        Values              values;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !CONVAR_H
