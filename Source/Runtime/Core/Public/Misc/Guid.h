// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef GUID_H
#define GUID_H

#include <memory>

#include "CoreDefines.h"
#include "Types.h"

namespace le
{
	struct SGuid
	{
		/**
		 * Compares two GUIDs for equality.
		 *
		 * @param Right The first GUID to compare.
		 * @param Left The second GUID to compare.
		 * @return true if the GUIDs are equal, false otherwise.
		 */
		friend bool operator==( const SGuid& Left, const SGuid& Right )
		{
			return Left.data1 == Right.data1 && Left.data2 == Right.data2 && Left.data3 == Right.data3 && !memcmp( Left.data4, Right.data4, sizeof( uint8 ) * 8 );
		}

		/**
		 * Compares two GUIDs for inequality.
		 *
		 * @param Left The first GUID to compare.
		 * @param Right The second GUID to compare.
		 * @return true if the GUIDs are not equal, false otherwise.
		 */
		friend bool operator!=( const SGuid& Left, const SGuid& Right )
		{
			return Left.data1 != Right.data1 || Left.data2 != Right.data2 || Left.data3 != Right.data3 || memcmp( Left.data4, Right.data4, sizeof( uint8 ) * 8 );
		}

		/**
		 * Compares two GUIDs.
		 *
		 * @param Left The first GUID to compare.
		 * @param Right The second GUID to compare.
		 * @return true if the first GUID is less than the second one.
		 */
		friend bool operator<( const SGuid& Left, const SGuid& Right )
		{
			return	( ( Left.data1 < Right.data1 ) ? true : ( ( Left.data1 > Right.data1 ) ? false :
					( ( Left.data2 < Right.data2 ) ? true : ( ( Left.data2 > Right.data2 ) ? false :
					( ( Left.data3 < Right.data3 ) ? true : ( ( Left.data3 > Right.data3 ) ? false :
					( ( Left.data4[ 0 ] < Right.data4[ 0 ] ) ? true : ( ( Left.data4[ 0 ] > Right.data4[ 0 ] ) ? false : 
					( ( Left.data4[ 1 ] < Right.data4[ 1 ] ) ? true : ( ( Left.data4[ 1 ] > Right.data4[ 1 ] ) ? false : 
					( ( Left.data4[ 2 ] < Right.data4[ 2 ] ) ? true : ( ( Left.data4[ 2 ] > Right.data4[ 2 ] ) ? false : 
					( ( Left.data4[ 3 ] < Right.data4[ 3 ] ) ? true : ( ( Left.data4[ 3 ] > Right.data4[ 3 ] ) ? false : 
					( ( Left.data4[ 4 ] < Right.data4[ 4 ] ) ? true : ( ( Left.data4[ 4 ] > Right.data4[ 4 ] ) ? false : 
					( ( Left.data4[ 5 ] < Right.data4[ 5 ] ) ? true : ( ( Left.data4[ 5 ] > Right.data4[ 5 ] ) ? false : 
					( ( Left.data4[ 6 ] < Right.data4[ 6 ] ) ? true : ( ( Left.data4[ 6 ] > Right.data4[ 6 ] ) ? false : 
					( ( Left.data4[ 7 ] < Right.data4[ 7 ] ) ? true : ( ( Left.data4[ 7 ] > Right.data4[ 7 ] ) ? false :  false ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) );
		}

		uint32			data1;
		uint16			data2;
		uint16			data3;
		uint8			data4[ 8 ];
	};
}

#endif // !GUID_H