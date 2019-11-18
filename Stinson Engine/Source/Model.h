#ifndef __MODEL_H__
#define __MODEL_H__

#include <mesh.h>
#include <vector>

typedef unsigned int GLuint;

class Model {
public:
	Model(const char *filename, unsigned int texture, unsigned int program);
	~Model();
	void Render();
	void Render(unsigned int meshTexture, unsigned int meshProgram);

public:
	struct Mesh {
		Mesh(aiMesh *mesh);
		~Mesh();
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
	std::vector<Mesh*> meshes;
};

#endif // __MODEL_H__
