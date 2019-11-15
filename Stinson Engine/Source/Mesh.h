#ifndef __MESH_H__
#define __MESH_H__

#include <mesh.h>
#include <vector>

typedef unsigned int GLuint;

class Mesh {
public:
	Mesh(const char *filename, unsigned int texture, unsigned int program);
	~Mesh();
	void Render();
	void Render(unsigned int meshTexture, unsigned int meshProgram);

public:
	struct MeshEntry {
		MeshEntry(aiMesh *mesh, int index);
		~MeshEntry();
		void Render();

		GLuint vao;
		GLuint vbo[4];
		unsigned int elementCount;
		enum class BUFFER { VERTEX, TEXCOORD, NORMAL, INDEX };
	};

	const char *filename = nullptr;
	unsigned int texture;
	unsigned int program;

private:
	std::vector<MeshEntry*> meshEntries;
};

#endif // __MESH_H__
