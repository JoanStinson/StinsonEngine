#include "ModuleProgram.h"
#include <glew.h>
#include <assert.h>

ModuleProgram::ModuleProgram() {
	textureProgram = new unsigned();
	gridLinesProgram = new unsigned();
}

ModuleProgram::~ModuleProgram() {
	delete textureProgram;
	delete gridLinesProgram;
}

bool ModuleProgram::Init() {
	LOG("Init Module Program\n");
	*textureProgram = LoadShader("../Resources/Shaders/texture.vs", "../Resources/Shaders/texture.fs");
	assert(textureProgram != nullptr);
	*gridLinesProgram = LoadShader("../Resources/Shaders/gridLines.vs", "../Resources/Shaders/gridLines.fs");
	assert(gridLinesProgram != nullptr);
	return true;
}

bool ModuleProgram::CleanUp() {
	return true;
}

unsigned ModuleProgram::LoadShader(const char *vertexFile, const char *fragmentFile) {
	// 1) Retrieve vertex/fragment source code from filePath
	FILE *file = nullptr;

	char *vShaderCode = nullptr;
	fopen_s(&file, vertexFile, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		vShaderCode = new char[size + 1];

		fread(vShaderCode, 1, size, file);
		vShaderCode[size] = 0;

		fclose(file);
	}
	assert(file != nullptr);

	char *fShaderCode = nullptr;
	fopen_s(&file, fragmentFile, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		fShaderCode = new char[size + 1];

		fread(fShaderCode, 1, size, file);
		fShaderCode[size] = 0;

		fclose(file);
	}
	assert(file != nullptr);

	// 2) Compile shaders
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	// 3) Create program
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	CheckCompileErrors(program, "PROGRAM");

	// 4) Delete shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	delete vShaderCode;
	delete fShaderCode;

	return program;
}

void ModuleProgram::CheckCompileErrors(unsigned int shader, const char *type) const {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			LOG("ERROR::SHADER_COMPILATION_ERROR of type: %s \n %s \n -- --------------------------------------------------- -- \n", type, infoLog);
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			LOG("ERROR::SHADER_LINKING_ERROR of type: %s \n %s \n -- --------------------------------------------------- -- \n", type, infoLog);
		}
	}
}
