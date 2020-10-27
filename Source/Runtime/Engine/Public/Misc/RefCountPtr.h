// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef REFCOUNTPTR_H
#define REFCOUNTPTR_H

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"

namespace le
{
	template< typename PtrType >
	class TRefCountPtr
	{
	public:
		/* Constructor */
		TRefCountPtr() :
			ptr( nullptr )
		{}

		/* Constructor */
		TRefCountPtr( PtrType* InPtr ) :
			ptr( InPtr )
		{
			if ( ptr )		ptr->AddRef();
		}

		/* Constructor of copy */
		TRefCountPtr( const TRefCountPtr& InCopy ) :
			ptr( InCopy.ptr )
		{
			if ( ptr )		ptr->AddRef();
		}

		/* Constructor of copy */
		template< typename BasePtrType >
		TRefCountPtr( const TRefCountPtr< BasePtrType >& InCopy ) :
			ptr( static_cast< PtrType* >( InCopy.GetPtr() ) )
		{
			if ( ptr )		ptr->AddRef();
		}

		/* Destructor */
		~TRefCountPtr()
		{
			if ( ptr )		ptr->ReleaseRef();
		}

		/* operator = */
		FORCEINLINE TRefCountPtr& operator=( PtrType* InCopy )
		{
			if ( ptr )		ptr->ReleaseRef();

			ptr = InCopy;
			if ( ptr )		ptr->AddRef();
			return *this;
		}

		FORCEINLINE TRefCountPtr& operator=( const TRefCountPtr& InCopy )
		{
			if ( ptr )		ptr->ReleaseRef();

			ptr = InCopy.ptr;
			if ( ptr )		ptr->AddRef();
			return *this;
		}

		template< typename BasePtrType >
		FORCEINLINE TRefCountPtr& operator=( const TRefCountPtr< BasePtrType >& InCopy )
		{
			if ( ptr )		ptr->ReleaseRef();

			ptr = static_cast< PtrType* >( InCopy.GetPtr() );
			if ( ptr )		ptr->AddRef();
			return *this;
		}

		/* operator == */
		FORCEINLINE bool operator==( const TRefCountPtr& InRight ) const		{ return ptr == InRight.ptr; }

		/* operator != */
		FORCEINLINE bool operator!=( const TRefCountPtr& InRight ) const		{ return ptr != InRight.ptr; }

		/* operator bool */
		FORCEINLINE operator bool() const										{ return ptr; }

		/* operator -> */
		FORCEINLINE PtrType* operator->() const
		{
			LIFEENGINE_ASSERT( ptr );
			return ptr;
		}

		/* operator * */
		FORCEINLINE PtrType& operator*()
		{
			LIFEENGINE_ASSERT( ptr );
			return *ptr;
		}

		/* operator & */
		FORCEINLINE PtrType** operator&()
		{
			LIFEENGINE_ASSERT( ptr );
			return &ptr;
		}

		/* Is valid */
		FORCEINLINE bool IsValid() const		{ return ptr; }

		/* Get reference count */
		FORCEINLINE uint32 GetRefCount() const
		{
			if ( !ptr )		return 0;
			return ptr->GetRefCount();
		}

		/* Get ptr */
		FORCEINLINE PtrType* GetPtr() const			{ return ptr; }

	private:
		PtrType*		ptr;
	};
}

#endif // !REFCOUNTPTR_H
