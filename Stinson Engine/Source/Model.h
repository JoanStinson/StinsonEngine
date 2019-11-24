#ifndef __MODEL_H__
#define __MODEL_H__

#include <mesh.h>
#include <vector>
#include <glew.h>
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include "../Libraries/MathGeoLib/MathGeoLib.h"

class Model {
public:
	Model(const char *filename, unsigned int texture, unsigned int program);
	~Model();
	void Render();
	void Render(unsigned int modelTexture, unsigned int modelProgram);

	unsigned int GetNumMeshes() const;
	unsigned int GetNumPolys() const;
	unsigned int GetNumVertices() const;

public:
	const char *filename = nullptr;
	unsigned int texture;
	unsigned int program;
	ILinfo imageInfo;
	math::float3 translate;
	math::float3 rotate;
	math::float3 scale;

private:
	struct Mesh {
		Mesh(aiMesh *mesh);
		~Mesh();
		void Render();

		GLuint vao;
		GLuint vbo[4];
		unsigned int polysCount;
		unsigned int verticesCount;
		enum BUFFER { VERTEX, TEXCOORD, NORMAL, INDEX };
	};

	std::vector<Mesh*> meshes;
	unsigned int numMeshes = 0;
	unsigned int numPolys = 0;
	unsigned int numVertices = 0;
};

#endif // __MODEL_H__
