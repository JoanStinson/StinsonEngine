#include "ModuleProgram.h"
#include <glew.h>
#include <assert.h>

bool ModuleProgram::Init() {
	// 1) Retrieve vertex/fragment source code from filePath
	char *vShaderCode = nullptr;
	char *fShaderCode = nullptr;
	FILE *file = nullptr;

	fopen_s(&file, "../Resources/Shaders/default.vs", "rb");
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

	fopen_s(&file, "../Resources/Shaders/default.fs", "rb");
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
	def_program = glCreateProgram();
	glAttachShader(def_program, vertex);
	glAttachShader(def_program, fragment);
	glLinkProgram(def_program);
	CheckCompileErrors(def_program, "PROGRAM");

	// 4) Delete shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	delete vShaderCode;
	delete fShaderCode;

	return true;
}

bool ModuleProgram::CleanUp() {
	return true;
}

void ModuleProgram::CheckCompileErrors(unsigned int shader, const std::string &type) const {
	int success;
	char infoLog[1024];
	std::string message;
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			message = "ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- \n";
			LOG(message.c_str());
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			message = "ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- \n";
			LOG(message.c_str());
		}
	}
}
