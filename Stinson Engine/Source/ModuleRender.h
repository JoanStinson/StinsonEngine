#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include <glew.h>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
struct ILinfo;
class Mesh;

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
	void WindowResized(unsigned int width, unsigned int height);
	void* GetContext() const;

public:
	unsigned int textureColorbuffer;

private:
	void DrawLineGrid();
	static const char* GetSourceStr(GLenum source);
	static const char* GetTypeStr(GLenum type);
	static const char* GetSeverityStr(GLenum severity);
	friend void __stdcall OpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
	void* context = nullptr;
	GLuint triangleVBO;
	GLuint squareVBO;
	GLuint squareIBO;
	unsigned int framebuffer;
	unsigned int rbo;
	unsigned int quadVAO;
};

#endif // __MODULERENDER_H__