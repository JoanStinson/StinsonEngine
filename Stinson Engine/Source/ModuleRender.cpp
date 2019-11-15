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

	// screen quad VAO
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// frame buffer configuration
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

UpdateStatus ModuleRender::PreUpdate() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleRender::Update() {
	// Draw grid lines
	glUseProgram(*App->programs->gridLinesProgram);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->gridLinesProgram, "model"), 1, GL_TRUE, &App->camera->GetModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->gridLinesProgram, "view"), 1, GL_TRUE, &App->camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->gridLinesProgram, "proj"), 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]);
	DrawLineGrid();

	// Draw baker house
	glUseProgram(*App->programs->textureProgram);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->textureProgram, "model"), 1, GL_TRUE, &App->camera->GetModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->textureProgram, "view"), 1, GL_TRUE, &App->camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*App->programs->textureProgram, "proj"), 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]);
	App->model->RenderAllMeshes();
	glUseProgram(0);

	// Draw framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0F, 1.0F, 1.0F, 1.0F); 
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(*App->programs->screenProgram);
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	
	glDrawArrays(GL_TRIANGLES, 0, 6);
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
	glDeleteVertexArrays(1, &quadVAO);
	return true;
}

void ModuleRender::WindowResized(unsigned int width, unsigned int height) {
	glViewport(0, 0, width, height);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
