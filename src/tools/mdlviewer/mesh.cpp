#include "mesh.h"
#include "engineapi.h"
#include "common/shaderparaminfo.h"
#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "studiorender/studiovertexelement.h"
#include "engine/consolesystem.h"
#include "engine/imodel.h"
#include "studiorender/istudiorender.h"
#include "mdldoc.h"
#include "ui_window_viewer.h"

#include <qfiledialog.h>
#include <qdebug.h>


//-----------------------------------------------------------------
// Load model
//-----------------------------------------------------------------
bool Mesh::Load(const QString& Path)
{

	// Clear mesh, mdlDoc and path
	if ( mesh )						Clear();

	// Load model
	if ( !mdlDoc.Load( Path.toStdString() ) ) return false;

	// Getting all data	
	auto& verteces = mdlDoc.GetVerteces();
	auto& vertexIndeces = mdlDoc.GetVertexIndeces();
	auto& materialPaths = mdlDoc.GetMaterials();
	auto& surfaces = mdlDoc.GetSurfaces();
	MDLVector3D			minXYZ = mdlDoc.GetMinXYZ();
	MDLVector3D			maxXYZ = mdlDoc.GetMaxXYZ();
	
	// Loading materials
	std::vector< le::IMaterial* >		materials;
	for (le::UInt32_t index = 0, count = materialPaths.size(); index < count; ++index)
	{
		le::IMaterial* material = EngineAPI::GetInstance()->GetResourceSystem()->LoadMaterial(materialPaths[index].c_str(), materialPaths[index].c_str());
		if (!material) continue;

		materials.push_back(material);
	}

	// Convert MDLSurface to le::MeshSurface
	std::vector< le::MeshSurface >			meshSurfaces;
	for (le::UInt32_t index = 0, count = surfaces.size(); index < count; ++index)
	{
		const MDLSurface& mdlSurface = surfaces[index];
		le::MeshSurface			surface;

		surface.startVertexIndex = 0;
		surface.lightmapID = 0;
		surface.materialID = mdlSurface.materialId;
		surface.startIndex = mdlSurface.startVertexIndex;
		surface.countIndeces = mdlSurface.countVertexIndeces;
		meshSurfaces.push_back(surface);
	}

	mesh = (le::IMesh*) EngineAPI::GetInstance()->GetStudioRender()->GetFactory()->Create(MESH_INTERFACE_VERSION);
	if (!mesh)				return false;
	qDebug() << "Create mesh";

	// Create descriptor format verteces
	std::vector< le::StudioVertexElement >			vertexElements =
	{
		{ 3, le::VET_FLOAT },
		{ 3, le::VET_FLOAT },
		{ 2, le::VET_FLOAT },
		{ 3, le::VET_FLOAT },
		{ 3, le::VET_FLOAT }
	};
	
	// Creating mesh descriptor and loading to gpu
	le::MeshDescriptor				meshDescriptor;
	meshDescriptor.countIndeces = vertexIndeces.size();
	meshDescriptor.countMaterials = materials.size();
	meshDescriptor.countLightmaps = 0;
	meshDescriptor.countSurfaces = meshSurfaces.size();
	meshDescriptor.sizeVerteces = verteces.size() * sizeof(MDLVertex);

	meshDescriptor.indeces = (le::UInt32_t*) vertexIndeces.data();
	meshDescriptor.materials = materials.data();
	meshDescriptor.lightmaps = nullptr;
	meshDescriptor.surfaces = meshSurfaces.data();
	meshDescriptor.verteces = verteces.data();

	meshDescriptor.min = le::Vector3D_t(minXYZ.x, minXYZ.y, minXYZ.z);
	meshDescriptor.max = le::Vector3D_t(maxXYZ.x, maxXYZ.y, maxXYZ.z);
	meshDescriptor.primitiveType = le::PT_TRIANGLES;
	meshDescriptor.countVertexElements = vertexElements.size();
	meshDescriptor.vertexElements = vertexElements.data();
	
	qDebug() << "Created mesh desctiptor";

	// Loading mesh to gpu
	mesh->Create( meshDescriptor );
	mesh->IncrementReference();
	if ( !mesh->IsCreated() )
	{
		EngineAPI::GetInstance()->GetStudioRender()->GetFactory()->Delete( mesh );
		return false;
	}

	path = Path.toStdString();

	qDebug() << "End Load";
	return true;
}

//-----------------------------------------------------------------
// Load material
//-----------------------------------------------------------------
bool Mesh::LoadMaterial( const QString& Path, le::UInt32_t Index )
{
	le::IMaterial* material = EngineAPI::GetInstance()->GetResourceSystem()->LoadMaterial( Path.toStdString().c_str(), Path.toStdString().c_str() );

	if ( !material )	return false;

	mesh->Update( &material, 1, Index );
	mdlDoc.SetMaterial( Path.toStdString(), Index );
	
	return true;
}

//-----------------------------------------------------------------
// Save model
//-----------------------------------------------------------------
bool Mesh::Save()
{
	if( mdlDoc.Save( path ) )
		return true;
	
	return false;
}

//-----------------------------------------------------------------
// Save model as
//-----------------------------------------------------------------
bool Mesh::SaveAs( const QString& Path )
{
	if ( mdlDoc.Save( Path.toStdString() ) )
	{
		path = Path.toStdString();
		return true;
	}

	return false;
}

void Mesh::Clear()
{
	if ( mesh->GetCountReferences() <= 0 )
	{
		mesh->Release();
		path = "";
		mdlDoc.Clear();
		EngineAPI::GetInstance()->GetResourceSystem()->UnloadAll();
	}
	else
	{
		mesh->DecrementReference();
		path = "";
		mdlDoc.Clear();
		EngineAPI::GetInstance()->GetResourceSystem()->UnloadAll();
	}
}

le::IMesh* Mesh::GetMesh()
{
	if ( !mesh ) return nullptr;

	qDebug() << "Mesh isn`t nullptr";
	return mesh;
}

std::vector<std::string> Mesh::GetMaterialPaths()
{
	if(mdlDoc.GetCountMaterials() == 0) return std::vector<std::string>();

	return mdlDoc.GetMaterials();
}

Mesh::Mesh()
{
	mesh = nullptr;
}

Mesh::~Mesh()
{}