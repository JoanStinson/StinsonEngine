#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include <SDL_events.h>

#define MOUSE_CLICK_RIGHT App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KeyState::REPEAT
#define LEFT_SHIFT App->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::REPEAT

#define FORWARD App->input->GetKey(SDL_SCANCODE_W) == KeyState::REPEAT
#define BACKWARD App->input->GetKey(SDL_SCANCODE_S) == KeyState::REPEAT
#define LEFT App->input->GetKey(SDL_SCANCODE_A) == KeyState::REPEAT
#define RIGHT App->input->GetKey(SDL_SCANCODE_D) == KeyState::REPEAT
#define UP App->input->GetKey(SDL_SCANCODE_Q) == KeyState::REPEAT
#define DOWN App->input->GetKey(SDL_SCANCODE_E) == KeyState::REPEAT

#define MOUSE_MOVE_LEFT motion.x < 0
#define MOUSE_MOVE_RIGHT motion.x > 0
#define MOUSE_MOVE_UP motion.y < 0
#define MOUSE_MOVE_DOWN motion.y > 0

#define ZOOM_IN App->input->GetMouseWheel()->y > 0
#define ZOOM_OUT App->input->GetMouseWheel()->y < 0

bool ModuleCamera::Init() {
	LOG("Init Module Camera\n");
	ResetCamera();
	return true;
}

UpdateStatus ModuleCamera::Update() {
	if (MOUSE_CLICK_RIGHT) {
		Translate();
		Rotate();
	}
	Zoom();
	speedScale = LEFT_SHIFT ? 3.0F : 1.0F;
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
	model = math::float4x4::FromTRS(math::float3::zero, math::float3x3::RotateY(math::pi / 4.0F), float3::one);
	view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::ResetCamera() {
	movSpeed = 0.2F;
	rotSpeed = 0.1F;
	zoomSpeed = 0.4F;
	speedScale = 1.0F;
	aspectRatio = (float)App->window->GetWindowWidth() / (float)App->window->GetWindowHeight();

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = math::float3::zero;
	frustum.front = -math::float3::unitZ;
	frustum.up = math::float3::unitY;
	frustum.nearPlaneDistance = 0.1F;
	frustum.farPlaneDistance = 100.0F;
	frustum.verticalFov = math::pi / 4.0F;
	frustum.horizontalFov = 2.F * atanf(tanf(frustum.verticalFov * 0.5F) * aspectRatio);
	frustum.Translate(math::float3(0.0f, 2.0f, 10.f));

	model = math::float4x4::FromTRS(math::float3::zero, math::float3x3::RotateY(math::pi / 4.0F), float3::one);
	view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::Translate() {
	if (FORWARD) {
		frustum.pos += frustum.front * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	}
	if (BACKWARD) {
		frustum.pos -= frustum.front * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	}
	if (LEFT) {
		frustum.pos -= frustum.WorldRight() * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	}
	if (RIGHT) {
		frustum.pos += frustum.WorldRight() * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	}
	if (UP) {
		frustum.pos -= frustum.up * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	}
	if (DOWN) {
		frustum.pos += frustum.up * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	}
}

void ModuleCamera::Rotate() {
	iPoint motion = App->input->GetMouseMotion();
	int motionOffset = 15;

	if (abs(motion.x) > motionOffset) {
		pitch = 0;
		yaw = rotSpeed * speedScale;

		if (MOUSE_MOVE_LEFT) {
			frustum.front = math::float3x3::RotateY(-yaw).Transform(frustum.front).Normalized();
			frustum.up = math::float3x3::RotateY(-yaw).Transform(frustum.up).Normalized();
			view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
		}
		else if (MOUSE_MOVE_RIGHT) {
			frustum.front = math::float3x3::RotateY(yaw).Transform(frustum.front).Normalized();
			frustum.up = math::float3x3::RotateY(yaw).Transform(frustum.up).Normalized();
			view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
		}
	}
	else if (abs(motion.y) > motionOffset) {
		yaw = 0;
		pitch = rotSpeed * speedScale;

		if (MOUSE_MOVE_UP) {
			frustum.front = math::float3x3::RotateAxisAngle(frustum.WorldRight(), pitch).Transform(frustum.front).Normalized();
			frustum.up = math::float3x3::RotateAxisAngle(frustum.WorldRight(), pitch).Transform(frustum.up).Normalized();
			view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
		}
		else if (MOUSE_MOVE_DOWN) {
			frustum.front = math::float3x3::RotateAxisAngle(frustum.WorldRight(), -pitch).Transform(frustum.front).Normalized();
			frustum.up = math::float3x3::RotateAxisAngle(frustum.WorldRight(), -pitch).Transform(frustum.up).Normalized();
			view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
		}
	}
}

void ModuleCamera::Zoom() {
	if (ZOOM_IN) {
		frustum.pos += frustum.front * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
		App->input->GetMouseWheel()->y = 0;
	}
	if (ZOOM_OUT) {
		frustum.pos -= frustum.front * (movSpeed*speedScale);
		view = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
		App->input->GetMouseWheel()->y = 0;
	}
}
