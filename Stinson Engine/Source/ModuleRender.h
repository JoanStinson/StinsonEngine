#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include <glew.h>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
struct ILinfo;

class ModuleRender : public Module {
public:
	ModuleRender(){}
	~ModuleRender(){}

	bool Init() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;

public:
	void WindowResized(unsigned width, unsigned height) const;
	void* GetContext() const;

public:
	unsigned int texture;
	unsigned int sanicTexture;
	unsigned int blueTexture;

	bool drawTriangle = false;
	bool drawSquare = true;

	ILinfo *imageInfo = nullptr;

private:
	void DrawLineGrid();
	static const char* GetSourceStr(GLenum source);
	static const char* GetTypeStr(GLenum type);
	static const char* GetSeverityStr(GLenum severity);
	friend void __stdcall OpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
	void* context;
	GLuint vbo[3];
	GLuint gProgramID = 0;
	GLint gVertexPos2DLocation = -1;
	GLuint gVBO = 0;
	GLuint gIBO = 0;
};

#endif // __MODULERENDER_H__