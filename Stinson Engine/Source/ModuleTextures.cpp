#include "ModuleTextures.h"
#include <glew.h>
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <string>

bool ModuleTextures::Init() {
	LOG("Init Module Textures\n");
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
		LOG("DevIL version is different...exiting!\n");
		return false;
	}
	return true;
}

bool ModuleTextures::CleanUp() {
	return true;
}

GLuint ModuleTextures::Load(const char *filename) {
	GLuint texture = NULL;
	ILenum error;

	ILuint image = ilGenImage();
	ilBindImage(image);

	if (ilLoadImage((const wchar_t*)filename)) {
		ILinfo imageInfo;
		iluGetImageInfo(&imageInfo);
		imageInfo.Type = ilDetermineType((const wchar_t*)filename);
		if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s - %s\n", std::to_string(error).c_str(), iluErrorString(error));
		}
	}
	else {
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s - %s\n", std::to_string(error).c_str(), iluErrorString(error));
	}

	ilDeleteImage(image);
	return texture;
}

GLuint ModuleTextures::Load(const char *filename, ILinfo *imageInfo) {
	GLuint texture = NULL;
	ILenum error;

	ILuint image = ilGenImage();
	ilBindImage(image);

	if (ilLoadImage((const wchar_t*)filename)) {
		iluGetImageInfo(imageInfo);
		imageInfo->Type = ilDetermineType((const wchar_t*)filename);
		if (imageInfo->Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s - %s\n", std::to_string(error).c_str(), iluErrorString(error));
		}
	}
	else {
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s - %s\n", std::to_string(error).c_str(), iluErrorString(error));
	}

	ilDeleteImage(image);
	return texture;
}

GLuint ModuleTextures::Load(const char *filename, ILinfo *imageInfo, GLuint wrap, bool filterNearest, bool useMipMaps) {
	GLuint texture = NULL;
	ILenum error;

	ILuint image = ilGenImage();
	ilBindImage(image);

	if (ilLoadImage((const wchar_t*)filename)) {
		iluGetImageInfo(imageInfo);
		imageInfo->Type = ilDetermineType((const wchar_t*)filename);
		if (imageInfo->Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			if (wrap == 1) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			else if (wrap == 2) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			}
			else if (wrap == 3) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else if (wrap == 4) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			}
			else {
				LOG("Error, wrap value not valid!\n");
			}

			if (filterNearest) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
			if (useMipMaps) glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s - %s\n", std::to_string(error).c_str(), iluErrorString(error));
		}
	}
	else {
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s - %s\n", std::to_string(error).c_str(), iluErrorString(error));
	}

	ilDeleteImage(image);
	return texture;
}
