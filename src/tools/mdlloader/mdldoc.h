//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

#include "mdltypes.h"

//---------------------------------------------------------------------//

class MDLDoc
{
public:
	MDLDoc();
	~MDLDoc();

	bool										Load( const std::string& Path );
	bool										Save( const std::string& Path );
	void										Clear();
	void										AddVertex( const MDLVertex& Vertex );
	void										AddVertexIndex( std::uint32_t VertexIndex );
	void										AddSurface( const MDLSurface& Surface );
	void										AddMaterial( const std::string& MaterialPath );
	void										RemoveVertex( std::uint32_t Index );
	void										RemoveVertexIndex( std::uint32_t Index );
	void										RemoveSurface( std::uint32_t Index );
	void										RemoveMaterial( std::uint32_t Index );

	void										SetMinXYZ( const MDLVector3D& MinXYZ );
	void										SetMaxXYZ( const MDLVector3D& MaxXYZ );
	void										SetVertex( const MDLVertex& Vertex, std::uint32_t Index );
	void										SetVertexIndex( std::uint32_t VertexIndex, std::uint32_t Index );
	void										SetSurface( const MDLSurface& Surface, std::uint32_t Index );
	void										SetMaterial( const std::string& MaterialPath, std::uint32_t Index );
	void										SetVerteces( const std::vector< MDLVertex >& Verteces );
	void										SetVertexIndeces( const std::vector< std::uint32_t >& VertexIndeces );
	void										SetSurfaces( const std::vector< MDLSurface >& Surfaces );
	void										SetMaterials( const std::vector< std::string >& MaterialPaths );

	const MDLVector3D&							GetMinXYZ() const;
	const MDLVector3D&							GetMaxXYZ() const;
	std::uint32_t								GetCountVerteces() const;
	std::uint32_t								GetCountVertexIndeces() const;
	std::uint32_t								GetCountSurfaces() const;
	std::uint32_t								GetCountMaterials() const;
	MDLVertex									GetVertex( std::uint32_t Index ) const;
	std::uint32_t								GetVertexIndex( std::uint32_t Index ) const;
	MDLSurface									GetSurface( std::uint32_t Index ) const;
	std::string									GetMaterial( std::uint32_t Index ) const;
	const std::vector< MDLVertex >&				GetVerteces() const;	
	const std::vector< std::uint32_t >&			GetVertexIndeces() const;	
	const std::vector< MDLSurface >&			GetSurfaces() const;	
	const std::vector< std::string >&			GetMaterials() const;
	
private:
	void						UpdateLumps( std::ofstream& File, MDLLump* Lumps, std::streampos OffsetToLumps );
	void						WriteMaterials( std::ofstream& File, MDLLump& Lump );
	void						WriteVerteces( std::ofstream& File, MDLLump& Lump );
	void						WriteIndeces( std::ofstream& File, MDLLump& Lump );
	void						WriteSurfaces( std::ofstream& File, MDLLump& Lump );

	MDLVector3D							minXYZ;
	MDLVector3D							maxXYZ;

	std::vector< MDLVertex >			verteces;
	std::vector< std::uint32_t >		vertexIndeces;
	std::vector< MDLSurface >			surfaces;
	std::vector< std::string >			materials;
};

//---------------------------------------------------------------------//