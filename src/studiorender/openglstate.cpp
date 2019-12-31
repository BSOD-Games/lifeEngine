//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <functional>

#include "studiorender/istudiorender.h"
#include "openglstate.h"

// ------------------------------------------------------------------------------------ //
// Объеденение хеша
// ------------------------------------------------------------------------------------ //
inline void HashCombine( std::size_t& Seed ) 
{}

// ------------------------------------------------------------------------------------ //
// Объеденение хеша
// ------------------------------------------------------------------------------------ //
template<typename T, typename... Arguments>
inline void HashCombine( std::size_t& Seed, const T& Value, Arguments... Values )
{
	std::hash< T >		hasher;
	Seed ^= hasher( Value ) + 0x9E3779B9 + ( Seed << 6 ) + ( Seed >> 2 );
	HashCombine( Seed, Values... );
}

// ------------------------------------------------------------------------------------ //
// Неявное преобразование к std::size_t
// ------------------------------------------------------------------------------------ //
std::size_t le::Hash_OpenGLState::operator()( const OpenGLState& Right ) const
{
	std::size_t				hash = 0;
	HashCombine( hash, 
				 std::hash< bool >{} ( Right.isDepthTest ),
				 std::hash< bool >{} ( Right.isCullFace ), 
				 std::hash< bool >{} ( Right.isBlend ), 
				 std::hash< CULLFACE_TYPE >{} ( Right.cullFaceType ) );

	return hash;
}

// ------------------------------------------------------------------------------------ //
// Сравнение состояний OpenGL'a
// ------------------------------------------------------------------------------------ //
bool le::operator==( const OpenGLState& Left, const OpenGLState& Right )
{
	return Left.isDepthTest == Right.isDepthTest && Left.isCullFace == Right.isCullFace &&
		Left.isBlend == Right.isBlend && Left.cullFaceType == Right.cullFaceType;
}
