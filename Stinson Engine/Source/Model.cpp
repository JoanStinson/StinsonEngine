#include "Model.h"
#include "Globals.h"
#include <Importer.hpp>
#include <postprocess.h>
#include <scene.h>
#include <Logger.hpp>
#include <DefaultLogger.hpp>

class myStream : public Assimp::LogStream {
public:
	// LOG assimp debug output to GUI console
	void write(const char *message) {
		LOG("%s", message);
	}
};

Model::Model(const char *filename, unsigned int texture, unsigned int program) : filename(filename), texture(texture), program(program) {
	// Assimp logger
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	Assimp::DefaultLogger::get()->info("this is my info-call");
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::DefaultLogger::get()->attachStream(new myStream, severity);

	// Assimp import model
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr) {
		LOG("Unable to load model %s\n", importer.GetErrorString());
	}
	else {
		// Get model transformation
		aiMatrix4x4 transform = scene->mRootNode->mTransformation;
		translate = math::float3(transform.a4, transform.b4, transform.c4);
		rotate = math::float3(transform.d1, transform.d2, transform.d3);
		scale = math::float3(transform.a1, transform.b2, transform.c3);

		// Process all meshes from model
		numMeshes = scene->mNumMeshes;
		for (int i = 0; i < numMeshes; ++i) {
			meshes.push_back(new Model::Mesh(scene->mMeshes[i]));
			numPolys += meshes[i]->polysCount;
			numVertices += meshes[i]->verticesCount;
		}
	}

	Assimp::DefaultLogger::kill();
}

Model::~Model() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
	meshes.clear();
}

void Model::Render() {
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->Render();
}

void Model::Render(unsigned int modelTexture, unsigned int modelProgram) {
	glUseProgram(modelProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modelTexture);
	glUniform1i(glGetUniformLocation(modelProgram, "texture0"), 0);

	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->Render();
}

unsigned int Model::GetNumMeshes() const {
	return numMeshes;
}

unsigned int Model::GetNumPolys() const {
	return numPolys;
}

unsigned int Model::GetNumVertices() const {
	return numVertices;
}

Model::Mesh::Mesh(aiMesh *mesh) {
	vbo[VERTEX] = 0;
	vbo[TEXCOORD] = 0;
	vbo[NORMAL] = 0;
	vbo[INDEX] = 0;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	polysCount = mesh->mNumFaces;
	verticesCount = mesh->mNumVertices;

	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &vbo[VERTEX]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete[] vertices;
	}

	if (mesh->HasTextureCoords(0)) {
		float *texCoords = new float[mesh->mNumVertices * 2];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &vbo[TEXCOORD]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] texCoords;
	}

	if (mesh->HasNormals()) {
		float *normals = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &vbo[NORMAL]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] normals;
	}

	if (mesh->HasFaces()) {
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &vbo[INDEX]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] indices;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::Mesh::~Mesh() {
	if (vbo[VERTEX])
		glDeleteBuffers(1, &vbo[VERTEX]);
	if (vbo[TEXCOORD])
		glDeleteBuffers(1, &vbo[TEXCOORD]);
	if (vbo[NORMAL])
		glDeleteBuffers(1, &vbo[NORMAL]);
	if (vbo[INDEX])
		glDeleteBuffers(1, &vbo[INDEX]);
	glDeleteVertexArrays(1, &vao);
}

void Model::Mesh::Render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, polysCount * 3, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
