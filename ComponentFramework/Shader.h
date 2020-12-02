#ifndef SHADER_H
#define SHADER_H
#include "glew.h"
#include <unordered_map>

class Shader {
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator = (const Shader&) = delete;
	Shader& operator = (Shader&&) = delete;

public:
	Shader(const char *vsFilename, const char *fsFilename, const char *tcsfilename = " ", const char* tesfilename = " ", const char* gsfilename = " ");
	~Shader();

	inline GLuint getProgram() const { return shaderID;}
	inline GLuint getUniformID(std::string name) { return uniformMap[name]; }

private:
	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int tessconShaderID;
	unsigned int tessevShaderID;
	unsigned int geoShaderID;
	unsigned int fragShaderID;
	std::unordered_map<std::string, unsigned int> uniformMap;

	char* readTextFile(const char *fileName);	
	void readCompileAttach(const char *vsFilename, const char *fsFilename, const char* tcsfilename, const char* tesfilename, const char* gsfilename);
	void link();
	void setUniformLocations();

};
#endif