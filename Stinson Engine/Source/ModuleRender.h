#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include <glew.h>
#include "../Libraries/MathGeoLib/MathGeoLib.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
struct ILinfo;
class Model;

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
	unsigned int GetRenderTexture() const;

public:
	math::float4 clearColor = math::float4(0.0F, 0.0F, 0.0F, 1.0F);
	bool drawGridLines = true;
	bool drawGeometry = true;
	bool drawFbo = true;

private:
	void DrawLineGrid();
	const char* GetSourceStr(GLenum source) const;
	const char* GetTypeStr(GLenum type) const;
	const char* GetSeverityStr(GLenum severity) const;
	friend void __stdcall OpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
	void* context = nullptr;
	unsigned int fbo;
	unsigned int rbo;
	unsigned int quadVAO;
	unsigned int renderTexture;

};

#endif // __MODULERENDER_H__