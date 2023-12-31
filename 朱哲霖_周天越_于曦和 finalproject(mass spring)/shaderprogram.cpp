#include "shaderprogram.h"

#include <memory>


#include <iostream>
#include <fstream>

const std::string ShaderProgram::ReadFromFile(std::string fileName) {
	std::ifstream ifs(fileName.c_str());
	std::string out;
	std::cerr << "shader:" << fileName.c_str() << std::endl;
	if (ifs.is_open()) {
		std::string s;
		while (std::getline(ifs, s)) {
			out += s + "\n";
		}
	}
	else {
		std::cerr << "shader error:";
		std::cerr << "Could not open file" << std::endl;
	}

	ifs.close();
	return out;
}

ShaderProgram::ShaderProgram(std::string vertex_shader_filename, std::string tessellation_control_shader_filename,
	std::string tessellation_eval_shader_filename, std::string geometry_shader_filename,
	std::string fragment_shader_filename) {

	if (vertex_shader_filename != "") {
		auto v_source = ReadFromFile(vertex_shader_filename);
		AttachShader(GL_VERTEX_SHADER, v_source);
	}
	if (fragment_shader_filename != "") {
		auto f_source = ReadFromFile(fragment_shader_filename);
		AttachShader(GL_FRAGMENT_SHADER, f_source);
	}

	//Link shaders
	ConfigureShaderProgram();

	//Detach shaders after successful linking
	for (GLuint shader_program : shader_programs_) {
		glDetachShader(prog, shader_program);
	}
}

ShaderProgram::~ShaderProgram() {
	for (GLuint shader_program : shader_programs_) {
		glDeleteShader(shader_program);
	}
	glDeleteProgram(prog);

}

GLuint ShaderProgram::AttachShader(GLuint shaderType, std::string source) {
	GLuint sh = compile(shaderType, source.c_str());
	shader_programs_.push_back(sh);

	//std::cout << "Attached shader of type: '" << getShaderType(shaderType) << "'\n";

	return sh;
}

void ShaderProgram::ConfigureShaderProgram() {
	prog = glCreateProgram();

	for (GLuint shader_program : shader_programs_) {
		glAttachShader(prog, shader_program);
	}

	glLinkProgram(prog);

	GLint isLinked;
	glGetProgramiv(prog, GL_LINK_STATUS, (int*)&isLinked);

	if (isLinked == GL_FALSE) {
		GLint length;
		glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &length);
		std::string log(length, ' ');
		glGetShaderInfoLog(prog, length, &length, &log[0]);
		std::cerr << "Failed to link shaderprogram : " << std::endl
			<< log << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		//std::cout << "Shader linking complete!\n";
	}
}

const std::string ShaderProgram::getShaderType(GLuint type) {
	std::string name;
	switch (type) {
	case GL_VERTEX_SHADER:
		name = "Vertex Shader";
		break;
	case GL_TESS_CONTROL_SHADER:
		name = "Tessellation Control Shader";
		break;
	case GL_TESS_EVALUATION_SHADER:
		name = "Tessellation Evaluation Shader";
		break;
	case GL_GEOMETRY_SHADER:
		name = "Geometry Shader";
		break;
	case GL_FRAGMENT_SHADER:
		name = "Fragment Shader";
		break;
	default:
		name = "Unknown Shader type";
		break;
	}
	return name;
}

GLuint ShaderProgram::compile(GLuint type, GLchar const* source) {
	GLuint shader = glCreateShader(type);
	std::cerr << "source:" << source << std::endl;
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
		std::string log(logSize, ' ');
		glGetShaderInfoLog(shader, logSize, &logSize, &log[0]);
		std::cerr << "Failed to compile shadertype: " << getShaderType(type) << std::endl
			<< log << std::endl;
		glDeleteShader(shader); // Don't leak the shader.
		exit(EXIT_FAILURE);
	}
	return shader;
}

void ShaderProgram::updateCommonUniforms(MouseRotator rotator, float width, float height, float time, glm::vec3 clear_color) {
	// Uniforms
	GLint MV_Loc, P_Loc, lDir_Loc, camPos_Loc, clear_color_Loc, time_Loc, window_dim_Loc = -1;
	time_Loc = glGetUniformLocation(*this, "time");
	MV_Loc = glGetUniformLocation(*this, "MV");
	P_Loc = glGetUniformLocation(*this, "P");
	camPos_Loc = glGetUniformLocation(*this, "camPos");
	lDir_Loc = glGetUniformLocation(*this, "lDir");
	clear_color_Loc = glGetUniformLocation(*this, "clear_color");
	window_dim_Loc = glGetUniformLocation(*this, "window_dim");

	glm::vec2 window_dim = glm::vec2(width, height);

	glm::mat4 MV, P;
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 VRotX = glm::rotate(M, (rotator.phi), glm::vec3(0.0f, -1.0f, 0.0f)); //Rotation about y-axis
	glm::mat4 VRotY = glm::rotate(M, (rotator.theta + 0.2f), glm::vec3(-1.0f, 0.0f, 0.0f)); //Rotation about x-axis
	glm::vec4 camPos = VRotX * VRotY * glm::vec4(0.0f, 20.0f, 200.0f + 0.2f * rotator.zoom, 1.0f);
	glm::vec3 scene_center(0.0f, 10.0f, 0.0f);
	glm::mat4 V = glm::lookAt(glm::vec3(camPos), scene_center, glm::vec3(0.0f, -1.0f, 0.0f));
	P = glm::perspectiveFov(50.0f, static_cast<float>(width), static_cast<float>(height), 0.1f, 4000.0f);
	MV = V * M;

	glm::vec3 lDir = glm::vec3(1.0f, 0.0f, 0.0f);

	//Send uniform variables
	glProgramUniform1f(*this, time_Loc, time);
	glUniformMatrix4fv(MV_Loc, 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(P_Loc, 1, GL_FALSE, &P[0][0]);
	glUniform3fv(camPos_Loc, 1, &camPos[0]);
	glUniform3fv(lDir_Loc, 1, &lDir[0]);
	glUniform3fv(clear_color_Loc, 1, &clear_color[0]);
	glUniform2fv(window_dim_Loc, 1, &window_dim[0]);

}

