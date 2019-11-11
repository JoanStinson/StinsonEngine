#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include <SDL_events.h>

bool ModuleCamera::Init() {
	LOG("Init Module Camera\n");
	ResetCamera();
	return true;
}

UpdateStatus ModuleCamera::Update() {
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KeyState::REPEAT) {
		HandleTranslation();
		HandleRotation();
	}
	HandleZoom();
	speedScale = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::REPEAT ? 3.0F : 1.0F;
	return UpdateStatus::CONTINUE;
}

math::float4x4 ModuleCamera::GetModelMatrix() const {
	return model;
}

math::float4x4 ModuleCamera::GetViewMatrix() const {
	return view;
}

math::float4x4 ModuleCamera::GetProjectionMatrix() const {
	return proj;
}

void ModuleCamera::SetFOV(float verticalFOV) {
	frustum.verticalFov = verticalFOV;
	frustum.horizontalFov = 2.F * atanf(tanf(frustum.verticalFov * 0.5F) * aspectRatio);
	CalculateMatrixes();
}

void ModuleCamera::SetAspectRatio(float ratio) {
	aspectRatio = ratio;
	frustum.horizontalFov = 2.F * atanf(tanf(frustum.verticalFov * 0.5F) * aspectRatio);
	CalculateMatrixes();
}

math::float4x4 ModuleCamera::LookAt(math::float3 eye, math::float3 target, math::float3 up) const {
	math::float4x4 matrix;
	math::float3 f(target - eye); f.Normalize();
	math::float3 s(f.Cross(up)); s.Normalize();
	math::float3 u(s.Cross(f));

	matrix[0][0] = s.x; matrix[0][1] = s.y; matrix[0][2] = s.z;
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z;
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye); matrix[1][3] = -u.Dot(eye); matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0F; matrix[3][1] = 0.0F; matrix[3][2] = 0.0F; matrix[3][3] = 1.0F;

	return matrix;
}

void ModuleCamera::CalculateMatrixes() {
	//TODO cambiar rotAngle per matriu de rotacio
	// math::float4x4 = math::floatRotateX(x) * math::floatRotateY(x) * math::floatRotateZ(x)
	model = math::float4x4::FromTRS(frustum.pos, math::float3x3::RotateY(math::pi / 4.0F), float3::one);
	view = LookAt(math::float3(0.0F, 1.F, 4.0F), rotAngle, math::float3::unitY); 
	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::ResetCamera() {
	movSpeed = 0.2F;
	rotSpeed = 0.4F;
	zoomSpeed = 0.4F;
	speedScale = 1.0F;
	aspectRatio = (float)App->window->GetWindowWidth() / (float)App->window->GetWindowHeight();
	rotAngle = math::float3::zero;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = math::float3::zero;
	frustum.front = -math::float3::unitZ;
	frustum.up = math::float3::unitY;
	frustum.nearPlaneDistance = 0.1F;
	frustum.farPlaneDistance = 100.0F;
	frustum.verticalFov = math::pi / 4.0F;
	frustum.horizontalFov = 2.F * atanf(tanf(frustum.verticalFov * 0.5F) * aspectRatio);

	CalculateMatrixes();
}

void ModuleCamera::HandleTranslation() {
	// Forward
	if (App->input->GetKey(SDL_SCANCODE_W) == KeyState::REPEAT) {
		frustum.Translate(math::float3(0.0F, 0.0F, movSpeed * speedScale));
		CalculateMatrixes();
	}
	// Backward
	if (App->input->GetKey(SDL_SCANCODE_S) == KeyState::REPEAT) {
		frustum.Translate(math::float3(0.0F, 0.0F, -(movSpeed * speedScale)));
		CalculateMatrixes();
	}
	// Left
	if (App->input->GetKey(SDL_SCANCODE_A) == KeyState::REPEAT) {
		frustum.Translate(math::float3(movSpeed * speedScale, 0.0F, 0.0F));
		CalculateMatrixes();
	}
	// Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KeyState::REPEAT) {
		frustum.Translate(math::float3(-(movSpeed * speedScale), 0.0F, 0.0F));
		CalculateMatrixes();
	}
	// Down
	if (App->input->GetKey(SDL_SCANCODE_Q) == KeyState::REPEAT) {
		frustum.Translate(math::float3(0.0F, movSpeed * speedScale, 0.0F));
		CalculateMatrixes();
	}
	// Up
	if (App->input->GetKey(SDL_SCANCODE_E) == KeyState::REPEAT) {
		frustum.Translate(math::float3(0.0F, -(movSpeed * speedScale), 0.0F));
		CalculateMatrixes();
	}
}

void ModuleCamera::HandleRotation() {
	iPoint motion = App->input->GetMouseMotion();
	int motionOffset = 10;
	if (abs(motion.x) > motionOffset) {
		// Left
		if (motion.x < 0) {
			rotAngle += math::float3(-rotSpeed * speedScale, 0.0F, 0.0F);
			CalculateMatrixes();
		}
		// Right
		else {
			rotAngle += math::float3(rotSpeed * speedScale, 0.0F, 0.0F);
			CalculateMatrixes();
		}
	}
	if (abs(motion.y) > motionOffset) {
		// Down
		if (motion.y > 0) {
			rotAngle += math::float3(0.0F, -rotSpeed * speedScale, 0.0F);
			CalculateMatrixes();
		}
		// Up
		else {
			rotAngle += math::float3(0.0F, rotSpeed * speedScale, 0.0F);
			CalculateMatrixes();
		}
	}
}

void ModuleCamera::HandleZoom() {
	// Forward
	if (App->input->GetMouseWheel()->y > 0) {
		frustum.Translate(math::float3(0.0F, 0.0F, zoomSpeed * speedScale));
		CalculateMatrixes();
		App->input->GetMouseWheel()->y = 0;
	}
	// Backward
	if (App->input->GetMouseWheel()->y < 0) {
		frustum.Translate(math::float3(0.0F, 0.0F, -(zoomSpeed * speedScale)));
		CalculateMatrixes();
		App->input->GetMouseWheel()->y = 0;
	}
}
