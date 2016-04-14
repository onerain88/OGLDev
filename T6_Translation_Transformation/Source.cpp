#include <stdio.h>
#include <string.h>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "ogldev_math_3d.h"
#include "ogldev_util.h"

GLuint VBO;
GLuint shaderProgram;

const char* VSFile = "shader.vs";
const char* FSFile = "shader.fs";

void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	static float scale = 0.1f;
	scale += 0.001f;

	Matrix4f world;
	world.m[0][0] = 1.0f; world.m[0][1] = 0.0f; world.m[0][2] = 0.0f; world.m[0][3] = sinf(scale);
	world.m[1][0] = 0.0f; world.m[1][1] = 1.0f; world.m[1][2] = 0.0f; world.m[1][3] = 0.0f;
	world.m[2][0] = 0.0f; world.m[2][1] = 0.0f; world.m[2][2] = 1.0f; world.m[2][3] = 0.0f;
	world.m[3][0] = 0.0f; world.m[3][1] = 0.0f; world.m[3][2] = 0.0f; world.m[3][3] = 1.0f;
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "_world"), 1, GL_TRUE, &world.m[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void CreateVertices()
{
	Vector3f vertices[3];
	vertices[0] = Vector3f(-0.5f, -0.5f, 0.0f);
	vertices[1] = Vector3f(0.5f, -0.5f, 0.0f);
	vertices[2] = Vector3f(0.0f, 0.5, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AddShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		fprintf(stderr, "Create shader error %d\n", shaderType);
		exit(1);
	}
	const GLchar* texts[1];
	texts[0] = pShaderText;
	GLint lens[1];
	lens[0] = strlen(texts[0]);
	glShaderSource(shader, 1, texts, lens);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar errorLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, errorLog);
		fprintf(stderr, "Compile shader %d error : %s\n", success, errorLog);
		exit(1);
	}
	glAttachShader(shaderProgram, shader);
}

void CompileShader()
{
	shaderProgram = glCreateProgram();
	string vs, fs;
	if (!ReadFile(VSFile, vs))
		exit(1);
	if (!ReadFile(FSFile, fs))
		exit(1);

	AddShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint success;
	GLchar errorLog[1024];
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
		fprintf(stderr, "Link error : %s\n", errorLog);
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
		fprintf(stderr, "Validate error : %s\n", errorLog);
		exit(1);
	}
}

void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Tutorial 06");

	GLenum ret = glewInit();
	if (ret != GLEW_OK)
	{
		fprintf(stderr, "Error : %s\n", glewGetErrorString(ret));
		exit(1);
	}

	glClearColor(0.8f, 0.3f, 0.1f, 1.0f);

	InitializeGlutCallbacks();
	
	CompileShader();
	CreateVertices();

	glutMainLoop();

	return 0;
}