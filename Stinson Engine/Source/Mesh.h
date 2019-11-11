#ifndef __MESH_H__
#define __MESH_H__

#include <mesh.h>
#include <vector>

typedef unsigned int GLuint;

class Mesh {
public:
	Mesh(const char *filename, unsigned texture, unsigned program);
	~Mesh();
	void Render();
	void Render(unsigned meshTexture, unsigned meshProgram);

public:
	struct Vertex {
		Vertex(aiMesh *mesh);
		~Vertex();
		void Render();

		GLuint vao;
		GLuint vbo[4];
		unsigned int elementCount;
		enum class BUFFER { VERTEX, TEXCOORD, NORMAL, INDEX };
	};

	unsigned int texture;
	unsigned int program;

private:
	std::vector<Vertex*> meshEntries;
};

#endif // __MESH_H__

// https://badvertex.com/2014/04/13/loading-meshes-using-assimp-in-opengl.html