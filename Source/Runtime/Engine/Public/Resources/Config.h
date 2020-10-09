// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

#include "Resources/Resource.h"

namespace le
{
	class Config : public Resource
	{
	public:

		enum EVarType
		{
			VT_Unknown,
			VT_Int,
			VT_Float,
			VT_String
		};

		class Var
		{
		public:
			/* Constructor */
			Var();

			/* Constructor of copy */
			Var( const Var& InCopy );

			/* Destructor */
			~Var();

			/* Set name */
			FORCEINLINE void SetName( const std::string& InName )		{ name = InName; }

			/* Set value int */
			void SetValueInt( int InValue );

			/* Set value float */
			void SetValueFloat( float InValue );

			/* Set value string */
			void SetValueString( const std::string& InValue );

			/* Is empty */
			FORCEINLINE bool IsEmpty() const							{ return !value; }

			/* Get type */
			FORCEINLINE EVarType GetType() const						{ return type; }

			/* Get name */
			FORCEINLINE const std::string& GetName() const				{ return name; }

			/* Get value int */
			int GetValueInt() const;

			/* Get value float */
			float GetValueFloat() const;

			/* Get value string */
			std::string GetValueString() const;

			/* Operator = */
			FORCEINLINE Var& operator=( const Var& InRight )
			{
				name = InRight.name;
				switch ( InRight.type )
				{
				case VT_Int:		SetValueInt( InRight.GetValueInt() );		break;
				case VT_Float:		SetValueFloat( InRight.GetValueFloat() );	break;
				case VT_String:		SetValueString( InRight.GetValueString() );	break;
				}

				return *this;
			}

		private:
			/* Destroy value */
			void DestroyValue();

			EVarType		type;
			std::string		name;
			void*			value;
		};

		class Group
		{
		public:
			/* Add var */
			FORCEINLINE void AddVar( const Var& InVar )				{ vars.push_back( InVar ); }

			/* Remove var by index */
			FORCEINLINE void RemoveVar( uint32 InIndex )
			{
				LIFEENGINE_ASSERT( InIndex < vars.size() );
				vars.erase( InIndex + vars.begin() );
			}

			/* Remove var by name */
			FORCEINLINE void RemoveVar( const std::string& InName )
			{
				for ( uint32 index = 0, count = static_cast< uint32 >( vars.size() ); index < count; ++index )
					if ( vars[ index ].GetName() == InName )
					{
						vars.erase( index + vars.begin() );
						return;
					}
			}

			/* Remove all vars */
			FORCEINLINE void RemoveAllVars()							{ vars.clear(); }

			/* Set name */
			FORCEINLINE void SetName( const std::string& InName )		{ name = InName; }

			/* Is empty */
			FORCEINLINE bool IsEmpty() const							{ return vars.empty(); }

			/* Get name */
			FORCEINLINE const std::string& GetName() const				{ return name; }

			/* Get var by index */
			FORCEINLINE Var* GetVar( uint32 InIndex )
			{
				LIFEENGINE_ASSERT( InIndex < vars.size() );
				return &vars[ InIndex ];
			}
			FORCEINLINE const Var* GetVar( uint32 InIndex ) const
			{
				LIFEENGINE_ASSERT( InIndex < vars.size() );
				return &vars[ InIndex ];
			}

			/* Get var by name */
			FORCEINLINE Var* GetVar( const std::string& InName )
			{
				for ( uint32 index = 0, count = static_cast< uint32 >( vars.size() ); index < count; ++index )
					if ( vars[ index ].GetName() == InName )
						return &vars[ index ];

				return nullptr;
			}
			FORCEINLINE const Var* GetVar( const std::string& InName ) const
			{
				for ( uint32 index = 0, count = static_cast< uint32 >( vars.size() ); index < count; ++index )
					if ( vars[ index ].GetName() == InName )
						return &vars[ index ];

				return nullptr;
			}

			/* Get vars */
			FORCEINLINE const std::vector< Var >& GetVars() const		{ return vars; }
			FORCEINLINE std::vector< Var >& GetVars()					{ return vars; }

		private:
			std::string				name;
			std::vector< Var >		vars;
		};

		/* Serialize resource */
		bool Serialize( const Path& InPath ) override;

		/* Deserialize resource */
		bool Deserialize( const Path& InPath ) override;

		/* Add group */
		FORCEINLINE void AddGroup( const Group& InGroup )				{ groups.push_back( InGroup ); }

		/* Remove group */
		FORCEINLINE void RemoveGroup( uint32 InIndex )
		{
			LIFEENGINE_ASSERT( InIndex < groups.size() );
			groups.erase( InIndex + groups.begin() );
		}

		FORCEINLINE void RemoveGroup( const std::string& InName )
		{
			for ( uint32 index = 0, count = static_cast< uint32 >( groups.size() ); index < count; ++index )
				if ( groups[ index ].GetName() == InName )
				{
					groups.erase( index + groups.begin() );
					return;
				}
		}	

		/* Remove all groups */
		FORCEINLINE void RemoveAllGroups()								{ groups.clear(); }

		/* Is empty */
		FORCEINLINE bool IsEmpty() const								{ return groups.empty(); }

		/* Get resource type */
		EResourceType GetType() const override;

		/* Get group by index */
		FORCEINLINE Group* GetGroup( uint32 InIndex )
		{
			LIFEENGINE_ASSERT( InIndex < groups.size() );
			return &groups[ InIndex ];
		}
		FORCEINLINE const Group* GetGroup( uint32 InIndex ) const
		{
			LIFEENGINE_ASSERT( InIndex < groups.size() );
			return &groups[ InIndex ];
		}

		/* Get group by name */
		FORCEINLINE Group* GetGroup( const std::string& InName )
		{
			for ( uint32 index = 0, count = static_cast< uint32 >( groups.size() ); index < count; ++index )
				if ( groups[ index ].GetName() == InName )
					return &groups[ index ];

			return nullptr;
		}
		FORCEINLINE const Group* GetGroup( const std::string& InName ) const
		{
			for ( uint32 index = 0, count = static_cast< uint32 >( groups.size() ); index < count; ++index )
				if ( groups[ index ].GetName() == InName )
					return &groups[ index ];

			return nullptr;
		}

		/* Get groups */
		FORCEINLINE const std::vector< Group >& GetGroups() const		{ return groups; }
		FORCEINLINE std::vector< Group >& GetGroups()					{ return groups; }

	private:
		std::vector< Group >		groups;
	};
}

#endif // !CONFIG_H
