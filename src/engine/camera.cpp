//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/ray.h"
#include "engine/camera.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать матрицу проекции
// ------------------------------------------------------------------------------------ //
void le::Camera::InitProjection_Perspective( float FOV, float Aspect, float Near, float Far )
{
	matrixProjection = glm::perspective( glm::radians( FOV ), Aspect, Near, Far );
	near = Near;
	far = Far;
}

// ------------------------------------------------------------------------------------ //
// Инициализировать матрицу проекции
// ------------------------------------------------------------------------------------ //
void le::Camera::InitProjection_Ortho( float Left, float Right, float Bottom, float Top, float Near, float Far )
{
	matrixProjection = glm::ortho( Left, Right, Bottom, Top, Near, Far );
	near = Near;
	far = Far;
}

// ------------------------------------------------------------------------------------ //
// Сместить камеру
// ------------------------------------------------------------------------------------ //
void le::Camera::Move( const Vector3D_t& FactorMove )
{
	position += FactorMove;
	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Сместить камеру
// ------------------------------------------------------------------------------------ //
void le::Camera::Move( CAMERA_SIDE_MOVE SideMove, float MoveSpeed )
{
	switch ( SideMove )
	{
	case CSM_FORWARD:
		position += targetDirection * MoveSpeed;
		break;

	case CSM_BACKWARD:
		position -= targetDirection * MoveSpeed;
		break;

	case CSM_LEFT:
		position -= right * MoveSpeed;
		break;

	case CSM_RIGHT:
		position += right * MoveSpeed;
		break;
	}

	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Повернуть камеру
// ------------------------------------------------------------------------------------ //
void le::Camera::Rotate( const Vector3D_t& FactorRotate )
{
	// 6.28319 радиан = 360 градусов

	if ( FactorRotate.z != 0.f )
	{
		eulerRotation.z += glm::radians( FactorRotate.z );
		if ( eulerRotation.z < -6.28319f || eulerRotation.z > 6.28319f )
			eulerRotation.z = 0.f;
	}

	if ( FactorRotate.x != 0.f )
	{
		eulerRotation.x += glm::radians( FactorRotate.x );
		if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )
			eulerRotation.x = 0.f;
	}

	if ( FactorRotate.y != 0.f )
	{
		eulerRotation.y += glm::radians( FactorRotate.y );
		if ( eulerRotation.y < -6.28319f || eulerRotation.y > 6.28319f )
			eulerRotation.y = 0.f;
	}

	quatRotation =
		glm::angleAxis( eulerRotation.z, glm::vec3( 0.f, 0.f, 1.f ) ) *
		glm::angleAxis( eulerRotation.x, glm::vec3( 1.f, 0.f, 0.f ) ) *
		glm::angleAxis( eulerRotation.y, glm::vec3( 0.f, 1.f, 0.f ) );

	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Повернуть камеру мышкой
// ------------------------------------------------------------------------------------ //
void le::Camera::RotateByMouse( const Vector2D_t& MouseOffset, float MouseSensitivit, bool ConstrainYaw )
{   
	// 1.5708 радиан = 90 градусов
	// 6.28319 радиан = 360 градусов

	if ( MouseOffset.x != 0 )
	{
		eulerRotation.y += glm::radians( MouseOffset.x * MouseSensitivit );

		if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )
			eulerRotation.x = 0.f;
	}

	if ( MouseOffset.y != 0 )
	{
		eulerRotation.x += glm::radians( MouseOffset.y * MouseSensitivit );

		if ( ConstrainYaw )
		{
			if ( eulerRotation.x > 1.5708f )
				eulerRotation.x = 1.5708f;

			if ( eulerRotation.x < -1.5708f )
				eulerRotation.x = -1.5708f;
		}
		else if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )
			eulerRotation.x = 0.f;
	}

	quatRotation =
		glm::angleAxis( eulerRotation.z, glm::vec3( 0.f, 0.f, 1.f ) ) *
		glm::angleAxis( eulerRotation.x, glm::vec3( 1.f, 0.f, 0.f ) ) *
		glm::angleAxis( eulerRotation.y, glm::vec3( 0.f, 1.f, 0.f ) );

	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Получить точку в мире по экранным координатам
// ------------------------------------------------------------------------------------ //
le::Ray le::Camera::ScreenToWorld( const Vector2D_t& Coords, const Vector2D_t & ViewportSize )
{
	Vector3D_t      farPlane	= glm::unProject( Vector3D_t( Coords.x, ViewportSize.y - Coords.y, 1.f ), matrixView, matrixProjection, Vector4D_t( 0, 0, ViewportSize.x, ViewportSize.y ) );
	Vector3D_t		nearPlane	= glm::unProject( Vector3D_t( Coords.x, ViewportSize.y - Coords.y, 0.f ), matrixView, matrixProjection, Vector4D_t( 0, 0, ViewportSize.x, ViewportSize.y ) );

	return Ray( position, glm::normalize( farPlane - position ) );
}

// ------------------------------------------------------------------------------------ //
// Перевести точку из мирового пространства в экранные
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::Camera::WorldToScreen( const Vector3D_t& Coords, const Vector2D_t& ViewportSize )
{
	return glm::project( Coords, matrixView, matrixProjection, Vector4D_t( 0, 0, ViewportSize.x, ViewportSize.y ) );
}

// ------------------------------------------------------------------------------------ //
// Задать позицию камеры
// ------------------------------------------------------------------------------------ //
void le::Camera::SetPosition( const Vector3D_t& Position )
{
	position = Position;
	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Задать поворот камеры
// ------------------------------------------------------------------------------------ //
void le::Camera::SetRotation( const Vector3D_t& Rotation )
{    
	// 6.28319 радиан = 360 градусов

	eulerRotation = Rotation;

	if ( eulerRotation.z < -6.28319f || eulerRotation.z > 6.28319f )
		eulerRotation.z = 0.f;

	if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )
		eulerRotation.x = 0.f;

	if ( eulerRotation.y < -6.28319f || eulerRotation.y > 6.28319f )
		eulerRotation.y = 0.f;

	quatRotation =
		glm::angleAxis( eulerRotation.z, glm::vec3( 0.f, 0.f, 1.f ) ) *
		glm::angleAxis( eulerRotation.x, glm::vec3( 1.f, 0.f, 0.f ) ) *
		glm::angleAxis( eulerRotation.y, glm::vec3( 0.f, 1.f, 0.f ) );

	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Задать вектор взгляда
// ------------------------------------------------------------------------------------ //
void le::Camera::SetTargetDirection( const Vector3D_t& TargetDirection )
{
	localTargetDirection = TargetDirection;
	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Задать ввектор вверх
// ------------------------------------------------------------------------------------ //
void le::Camera::SetUp( const Vector3D_t& Up )
{
	up = Up;
	isNeedUpdate = true;
}

// ------------------------------------------------------------------------------------ //
// Попал ли паралеллепипед в фокус камеры
// ------------------------------------------------------------------------------------ //
bool le::Camera::IsVisible( const Vector3D_t& MinPosition, const Vector3D_t& MaxPosition )
{
	if ( isNeedUpdate )			Update();
	return frustum.IsVisible( MinPosition, MaxPosition );
}

// ------------------------------------------------------------------------------------ //
// Попал ли паралеллепипед в фокус камеры
// ------------------------------------------------------------------------------------ //
bool le::Camera::IsVisible( const Vector3DInt_t& MinPosition, const Vector3DInt_t& MaxPosition )
{
	if ( isNeedUpdate )			Update();
	return frustum.IsVisible( MinPosition, MaxPosition );
}

// ------------------------------------------------------------------------------------ //
// Попала ли сфера в фокус камеры
// ------------------------------------------------------------------------------------ //
bool le::Camera::IsVisible( const Vector3DInt_t& Position, float Radius )
{
	if ( isNeedUpdate )			Update();
	return frustum.IsVisible( Position, Radius );
}

// ------------------------------------------------------------------------------------ //
// Получить ближнию плоскость отсечения
// ------------------------------------------------------------------------------------ //
float le::Camera::GetNear() const
{
	return near;
}

// ------------------------------------------------------------------------------------ //
// Получить дальнию плоскость отсечения
// ------------------------------------------------------------------------------------ //
float le::Camera::GetFar() const
{
	return far;
}

// ------------------------------------------------------------------------------------ //
// Получить позицию
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Camera::GetPosition() const
{
	return position;
}

// ------------------------------------------------------------------------------------ //
// Получить вектор вправо
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Camera::GetUp() const
{
	return up;
}

// ------------------------------------------------------------------------------------ //
// Получить номер сборки
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Camera::GetRight() const
{
	return right;
}

// ------------------------------------------------------------------------------------ //
// Получить поворот в кватернионе
// ------------------------------------------------------------------------------------ //
const le::Quaternion_t& le::Camera::GetQuatRotation() const
{
	return quatRotation;
}

// ------------------------------------------------------------------------------------ //
// Получить поворот в углах Эйлера
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Camera::GetEulerRotation() const
{
	return eulerRotation;
}

// ------------------------------------------------------------------------------------ //
// Получить вектор взгляда
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Camera::GetTargetDirection() const
{
	return targetDirection;
}

// ------------------------------------------------------------------------------------ //
// Получить матрицу вида
// ------------------------------------------------------------------------------------ //
const le::Matrix4x4_t& le::Camera::GetViewMatrix()
{
	if ( isNeedUpdate )		Update();
	return matrixView;
}

// ------------------------------------------------------------------------------------ //
// Получить матрицу проекции
// ------------------------------------------------------------------------------------ //
const le::Matrix4x4_t& le::Camera::GetProjectionMatrix() const
{
	return matrixProjection;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Camera::Camera() :
	isNeedUpdate( true ),
	near( 0.f ),
	far( 0.f ),
	up( 0.f, 1.f, 0.f ),
	localUp( 0.f, 1.f, 0.f ),
	position( 0.f, 0.f, 0.f ),
	targetDirection( 0.f, 0.f, -1.f ),
	localTargetDirection( 0.f, 0.f, -1.f ),
	quatRotation( 0.f, 0.f, 0.f, 0.f ),
	eulerRotation( 0.f, 0.f, 0.f ),
	matrixProjection( 1.f ),
	matrixView( 1.f )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Camera::~Camera()
{}

// ------------------------------------------------------------------------------------ //
// Обновить камеру
// ------------------------------------------------------------------------------------ //
void le::Camera::Update()
{
	if ( !isNeedUpdate )		return;

	targetDirection = localTargetDirection * quatRotation;
	up = localUp * quatRotation;
	right = glm::normalize( glm::cross( targetDirection, Vector3D_t( 0.f, 1.f, 0.f ) ) );
	matrixView = glm::lookAt( position, position + targetDirection, up );
	frustum.Update( matrixProjection, matrixView );

	isNeedUpdate = false;
}