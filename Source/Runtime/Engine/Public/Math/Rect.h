// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RECT_H
#define RECT_H

namespace le
{
	template< typename T >
	struct SRect
	{
		/* Constructor */
		SRect()
		{}

		/* Constructor */
		SRect( const T& InLeft, const T& InBottom, const T& InWidth, const T& InHeight ) :
			left( InLeft ),
			bottom( InBottom ),
			width( InWidth ),
			height( InHeight )
		{}

		/* Constructor of copy */
		SRect( const SRect< T >& InCopy ) :
			left( InCopy.left ),
			bottom( InCopy.bottom ),
			width( InCopy.width ),
			height( InCopy.height )
		{}

		/* Operator of copy */
		SRect< T >& operator=( const SRect< T >& InRight )
		{
			left = InRight.left;
			bottom = InRight.bottom;
			width = InRight.width;
			height = InRight.height;
			return *this;
		}
		const SRect< T >& operator=( const SRect< T >& InRight ) const
		{
			left = InRight.left;
			bottom = InRight.bottom;
			width = InRight.width;
			height = InRight.height;
			return *this;
		}

		T		left;
		T		bottom;
		T		width;
		T		height;
	};

	typedef SRect< int >		FSRectInt;
	typedef SRect< float >		FSRectFloat;
}

#endif // !RECT_H
