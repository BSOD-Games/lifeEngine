//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "phytypes.h"

//---------------------------------------------------------------------//

class PHYDoc
{
public:
	PHYDoc();
	~PHYDoc();

	bool										Load( const std::string& Path );
	bool										Save( const std::string& Path );
	void										Clear();
	void										AddVertex( const PHYVector3D& Vertex );
	void										AddVertexIndex( std::uint32_t VertexIndex );
	void										RemoveVertex( std::uint32_t Index );
	void										RemoveVertexIndex( std::uint32_t Index );

	void										SetStatic( bool IsStatic = true );
	void										SetMasa( float Masa );
	void										SetInertia( const PHYVector3D& Inertia );
	void										SetVertex( const PHYVector3D& Vertex, std::uint32_t Index );
	void										SetVertexIndex( std::uint32_t VertexIndex, std::uint32_t Index );
	void										SetVerteces( const std::vector< PHYVector3D >& Verteces );
	void										SetVertexIndeces( const std::vector< std::uint32_t >& VertexIndeces );

	bool										IsStatic() const;
	float										GetMasa() const;
	const PHYVector3D&							GetInertia() const;
	std::uint32_t								GetCountVerteces() const;
	std::uint32_t								GetCountVertexIndeces() const;
	PHYVector3D									GetVertex( std::uint32_t Index ) const;
	std::uint32_t								GetVertexIndex( std::uint32_t Index ) const;
	const std::vector< PHYVector3D >&			GetVerteces() const;
	const std::vector< std::uint32_t >&			GetVertexIndeces() const;

private:
	void										UpdateLumps( std::ofstream& File, PHYLump* Lumps, std::uint32_t OffsetToLumps );
	void										WriteParameters( std::ofstream& File, PHYLump& Lump );
	void										WriteVerteces( std::ofstream& File, PHYLump& Lump );
	void										WriteIndeces( std::ofstream& File, PHYLump& Lump );

	bool								isStatic;
	float								masa;
	PHYVector3D							inertia;
	std::vector< PHYVector3D >			verteces;
	std::vector< std::uint32_t >		vertexIndeces;
};

//---------------------------------------------------------------------//