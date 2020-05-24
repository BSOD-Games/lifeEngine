//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "mathlib/gtc/type_ptr.hpp"

#include "engine/lifeengine.h"
#include "engine/iconsolesystem.h"

#include "global.h"
#include "gpuprogram.h"
#include "openglstate.h"

// ------------------------------------------------------------------------------------ //
// Вставить дефайны в код шейдера
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
// Скомпилировать шейдер
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile( const ShaderDescriptor& ShaderDescriptor, UInt32_t CountDefines, const char** Defines )
{
	std::string				defineCode;
	std::string				codeShader;

	if ( CountDefines > 0 && Defines )
		for ( UInt32_t index = 0; index < CountDefines; ++index )
			defineCode += "#define " + std::string( Defines[ index ] ) + "\n";

	// Если шейдер ранее был создан - удаляем
	if ( programID > 0 )		Clear();

	try
	{
		// Компилируем вершиный шейдер
		if ( ShaderDescriptor.vertexShaderSource && ShaderDescriptor.vertexShaderSource != "" )
		{
			codeShader = ShaderDescriptor.vertexShaderSource;
			InstertDefinesToShaderCode( codeShader, defineCode );

			if ( !Compile_VertexShader( codeShader.c_str() ) )	throw;
		}

		// Компилируем геометрический шейдер
		if ( ShaderDescriptor.geometryShaderSource && ShaderDescriptor.geometryShaderSource != "" )
		{
			codeShader = ShaderDescriptor.geometryShaderSource;
			InstertDefinesToShaderCode( codeShader, defineCode );

			if ( !Compile_GeometryShader( codeShader.c_str() ) )		throw;
		}

		// Компилируем фрагментный шейдер
		if ( ShaderDescriptor.fragmentShaderSource && ShaderDescriptor.fragmentShaderSource != "" )
		{
			codeShader = ShaderDescriptor.fragmentShaderSource;
			InstertDefinesToShaderCode( codeShader, defineCode );

			if ( !Compile_FragmentShader( codeShader.c_str() ) )		throw;
		}

		// Линкуем шейдер
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
// Активировать шейдер
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::Bind()
{
	if ( programID == 0 ) return;
	OpenGLState::SetGPUProgram( this );
}

// ------------------------------------------------------------------------------------ //
// Деактивировать шейдер
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::Unbind()
{
	if ( programID == 0 ) return;
	OpenGLState::SetGPUProgram( nullptr );
}

// ------------------------------------------------------------------------------------ //
// Удалить шейдер
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
// Задать юниформ-переменную
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, int Value )
{
	if ( programID == 0 ) return;
	glUniform1i( GetUniformLocation( Name ), Value );
}

// ------------------------------------------------------------------------------------ //
// Задать юниформ-переменную
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, float Value )
{
	if ( programID == 0 ) return;
	glUniform1f( GetUniformLocation( Name ), Value );
}

// ------------------------------------------------------------------------------------ //
// Задать юниформ-переменную
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, bool Value )
{
	if ( programID == 0 ) return;
	glUniform1i( GetUniformLocation( Name ), ( int ) Value );
}

// ------------------------------------------------------------------------------------ //
// Задать юниформ-переменную
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Vector2D_t& Value )
{
	if ( programID == 0 ) return;
	glUniform2f( GetUniformLocation( Name ), Value.x, Value.y );
}

// ------------------------------------------------------------------------------------ //
// Задать юниформ-переменную
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Vector3D_t& Value )
{
	if ( programID == 0 ) return;
	glUniform3f( GetUniformLocation( Name ), Value.x, Value.y, Value.z );
}

// ------------------------------------------------------------------------------------ //
// Задать юниформ-переменную
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Vector4D_t& Value )
{
	if ( programID == 0 ) return;
	glUniform4f( GetUniformLocation( Name ), Value.x, Value.y, Value.z, Value.w );
}

// ------------------------------------------------------------------------------------ //
// Задать юниформ-переменную
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::SetUniform( const char* Name, const Matrix4x4_t& Value )
{
	if ( programID == 0 ) return;
	glUniformMatrix4fv( GetUniformLocation( Name ), 1, GL_FALSE, glm::value_ptr( Value ) );
}

// ------------------------------------------------------------------------------------ //
// Скомпилирован ли шейдер
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::IsCompile() const
{
	return programID > 0;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::GPUProgram::GPUProgram() :
	vertexShaderID( 0 ),
	geometryShaderID( 0 ),
	fragmentShaderID( 0 ),
    programID( 0 ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::GPUProgram::~GPUProgram()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Скомпилировать вершиный шейдер
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile_VertexShader( const char* Code )
{
	vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShaderID, 1, &Code, NULL );
	glCompileShader( vertexShaderID );

	// Проверяем результат компиляции	
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
// Скомпилировать геометрический шейдер
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile_GeometryShader( const char* Code )
{
	geometryShaderID = glCreateShader( GL_GEOMETRY_SHADER );
	glShaderSource( geometryShaderID, 1, &Code, NULL );
	glCompileShader( geometryShaderID );

	// Проверяем результат компиляции	
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
// Скомпилировать фрагментный шейдер
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Compile_FragmentShader( const char* Code )
{
	int					errorCompilation = 0;

	fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderID, 1, &Code, NULL );
	glCompileShader( fragmentShaderID );

	// Проверяем результат компиляции	
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
// Слинковать шейдер
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::Link()
{
	programID = glCreateProgram();

	if ( vertexShaderID )		glAttachShader( programID, vertexShaderID );
	if ( geometryShaderID )		glAttachShader( programID, geometryShaderID );
	if ( fragmentShaderID )		glAttachShader( programID, fragmentShaderID );

	// Линкуем программу и проверяем на ошибки
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
// Существует ли юниформ-переменная
// ------------------------------------------------------------------------------------ //
bool le::GPUProgram::IsUniformExists( const char* Name )
{
	// Провиряем кеш юниформов
	auto		it = uniforms.find( Name );

	// Нашли в кеше - возвращаем true если позиция больше -1
	if ( it != uniforms.end() )
		return it->second > -1;
	else
	{
		// В кеше не нашли, то тогда запрашиваем позицию в OpenGL'е
		int			location = glGetUniformLocation( programID, Name );
		uniforms.insert( std::make_pair( Name, location ) );

		return location > -1;
	}
}

// ------------------------------------------------------------------------------------ //
// Получить расположение юниформ-переменной
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::GPUProgram::GetUniformLocation( const char* Name )
{
	// Провиряем кеш юниформов
	auto		it = uniforms.find( Name );

	// Нашли в кеше - возвращаем позицию с него
	if ( it != uniforms.end() )
		return it->second;
	else
	{
		// В кеше не нашли, то тогда запрашиваем позицию в OpenGL'е
		int			location = glGetUniformLocation( programID, Name );
		uniforms.insert( std::make_pair( Name, location ) );

		if ( location == -1 )	g_consoleSystem->PrintError( "Uniform [%s] not found in shader", Name );
		return location;
	}
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::GPUProgram::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::GPUProgram::GetCountReferences() const
{
    return countReferences;
}
