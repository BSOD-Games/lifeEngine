//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <vector>

#include "engine/materialproxyvarinfo.h"
#include "engine/imaterialproxyvar.h"
#include "engine/consolesystem.h"
#include "engine/ishaderparameter.h"

#include "global.h"
#include "sineproxy.h"

// ------------------------------------------------------------------------------------ //
// Set variable
// ------------------------------------------------------------------------------------ //
void le::SineProxy::SetVar( IMaterialProxyVar* MaterialProxyVar )
{
    if ( strcmp( MaterialProxyVar->GetName(), "sinemin" ) == 0 )
    {   
        MATERIAL_PROXY_VAR_TYPE     type = MaterialProxyVar->GetType();
        if ( type != MPVT_FLOAT && type != MPVT_INT )
        {
            g_consoleSystem->PrintError( "In variable [sinemin] mast be type int or float" );
            return;
        }

        sinemin = MaterialProxyVar;
        sinemin->IncrementReference();

        if ( type == MPVT_FLOAT )
            currentSine = sinemin->GetValueFloat();
        else 
            currentSine = sinemin->GetValueInt();
    }

    else if ( strcmp( MaterialProxyVar->GetName(), "sinemax" ) == 0 )
    {
        MATERIAL_PROXY_VAR_TYPE     type = MaterialProxyVar->GetType();
        if ( type != MPVT_FLOAT && type != MPVT_INT )
        {
            g_consoleSystem->PrintError( "In variable [sinemin] mast be type int or float" );
            return;
        }

        sinemax = MaterialProxyVar;
        sinemax->IncrementReference();
    }

    else if ( strcmp( MaterialProxyVar->GetName(), "sineoffset" ) == 0 )
    {
        MATERIAL_PROXY_VAR_TYPE     type = MaterialProxyVar->GetType();
        if ( type != MPVT_FLOAT && type != MPVT_INT )
        {
            g_consoleSystem->PrintError( "In variable [sineoffset] mast be type int or float" );
            return;
        }

        sineoffset = MaterialProxyVar;
        sineoffset->IncrementReference();
    }

    else if ( strcmp( MaterialProxyVar->GetName(), "resultVar" ) == 0 )
    {
        if ( MaterialProxyVar->GetType() != MPVT_SHADER_PARAMETER )
        {
            g_consoleSystem->PrintError( "In variable [resultVar] mast be type shader parameter" );
            return;
        }

        resultVar = MaterialProxyVar;
        resultVar->IncrementReference();
    }

    else if ( strcmp( MaterialProxyVar->GetName(), "changeComponent" ) == 0 )
    {
        if ( MaterialProxyVar->GetType() != MPVT_INT )
        {
            g_consoleSystem->PrintError( "In variable [changeComponent] mast be type int" );
            return;
        }

        changeComponent = MaterialProxyVar;
        changeComponent->IncrementReference();
    }

    else
        g_consoleSystem->PrintError( "Variable [%s] not used in proxy-material le::SineProxy", MaterialProxyVar->GetName() );

    if ( sinemin && sinemax && sineoffset && resultVar )
        isInitialized = true;
}

// ------------------------------------------------------------------------------------ //
// Is nead update
// ------------------------------------------------------------------------------------ //
bool le::SineProxy::IsNeadUpdate() const
{
    return isNeadUpdate;
}

// ------------------------------------------------------------------------------------ //
// Clear variable
// ------------------------------------------------------------------------------------ //
void le::SineProxy::ClearVar( const char* NameVar )
{
    if ( strcmp( NameVar, "sinemin" ) == 0 && sinemin )
    {
        if ( sinemin->GetCountReferences() <= 0 )
            sinemin->Release();
        else
            sinemin->DecrementReference();

        sinemin = nullptr;
    }
    else if ( strcmp( NameVar, "sinemax" ) == 0 && sinemax )
    {
        if ( sinemax->GetCountReferences() <= 0 )
            sinemax->Release();
        else
            sinemax->DecrementReference();

        sinemax = nullptr;
    }
    else if ( strcmp( NameVar, "sineoffset" ) == 0 && sineoffset )
    {
        if ( sineoffset->GetCountReferences() <= 0 )
            sineoffset->Release();
        else
            sineoffset->DecrementReference();

        sineoffset = nullptr;
    }
    else if ( strcmp( NameVar, "resultVar" ) == 0 && resultVar )
    {
        if ( resultVar->GetCountReferences() <= 0 )
            resultVar->Release();
        else
            resultVar->DecrementReference();

        resultVar = nullptr;
    }
    else if ( strcmp( NameVar, "changeComponent" ) == 0 && changeComponent )
    {
        if ( changeComponent->GetCountReferences() <= 0 )
            changeComponent->Release();
        else
            changeComponent->DecrementReference();

        changeComponent = nullptr;
    }
    else return;

    if ( !sinemin || !sinemax || !sineoffset || !resultVar )
        isInitialized = false;
}

// ------------------------------------------------------------------------------------ //
// Clear all variables
// ------------------------------------------------------------------------------------ //
void le::SineProxy::ClearAllVars()
{
    if ( sinemin )
    {
        if ( sinemin->GetCountReferences() <= 0 )
            sinemin->Release();
        else
            sinemin->DecrementReference();

        sinemin = nullptr;
    }

    if ( sinemax )
    {
        if ( sinemax->GetCountReferences() <= 0 )
            sinemax->Release();
        else
            sinemax->DecrementReference();

        sinemax = nullptr;
    }

    if ( sineoffset )
    {
        if ( sineoffset->GetCountReferences() <= 0 )
            sineoffset->Release();
        else
            sineoffset->DecrementReference();

        sineoffset = nullptr;
    }

    if ( resultVar )
    {
        if ( resultVar->GetCountReferences() <= 0 )
            resultVar->Release();
        else
            resultVar->DecrementReference();

        resultVar = nullptr;
    }

    if ( changeComponent )
    {
        if ( changeComponent->GetCountReferences() <= 0 )
            changeComponent->Release();
        else
            changeComponent->DecrementReference();

        changeComponent = nullptr;
    }

    isInitialized = false;
}

// ------------------------------------------------------------------------------------ //
// Update animation
// ------------------------------------------------------------------------------------ //
void le::SineProxy::Update()
{
    if ( !isInitialized || !resultVar->IsDefined() )
        return;

    float       sinemax = this->sinemax->GetType() == MPVT_FLOAT ? this->sinemax->GetValueFloat() : this->sinemax->GetValueInt();
    if ( currentSine > sinemax )        currentSine = sinemin->GetType() == MPVT_FLOAT ? sinemin->GetValueFloat() : sinemin->GetValueInt();

    IShaderParameter*       resultVar = this->resultVar->GetValueShaderParameter();

    switch ( resultVar->GetType() )
    {
    case SPT_COLOR:
    {   
        Color_t                 color = resultVar->GetValueColor();
        
        if ( changeComponent )
        {
            switch ( changeComponent->GetValueInt() )
            {
            case 0:        color.r = sin( currentSine );    break;
            case 1:        color.g = sin( currentSine );    break;
            case 2:        color.b = sin( currentSine );    break;

            default:
            case 3:        color.a = sin( currentSine );    break;
            }
        }
        else
        {
            color.r = sin( currentSine );
            color.g = sin( currentSine );
            color.b = sin( currentSine );
            color.a = sin( currentSine );
        }

        resultVar->SetValueColor( color );
        break;
    }
    }

    currentSine += sineoffset->GetType() == MPVT_FLOAT ? sineoffset->GetValueFloat() : sineoffset->GetValueInt();
}

// ------------------------------------------------------------------------------------ //
// Set nead update proxy-material
// ------------------------------------------------------------------------------------ //
void le::SineProxy::NeadUpdate()
{
    isNeadUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Get name proxy-material
// ------------------------------------------------------------------------------------ //
const char* le::SineProxy::GetName() const
{
    return "Sine";
}

// ------------------------------------------------------------------------------------ //
// Get variable
// ------------------------------------------------------------------------------------ //
le::IMaterialProxyVar* le::SineProxy::GetVar( const char* NameVar ) const
{
    if ( strcmp( NameVar, "sinemin" ) )                 return sinemin;
    else if ( strcmp( NameVar, "sinemax" ) )            return sinemax;
    else if ( strcmp( NameVar, "sineoffset" ) )         return sineoffset;
    else if ( strcmp( NameVar, "resultVar" ) )          return resultVar;
    else if ( strcmp( NameVar, "changeComponent" ) )    return changeComponent;

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::SineProxy::SineProxy() :
    sinemin( nullptr ),
    sinemax( nullptr ),
    sineoffset( nullptr ),
    resultVar( nullptr ),
    changeComponent( nullptr ),
    isInitialized( false ),
    isNeadUpdate( false ),
    currentSine( 0.f ),
    countReference( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::SineProxy::~SineProxy()
{
    ClearAllVars();
}

// ------------------------------------------------------------------------------------ //
// Material proxy descriptor
// ------------------------------------------------------------------------------------ //
le::MaterialProxyDescriptor le::SineProxy::GetDescriptor()
{
	static std::vector< MaterialProxyVarInfo >			parametersInfo =
	{
		{ "sinemin", MPVT_FLOAT },
        { "sinemax", MPVT_FLOAT },
        { "sineoffset", MPVT_FLOAT },
		{ "resultVar", MPVT_SHADER_PARAMETER },
		{ "changeComponent", MPVT_INT }
	}; 

	MaterialProxyDescriptor			materialProxyDescriptor;
	materialProxyDescriptor.name = "Sine";
	materialProxyDescriptor.countParameters = parametersInfo.size();
	materialProxyDescriptor.parametersInfo = parametersInfo.data();
	materialProxyDescriptor.CreateMaterialProxyFn = []() -> IMaterialProxy* { return new SineProxy(); };
	return materialProxyDescriptor;
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::SineProxy::IncrementReference()
{
    ++countReference;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::SineProxy::DecrementReference()
{
    --countReference;
}

// ------------------------------------------------------------------------------------ //
// Delete object
// ------------------------------------------------------------------------------------ //
void le::SineProxy::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count reference
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::SineProxy::GetCountReferences() const
{
    return countReference;
}
