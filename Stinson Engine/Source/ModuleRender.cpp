#include "ModuleRender.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "Mesh.h"
#include <SDL.h>
#include <il.h>
#include <ilu.h>
#include <ilut.h>

// https://www.khronos.org/opengl/wiki/Common_Mistakes
void __stdcall OpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

bool ModuleRender::Init() {
	LOG("Init Module Render\n");
	context = SDL_GL_CreateContext(App->window->window);
	GLenum err = glewInit();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0F);
	glClearColor(0.F, 0.F, 0.F, 1.F);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OpenGLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	// triangle
	float vertex_data[] = {
		-1.0F, -1.0F, 0.0F, // v0 pos
		1.0F, -1.0F, 0.0F, // v1 pos
		0.0F, 1.0F, 0.0F, // v2 pos

		0.0F, 0.0F, // v0 texcoord
		1.0F, 0.0F, // v1 texcoord
		0.5F, 1.0F // v2 texcoord
	};

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// square
	float vertexData[] = {
		-1.0F, -1.0F, 0.0F,
		 1.0F, -1.0F, 0.0F,
		 1.0F,  1.0F, 0.0F,
		-1.0F,  1.0F, 0.0F,

		0.0F, 0.0F,
		1.0F, 0.0F,
		1.0F, 1.0F,
		0.F, 1.0F
	};

	unsigned int indexData[] = { 0, 1, 2, 3 };

	glGenBuffers(1, &squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &squareIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// textures
	imageInfo = new ILinfo();
	imageInfo->Width = 1;
	imageInfo->Height = 1;
	texture = App->textures->Load("../Resources/Assets/butterflies.jpg", imageInfo);

	return true;
}

UpdateStatus ModuleRender::PreUpdate() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleRender::Update() {
	// Draw Texture
	glUseProgram(*App->programs->textureProgram);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->textureProgram, "model"), 1, GL_TRUE, &App->camera->GetModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->textureProgram, "view"), 1, GL_TRUE, &App->camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->textureProgram, "proj"), 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]);

	if (drawTriangle) {
		// Triangle
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		// Triangle Texture
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 3));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(*App->programs->textureProgram, "texture0"), 0);
	}
	else if (drawSquare) {
		// Square
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIBO);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(0);

		// Square Texture
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 4));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(*App->programs->textureProgram, "texture0"), 0);
	}

	// Draw Grid Lines
	glUseProgram(*App->programs->gridLinesProgram);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->gridLinesProgram, "model"), 1, GL_TRUE, &App->camera->GetModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->gridLinesProgram, "view"), 1, GL_TRUE, &App->camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->gridLinesProgram, "proj"), 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]);
	DrawLineGrid();

	// Draw baker house
	App->model->RenderAllMeshes();
	glUseProgram(0);

	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleRender::PostUpdate() {
	return UpdateStatus::CONTINUE;
}

bool ModuleRender::CleanUp() {
	LOG("Destroying renderer\n");
	//Destroy window
	SDL_GL_DeleteContext(context);
	delete imageInfo;
	//TODO delete all textures
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height) const {
	glViewport(0, 0,(GLsizei)width, (GLsizei)height);
}

void* ModuleRender::GetContext() const {
	return context;
}

void ModuleRender::DrawLineGrid() {
	// Lines white
	glLineWidth(1.0F);
	float d = 200.0F;
	glColor4f(1.F, 1.F, 1.F, 1.F);
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0F) {
		glVertex3f(i, 0.0F, -d);
		glVertex3f(i, 0.0F, d);
		glVertex3f(-d, 0.0F, i);
		glVertex3f(d, 0.0F, i);
	}
	glEnd();

	glLineWidth(2.0F);
	glBegin(GL_LINES);

	// Red X
	glColor4f(1.0F, 0.0F, 0.0F, 1.0F);
	glVertex3f(0.0F, 0.0F, 0.0F);
	glVertex3f(1.0F, 0.0F, 0.0F);
	glVertex3f(1.0F, 0.1F, 0.0F);
	glVertex3f(1.1F, -0.1F, 0.0F);
	glVertex3f(1.1F, 0.1F, 0.0F);
	glVertex3f(1.0F, -0.1F, 0.0F);

	// Green Y
	glColor4f(0.0F, 1.0F, 0.0F, 1.0F);
	glVertex3f(0.0F, 0.0F, 0.0F);
	glVertex3f(0.0F, 1.0F, 0.0F);
	glVertex3f(-0.05F, 1.25F, 0.0F);
	glVertex3f(0.0F, 1.15F, 0.0F);
	glVertex3f(0.05F, 1.25F, 0.0F);
	glVertex3f(0.0F, 1.15F, 0.0F);
	glVertex3f(0.0F, 1.15F, 0.0F);
	glVertex3f(0.0F, 1.05F, 0.0F);

	// Blue Z
	glColor4f(0.0F, 0.0F, 1.0F, 1.0F);
	glVertex3f(0.0F, 0.0F, 0.0F);
	glVertex3f(0.0F, 0.0F, 1.0F);
	glVertex3f(-0.05F, 0.1F, 1.05F);
	glVertex3f(0.05F, 0.1F, 1.05F);
	glVertex3f(0.05F, 0.1F, 1.05F);
	glVertex3f(-0.05F, -0.1F, 1.05F);
	glVertex3f(-0.05F, -0.1F, 1.05F);
	glVertex3f(0.05F, -0.1F, 1.05F);

	glEnd();
	glLineWidth(1.0F);
}

const char* ModuleRender::GetSourceStr(GLenum source) {
	switch (source) {
	case GL_DEBUG_SOURCE_API: return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW";
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER";
	case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
	case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER: return "OTHER";
	default: return "INVALID";
	}
}

const char* ModuleRender::GetTypeStr(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED";
	case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
	case GL_DEBUG_TYPE_MARKER: return "MARKER";
	case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH GROUP";
	case GL_DEBUG_TYPE_POP_GROUP: return "POP GROUP";
	case GL_DEBUG_TYPE_OTHER: return "OTHER";
	default: return "INVALID";
	}
}

const char* ModuleRender::GetSeverityStr(GLenum severity) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW: return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	default: return "INVALID";
	}
}

void __stdcall OpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	char tmp_string[4096];
	const char* tmp_source = ModuleRender::GetSourceStr(source);
	const char* tmp_type = ModuleRender::GetTypeStr(type);
	const char* tmp_severity = ModuleRender::GetSeverityStr(severity);
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		fprintf(stderr, "Aborting...\n");
		//abort();
	}
	sprintf_s(tmp_string, 4095, "<Severity:%s> <Source:%s> <Type:%s> <ID:%d> <Message:%s>\n", tmp_severity, tmp_source, tmp_type, id, message);
	OutputDebugString((const wchar_t*)tmp_string);
	//LOG(tmp_string);
}
