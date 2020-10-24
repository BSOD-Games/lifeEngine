// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef EVENTCHANNEL_H
#define EVENTCHANNEL_H

#include <vector>
#include "Misc/Object.h"

namespace le
{
	template< typename EventType >
	class TEventChannel
	{
	public:
		typedef		void	( *EventCallbackFunction )( const EventType& InEvent );
		typedef		void	( Object::*EventCallbackMethod )( const EventType& InEvent );

		/* Subscribe callback function on event */
		FORCEINLINE void Subscribe( EventCallbackFunction InEventCallbackFunction )			
		{ 
			LIFEENGINE_ASSERT( EventCallbackFunction );
			callbackFunctions.push_back( InEventCallbackFunction );
		}

		/* Subscribe callback method on event */
		void Subscribe( Object* InObject, EventCallbackMethod InEventCallbackMethod )			
		{ 
			LIFEENGINE_ASSERT( InObject && InEventCallbackMethod );
			callbackMethods.push_back( SCallbackMethod{ InObject, InEventCallbackMethod } );
		}

		/* Subscribe callback method on event */
		template< typename ClassType >
		FORCEINLINE void Subscribe( ClassType* InObject, void ( ClassType::*InEventCallbackMethod )( const EventType& InEvent ) )			{ Subscribe( ( Object* ) InObject, ( EventCallbackMethod ) InEventCallbackMethod ); }

		/* Unsubscribe callback function from event */
		void Unsubscribe( EventCallbackFunction InEventCallbackFunction )
		{
			for ( uint32 index = 0, count = static_cast< uint32 >( callbackFunctions.size() ); index < count; ++index )
				if ( callbackFunctions[ index ] == InEventCallbackFunction )
				{
					callbackFunctions.erase( index + callbackFunctions.begin() );
					return;
				}
		}

		/* Unsubscribe callback method from event */
		void Unsubscribe( Object* InObject, EventCallbackMethod InEventCallbackMethod )
		{
			for ( uint32 index = 0, count = static_cast< uint32 >( callbackMethods.size() ); index < count; ++index )
			{
				const SCallbackMethod&			callbackMethod = callbackMethods[ index ];
				if ( callbackMethod.object == InObject && callbackMethod.method == InEventCallbackMethod )
				{
					callbackMethods.erase( index + callbackMethods.begin() );
					return;
				}
			}
		}

		/* Subscribe callback method on event */
		template< typename ClassType >
		FORCEINLINE void Unsubscribe( ClassType* InObject, void ( ClassType::*InEventCallbackMethod )( const EventType& InEvent ) )		{ Unsubscribe( ( Object* ) InObject, ( EventCallbackMethod ) InEventCallbackMethod ); }

		/* Emit event */
		void Emit( const EventType& InEvent ) const
		{
			// Call callback functions
			if ( !callbackFunctions.empty() )
			{
				for ( uint32 index = 0, count = static_cast< uint32 >( callbackFunctions.size() ); index < count; ++index )
					callbackFunctions[ index ]( InEvent );
			}

			// Call callback methods
			if ( !callbackMethods.empty() )
			{
				for ( uint32 index = 0, count = static_cast< uint32 >( callbackMethods.size() ); index < count; ++index )
				{
					const SCallbackMethod&		callback = callbackMethods[ index ];
					( callback.object->*callback.method )( InEvent );
				}
			}
		}

	private:
		struct SCallbackMethod
		{
			Object*					object;
			EventCallbackMethod		method;
		};

		std::vector< EventCallbackFunction >		callbackFunctions;
		std::vector< SCallbackMethod >				callbackMethods;
	};
}

#endif // !EVENTCHANNEL_H
