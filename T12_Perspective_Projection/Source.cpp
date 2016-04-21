#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev_math_3d.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"

GLuint VBO;
GLuint IBO;
GLuint shaderProgram;
PersProjInfo ppi;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	static float scale = 0.0f;
	scale += 0.1f;

	Pipeline p;
	
	p.WorldPos(0.0f, 0.0f, 5.0f);
	p.Rotate(0.0f, scale, 0.0f);
	p.SetPerspectiveProj(ppi);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "_World"), 1, GL_TRUE, (const GLfloat*)p.GetWPTrans());

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

void CreateVertexBuffer()
{
	Vector3f vertices[4];
	vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	vertices[1] = Vector3f(0.0f, -1.0, 1.0f);
	vertices[2] = Vector3f(1.0f, -1.0f, 0.0f);
	vertices[3] = Vector3f(0.0f, 1.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void CreateIndexBuffer()
{
	unsigned int indecies[] = 
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
}

void AddShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
	GLuint shaderObj = glCreateShader(shaderType);
	if (shaderObj == 0)
	{
		fprintf(stderr, "Error create shader type %d\n", shaderType);
		exit(0);
	}
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint lengths[1];
	lengths[0] = strlen(pShaderText);
	glShaderSource(shaderObj, 1, p, lengths);
	glCompileShader(shaderObj);
	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compile shader type %d: %s\n", shaderType, infoLog);
		exit(1);
	}
	glAttachShader(shaderProgram, shaderObj);
}

void CompileShader()
{
	shaderProgram = glCreateProgram();
	if (shaderProgram == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	string vs, fs;
	if (!ReadFile(pVSFileName, vs))
		exit(1);
	if (!ReadFile(pFSFileName, fs))
		exit(1);

	// ±‡“Î∂•µ„∫Õ∆¨∂Œshader
	AddShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	// ¡¥Ω”shader
	GLint success = 0;
	GLchar errorLog[1024];
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
		fprintf(stderr, "Error link shader program : %s\n", errorLog);
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
		fprintf(stderr, "Invalid shader program: %s\n", errorLog);
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 12");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.8f, 0.3f, 0.1f, 1.0f);

	InitializeGlutCallbacks();

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShader();

	ppi.FOV = 30.0f;
	ppi.Width = GLUT_WINDOW_WIDTH;
	ppi.Height = GLUT_WINDOW_HEIGHT;
	ppi.zNear = 1.0f;
	ppi.zFar = 1000.0f;

	glutMainLoop();

	return 0;
}