#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__

#include "Module.h"
#include "../Libraries/MathGeoLib/MathGeoLib.h"

class ModuleCamera : public Module {
public:
	ModuleCamera(){}
	~ModuleCamera(){}

	bool Init() override;
	UpdateStatus Update() override;

public:
	math::float4x4 GetModelMatrix() const;
	math::float4x4 GetViewMatrix() const;
	math::float4x4 GetProjectionMatrix() const;

	void SetFOV(float verticalFOV);
	void SetAspectRatio(float ratio);

	math::float4x4 LookAt(math::float3 eye, math::float3 target, math::float3 up) const;
	void CalculateMatrixes();
	void ResetCamera();

public:
	Frustum frustum;
	float movSpeed;
	float rotSpeed;
	float zoomSpeed;
	float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

private:
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 proj;

	float speedScale;
	math::float3 rotAngle = math::float3::zero;
};

#endif // __MODULECAMERA_H__