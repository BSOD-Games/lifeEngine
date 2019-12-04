//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "mathlib/gtc/type_ptr.hpp"

#include "engine/lifeengine.h"
#include "engine/iconsolesystem.h"
#include "gpuprogram.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// �������� ������� � ��� �������
// ------------------------------------------------------------------------------------ //
inline void InstertDefinesToShaderCode( std::string& CodeShader, const std::string& DefineCode )
{
	if ( DefineCode.empty() ) return;

	le::UInt32_t			versionIndex = CodeShader.find( "#version" );

	if ( versionIndex == -1 )
		le::g_consoleSystem->PrintWarning( "Missing #version xxx in shader" );
	else
	{
		le::UInt32_t		defineInsertPoint = CodeShader.find( "\n", versionIndex ) + 1;
		CodeShader.insert( defineInsertPoint, DefineCode );
	}
}

// ------------------------------------------------------------------------------------ //
// �������������� ������
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile( const ShaderDescriptor& ShaderDescriptor, UInt32_t CountDefines, const char** Defines )
{
	std::string				defineCode;
	std::string				codeShader;

	if ( CountDefines > 0 && Defines )
		for ( UInt32_t index = 0; index < CountDefines; ++index )
			defineCode += "#define " + std::string( Defines[ index ] ) + "\n";

	// ���� ������ ����� ��� ������ - �������
	if ( programID > 0 )		Clear();

	try
	{
		// ����������� �������� ������
		if ( ShaderDescriptor.vertexShaderSource )
		{
			codeShader = ShaderDescriptor.vertexShaderSource;
			InstertDefinesToShaderCode( codeShader, defineCode );

			if ( !Compile_VertexShader( codeShader.c_str() ) )	throw;
		}

		// ����������� �������������� ������
		if ( ShaderDescriptor.geometryShaderSource )
		{
			codeShader = ShaderDescriptor.geometryShaderSource;
			InstertDefinesToShaderCode( codeShader, defineCode );

			if ( !Compile_GeometryShader( codeShader.c_str() ) )		throw;
		}

		// ����������� ����������� ������
		if ( ShaderDescriptor.fragmentShaderSource )
		{
			codeShader = ShaderDescriptor.fragmentShaderSource;
			InstertDefinesToShaderCode( codeShader, defineCode );

			if ( !Compile_FragmentShader( codeShader.c_str() ) )		throw;
		}

		// ������� ������
		if ( !Link() )		throw;
	}
	catch ( ... )
	{
		Clear();
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// ������������ ������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::Bind()
{
	if ( programID == 0 ) return;
	glUseProgram( programID );
}

// ------------------------------------------------------------------------------------ //
// �������������� ������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::Unbind()
{
	if ( programID == 0 ) return;
	glUseProgram( 0 );
}

// ------------------------------------------------------------------------------------ //
// ������� ������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::Clear()
{
	if ( vertexShaderID != 0 )		glDeleteShader( vertexShaderID );
	if ( geometryShaderID != 0 )	glDeleteShader( geometryShaderID );
	if ( fragmentShaderID != 0 )	glDeleteShader( fragmentShaderID );
	if ( programID != 0 )			glDeleteProgram( programID );

	uniforms.clear();
}

// ------------------------------------------------------------------------------------ //
// ������ �������-����������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, int Value )
{
	if ( programID == 0 ) return;
	glUniform1i( GetUniformLocation( Name ), Value );
}

// ------------------------------------------------------------------------------------ //
// ������ �������-����������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, float Value )
{
	if ( programID == 0 ) return;
	glUniform1f( GetUniformLocation( Name ), Value );
}

// ------------------------------------------------------------------------------------ //
// ������ �������-����������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, bool Value )
{
	if ( programID == 0 ) return;
	glUniform1i( GetUniformLocation( Name ), ( int ) Value );
}

// ------------------------------------------------------------------------------------ //
// ������ �������-����������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Vector2D_t& Value )
{
	if ( programID == 0 ) return;
	glUniform2f( GetUniformLocation( Name ), Value.x, Value.y );
}

// ------------------------------------------------------------------------------------ //
// ������ �������-����������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Vector3D_t& Value )
{
	if ( programID == 0 ) return;
	glUniform3f( GetUniformLocation( Name ), Value.x, Value.y, Value.z );
}

// ------------------------------------------------------------------------------------ //
// ������ �������-����������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Vector4D_t& Value )
{
	if ( programID == 0 ) return;
	glUniform4f( GetUniformLocation( Name ), Value.x, Value.y, Value.z, Value.w );
}

// ------------------------------------------------------------------------------------ //
// ������ �������-����������
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Matrix4x4_t& Value )
{
	if ( programID == 0 ) return;
	glUniformMatrix4fv( GetUniformLocation( Name ), 1, GL_FALSE, glm::value_ptr( Value ) );
}

// ------------------------------------------------------------------------------------ //
// ������������� �� ������
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::IsCompile() const
{
	return programID > 0;
}

// ------------------------------------------------------------------------------------ //
// �����������
// ------------------------------------------------------------------------------------ //
le::GPUProgram::GPUProgram() :
	vertexShaderID( 0 ),
	geometryShaderID( 0 ),
	fragmentShaderID( 0 ),
	programID( 0 )
{}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::GPUProgram::~GPUProgram()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// �������������� �������� ������
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile_VertexShader( const char* Code )
{
	vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShaderID, 1, &Code, NULL );
	glCompileShader( vertexShaderID );

	// ��������� ��������� ����������	
	int						errorCompilation = 0;
	glGetShaderiv( vertexShaderID, GL_COMPILE_STATUS, &errorCompilation );

	if ( errorCompilation != GL_TRUE )
	{
		int				lengthLog = 0;
		char*			errorMessage;

		glGetShaderiv( vertexShaderID, GL_INFO_LOG_LENGTH, &lengthLog );
		errorMessage = new char[ lengthLog ];

		glGetShaderInfoLog( vertexShaderID, lengthLog, &lengthLog, errorMessage );
		
		g_consoleSystem->PrintError( "**** Shader error ****" );
		g_consoleSystem->PrintError( "Failed to compile vertex shader:" );
		g_consoleSystem->PrintError( errorMessage );
		g_consoleSystem->PrintError( "**** Shader error end ****" );

		delete[] errorMessage;
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// �������������� �������������� ������
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile_GeometryShader( const char* Code )
{
	geometryShaderID = glCreateShader( GL_GEOMETRY_SHADER );
	glShaderSource( geometryShaderID, 1, &Code, NULL );
	glCompileShader( geometryShaderID );

	// ��������� ��������� ����������	
	int						errorCompilation = 0;
	glGetShaderiv( geometryShaderID, GL_COMPILE_STATUS, &errorCompilation );

	if ( errorCompilation != GL_TRUE )
	{
		int					lengthLog = 0;
		char*				errorMessage;

		glGetShaderiv( geometryShaderID, GL_INFO_LOG_LENGTH, &lengthLog );
		errorMessage = new char[ lengthLog ];

		glGetShaderInfoLog( geometryShaderID, lengthLog, &lengthLog, errorMessage );

		g_consoleSystem->PrintError( "**** Shader error ****" );
		g_consoleSystem->PrintError( "Failed to compile geometry shader:" );
		g_consoleSystem->PrintError( errorMessage );
		g_consoleSystem->PrintError( "**** Shader error end ****" );

		delete[] errorMessage;
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// �������������� ����������� ������
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile_FragmentShader( const char* Code )
{
	int					errorCompilation = 0;

	fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderID, 1, &Code, NULL );
	glCompileShader( fragmentShaderID );

	// ��������� ��������� ����������	
	glGetShaderiv( fragmentShaderID, GL_COMPILE_STATUS, &errorCompilation );

	if ( errorCompilation != GL_TRUE )
	{
		int					lengthLog = 0;
		char* errorMessage;

		glGetShaderiv( fragmentShaderID, GL_INFO_LOG_LENGTH, &lengthLog );
		errorMessage = new char[ lengthLog ];

		glGetShaderInfoLog( fragmentShaderID, lengthLog, &lengthLog, errorMessage );

		g_consoleSystem->PrintError( "**** Shader error ****" );
		g_consoleSystem->PrintError( "Failed to compile fragment shader:" );
		g_consoleSystem->PrintError( errorMessage );
		g_consoleSystem->PrintError( "**** Shader error end ****" );

		delete[] errorMessage;
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// ���������� ������
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Link()
{
	programID = glCreateProgram();

	if ( vertexShaderID )		glAttachShader( programID, vertexShaderID );
	if ( geometryShaderID )		glAttachShader( programID, geometryShaderID );
	if ( fragmentShaderID )		glAttachShader( programID, fragmentShaderID );

	// ������� ��������� � ��������� �� ������
	glLinkProgram( programID );

	int			errorLink = 0;
	glGetProgramiv( programID, GL_LINK_STATUS, &errorLink );

	if ( errorLink != GL_TRUE )
	{
		int				lengthLog = 0;
		char*			errorMessage;

		glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &lengthLog );
		errorMessage = new char[ lengthLog ];

		glGetProgramInfoLog( programID, lengthLog, &lengthLog, errorMessage );

		g_consoleSystem->PrintError( "**** Shader error ****" );
		g_consoleSystem->PrintError( "Failed to link shader:" );
		g_consoleSystem->PrintError( errorMessage );
		g_consoleSystem->PrintError( "**** Shader error end ****" );

		delete[] errorMessage;
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// ���������� �� �������-����������
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::IsUniformExists( const char* Name )
{
	// ��������� ��� ���������
	auto		it = uniforms.find( Name );

	// ����� � ���� - ���������� true ���� ������� ������ -1
	if ( it != uniforms.end() )
		return it->second > -1;
	else
	{
		// � ���� �� �����, �� ����� ����������� ������� � OpenGL'�
		int			location = glGetUniformLocation( programID, Name );
		uniforms.insert( std::make_pair( Name, location ) );

		return location > -1;
	}
}

// ------------------------------------------------------------------------------------ //
// �������� ������������ �������-����������
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::GPUProgram::GetUniformLocation( const char* Name )
{
	// ��������� ��� ���������
	auto		it = uniforms.find( Name );

	// ����� � ���� - ���������� ������� � ����
	if ( it != uniforms.end() )
		return it->second;
	else
	{
		// � ���� �� �����, �� ����� ����������� ������� � OpenGL'�
		int			location = glGetUniformLocation( programID, Name );
		uniforms.insert( std::make_pair( Name, location ) );

		if ( location == -1 )	g_consoleSystem->PrintError( "Uniform [%s] not found in shader", Name );
		return location;
	}
}
