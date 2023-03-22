#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
public:
	// program ID
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	// use/enable function
	void use();

	// uniform utility function
	void uniformSet(const std::string &name, bool value) const;
	void uniformSet(const std::string &name, int value) const;
	void uniformSet(const std::string &name, float value) const;
	void uniformSet(const std::string &name, glm::mat4 value) const;
};


#endif
