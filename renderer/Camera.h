#pragma once
#include <d3dx9math.h>

class Camera {

public:
    Camera();
    void CreateProjectionMatrix(float screenWidth,float screenHeight,float fov, float aspect, float nearPlane, float farPlane );
	void createOrthoProjectionMatrix(float screenWidth,float screenHeight);
    void MoveForward( float units );
    void Strafe( float units ); 
	void MoveUp( float units );    
	
	void Yaw( float radians );
	void Pitch( float radians );
	void Roll( float radians );
    void Update();

    void setPosition(float x,float y,float z);
	void tick();

    void SetLookAt( D3DXVECTOR3* pLookAt );
    void SetFOV( float fov )            { CreateProjectionMatrix( _screenWidth, _screenHeight, fov, m_aspect, m_nearPlane, m_farPlane ); }
	void SetAspectRatio(float aspect) { CreateProjectionMatrix(_screenWidth, _screenHeight, m_fov, aspect, m_nearPlane, m_farPlane); }
	void SetNearPlane(float plane)    { CreateProjectionMatrix(_screenWidth, _screenHeight, m_fov, m_aspect, plane, m_farPlane); }
	void SetFarPlane(float plane)     { CreateProjectionMatrix(_screenWidth, _screenHeight, m_fov, m_aspect, m_nearPlane, plane); }
    void SetMaxVelocity( float maxVelocity ) { m_maxVelocity = maxVelocity; }
    void SetInvertY( BOOL invert )           { m_invertY = invert; }
    void SetMaxPitch( float maxPitch )       { m_maxPitch = maxPitch; }

    const D3DXMATRIX& GetViewMatrix() const { 
		return m_view; 
	}
    const D3DXMATRIX& GetProjectionMatrix() const { 
		return m_projection; 
	}
    D3DXVECTOR3* GetPosition()         { return &m_position; }
    D3DXVECTOR3* GetLookAt()           { return &m_lookAt; }
    float GetFOV()                     { return m_fov; }
    float GetAspectRatio()             { return m_aspect; }
    float GetNearPlane()               { return m_nearPlane; }
    float GetFarPlane()                { return m_farPlane; }
    float GetMaxVelocity()             { return m_maxVelocity; }
    BOOL  GetInvertY()                 { return m_invertY; }
    float GetPitch()                   { return m_pitch; }
    float GetYaw()                     { return m_yaw; }
    float GetMaxPitch()                { return m_maxPitch; }

private:
    D3DXMATRIX  m_view;      
    D3DXMATRIX  m_projection;
    D3DXVECTOR3 m_right;     
    D3DXVECTOR3 m_up;        
    D3DXVECTOR3 m_look;      
    D3DXVECTOR3 m_position;  
    D3DXVECTOR3 m_lookAt;    
    D3DXVECTOR3 m_velocity;  
    float       m_yaw;       
    float       m_pitch;     
    float       m_maxPitch;
    float       m_maxVelocity;
    float       m_fov;       
    float       m_aspect;    
    float       m_nearPlane; 
    float       m_farPlane;  
    BOOL        m_invertY;
    BOOL        m_enableYMovement;

	float _screenWidth;
	float _screenHeight;
};

