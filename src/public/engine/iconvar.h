//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICONVAR_H
#define ICONVAR_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IConVar;
	typedef	void				( *ChangeCallbackFn_t )( IConVar* ConVar );

	//---------------------------------------------------------------------//

	enum CONVAR_TYPE
	{
		CVT_UNDEFINED,
		CVT_INT,	
		CVT_FLOAT,	
		CVT_BOOL,	
		CVT_STRING	
	};

	//---------------------------------------------------------------------//

	class IConVar
	{
	public:
		virtual void				Initialize( const char* Name, const char* DefaultValue, CONVAR_TYPE VarType, const char* HelpString, ChangeCallbackFn_t ChangeCallback, bool IsReadOnly = false ) = 0;
		virtual void				Initialize( const char* Name, const char* DefaultValue, CONVAR_TYPE VarType, const char* HelpString, bool HasMin, float Min, bool HasMax, float Max, ChangeCallbackFn_t ChangeCallback, bool IsReadOnly = false ) = 0;
		virtual void				Revert() = 0;

		virtual void				SetChangeCallback( ChangeCallbackFn_t ChangeCallback ) = 0;
		virtual void				SetName( const char* Name ) = 0;
		virtual void				SetMin( bool HasMin, float Min = 0 ) = 0;
		virtual void				SetMax( bool HasMax, float Max = 0 ) = 0;
		virtual void				SetValue( const char* Value, CONVAR_TYPE VarType ) = 0;
		virtual void				SetValueInt( int Value ) = 0;
		virtual void				SetValueFloat( float Value ) = 0;
		virtual void				SetValueBool( bool Value ) = 0;
		virtual void				SetValueString( const char* Value ) = 0;
		
		virtual const char*			GetName() const = 0;
		virtual CONVAR_TYPE			GetType() const = 0;
		virtual int					GetValueInt() const = 0;
		virtual float				GetValueFloat() const = 0;
		virtual bool				GetValueBool() const = 0;
		virtual const char*			GetValueString() const = 0;
		virtual const char*			GetHelpText() const = 0;
		virtual const char*			GetValueDefault() const = 0;
		virtual float				GetMin() const = 0;
		virtual float				GetMax() const = 0;

		virtual bool				IsReadOnly() const = 0;
		virtual bool				HasMin() const = 0;
		virtual bool				HasMax() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define CONVAR_INTERFACE_VERSION "LE_ConVar001"

//---------------------------------------------------------------------//

#endif // !ICONVAR_H

