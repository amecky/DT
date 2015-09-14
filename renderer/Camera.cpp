#include "Camera.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Camera::Camera() {
    m_maxPitch        = D3DXToRadian( 89.0f );
    m_maxVelocity     = 1.0f;
    m_invertY         = FALSE;
    m_enableYMovement = TRUE;
    m_position        = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    m_velocity        = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );  
    m_look            = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    CreateProjectionMatrix( 800.0f,600.0f,D3DX_PI / 3.0f, 1.3f, 0.1f, 1000.0f );
    Update();
} 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Creates the projection matrix.
Parameters:
[in] fov - Field of view
[in] aspect - Aspect ratio
[in] near - Near plane
[in] far - Far plane
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::CreateProjectionMatrix(float screenWidth, float screenHeight, float fov, float aspect, float nearPlane, float farPlane) {
    m_fov       = fov;
    m_aspect    = screenWidth / screenHeight;
    m_nearPlane = nearPlane;
    m_farPlane  = farPlane;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
    D3DXMatrixPerspectiveFovLH( &m_projection, m_fov, m_aspect, m_nearPlane, m_farPlane );

	//D3DXMatrixOrthoLH(&m_projection,_screenWidth, _screenHeight, nearPlane, farPlane);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Moves the camera forward and backward
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::MoveForward( float units )
{
    if ( m_enableYMovement )
    {
       m_velocity += m_look * units;
    }
    else
    {
        D3DXVECTOR3 moveVector( m_look.x, 0.0f, m_look.z );
        D3DXVec3Normalize( &moveVector, &moveVector );
        moveVector *= units;
        m_velocity += moveVector;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Moves the camera left and right
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Strafe( float units )
{
    m_velocity += m_right * units;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Moves the camera up and down
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::MoveUp( float units )
{
    if ( m_enableYMovement )
    {
        m_velocity.y += units;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Yaw the camera around its Y-axis.
Parameters:
[in] radians - Radians to yaw.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Yaw( float radians )
{
    if ( radians == 0.0f )
    {
        return;
    }
    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis( &rotation, &m_up, radians );
    D3DXVec3TransformNormal( &m_right, &m_right, &rotation );
    D3DXVec3TransformNormal( &m_look, &m_look, &rotation );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Pitch the camera around its X-axis.
Parameters:
[in] radians - Radians to pitch.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Pitch( float radians )
{
    if ( radians == 0.0f )
    {
        return;
    }

    radians = (m_invertY) ? -radians : radians;
    m_pitch -= radians;
    if ( m_pitch > m_maxPitch )
    {
        radians += m_pitch - m_maxPitch;
    }
    else if ( m_pitch < -m_maxPitch )
    {
        radians += m_pitch + m_maxPitch;
    }

    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis( &rotation, &m_right, radians );
    D3DXVec3TransformNormal( &m_up, &m_up, &rotation );
    D3DXVec3TransformNormal( &m_look, &m_look, &rotation );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Roll the camera around its Z-axis.
Parameters:
[in] radians - Radians to roll.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Roll( float radians )
{
    if ( radians == 0.0f )
    {
        return;
    }
    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis( &rotation, &m_look, radians );
    D3DXVec3TransformNormal( &m_right, &m_right, &rotation );
    D3DXVec3TransformNormal( &m_up, &m_up, &rotation );
}

void Camera::tick() {
	if ( GetAsyncKeyState('A') != 0 ) {
		Strafe(-0.1f);
	}
	if ( GetAsyncKeyState('D') != 0 ) {
		Strafe(0.1f);
	}
	if ( GetAsyncKeyState('W') != 0 ) {
		MoveForward(0.1f);
	}
	if ( GetAsyncKeyState('S') != 0 ) {
		MoveForward(-0.1f);
	}
	if ( GetAsyncKeyState('Q') != 0 ) {
		MoveUp(0.1f);
	}
	if ( GetAsyncKeyState('Y') != 0 ) {
		MoveUp(-0.1f);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Updates the camera and creates a new view matrix.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Update() {
    // Cap velocity to max velocity
    if ( D3DXVec3Length( &m_velocity ) > m_maxVelocity ) {
        m_velocity = *(D3DXVec3Normalize( &m_velocity, &m_velocity )) * m_maxVelocity;
    }

    // Move the camera
    m_position += m_velocity;
    // Could decelerate here. I'll just stop completely.
    m_velocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    m_lookAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );//m_position + m_look;
    
    // Calculate the new view matrix
    D3DXVECTOR3 up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &m_view, &m_position, &m_lookAt, &up );

    // Set the camera axes from the view matrix
    m_right.x = m_view._11;  
    m_right.y = m_view._21;  
    m_right.z = m_view._31;  
    m_up.x = m_view._12;
    m_up.y = m_view._22;
    m_up.z = m_view._32;
    m_look.x = m_view._13;
    m_look.y = m_view._23;
    m_look.z = m_view._33;

    // Calculate yaw and pitch
    float lookLengthOnXZ = sqrtf( m_look.z * m_look.z + m_look.x * m_look.x );
    m_pitch = atan2f( m_look.y, lookLengthOnXZ );
    m_yaw   = atan2f( m_look.x, m_look.z );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Updates the camera and creates a new view matrix.
Parameters:
[in] pPosition - New position
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::setPosition(float x,float y,float z) {
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Updates the camera and creates a new view matrix.
Parameters:
[in] pPosition - New target
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::SetLookAt( D3DXVECTOR3* pLookAt )
{
    m_lookAt.x = pLookAt->x;
    m_lookAt.y = pLookAt->y;
    m_lookAt.z = pLookAt->z;
    D3DXVec3Normalize( &m_look, &(m_lookAt - m_position) );
}