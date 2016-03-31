#pragma once
#include "Include.h"

#include <ft2build.h>
#include FT_FREETYPE_H

GLuint vbo;

FT_Library ft;
FT_Face face;

GLuint tex;

// Shader sources
const GLchar* avSource =
"#version 150 core\n"
"in vec2 position;"
"in vec3 color;"
"in vec2 texcoord;"
"out vec3 Color;"
"out vec2 Texcoord;"
"void main() {"
"   Color = color;"
"   Texcoord = texcoord;"
"   gl_Position = vec4(position, 0.0, 1.0);"
"}";

const GLchar* afSource =
"#version 150 core\n"
"in vec3 Color;"
"in vec2 Texcoord;"
"out vec4 outColor;"
"uniform sampler2D sampTex;"
"void main() {"
"	vec4 tex = texture(sampTex, Texcoord);"
"   outColor = vec4(Color.rgb,1.0*tex.a);"
"}";

GLuint shaderProgram;

GLuint vertexShader;
GLuint fragmentShader;

void FREEGL()
{
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteProgram(shaderProgram);
}

GLint posAttrib;
GLint colAttrib;
GLint texAttrib;

int init_resources() {
	/* Initialize the FreeType2 library */
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return 0;
	}

	/* Load a font */
	if (FT_New_Face(ft, "segoeuil.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font %s\n", "segoeuil.ttf");
		return 0;
	};

	// Create the vertex buffer object
	glGenBuffers(1, &vbo);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &avSource, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &afSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);

	posAttrib = glGetAttribLocation(shaderProgram, "position");
	colAttrib = glGetAttribLocation(shaderProgram, "color");
	texAttrib = glGetAttribLocation(shaderProgram, "texcoord");

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	return 1;
}


void render_text(const char *text, float x, float y, float sx, float sy) 
{
	const char *p = text;
	FT_GlyphSlot g = face->glyph;

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));


	/* Loop through all characters */
	while (*p != '\0') {
		/* Try to load and render the character */
		//if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
		//	continue;

		FT_Set_Pixel_Sizes(face, 0, 58);

		FT_Error err = FT_Load_Char(face, *p, FT_LOAD_RENDER);


		
		

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat vertices[] = {
			//Position			Color             Texcoords
			x2, -y2,			0.3f, 1.0f, 0.5f, 0.0f, 0.0f, // Top-left
			x2 + w, -y2,		0.3f, 1.0f, 0.5f, 1.0f, 0.0f, // Top-right
			x2, -y2 - h,		0.3f, 0.0f, 0.5f, 0.0f, 1.0f, // Bottom-left
			x2 + w, -y2 - h,	0.3f, 0.0f, 0.5f, 1.0f, 1.0f  // Bottom-right
		};
		
		glUseProgram(shaderProgram);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glUseProgram(0);

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;

		//glDisableVertexAttribArray(colAttrib);
		//glDisableVertexAttribArray(texAttrib);
		//glDisableVertexAttribArray(posAttrib);
		++p;
	}

	
	
	glDeleteTextures(1, &tex);
}

/*
void display() {
	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

	glUseProgram(program);

	// White background
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable blending, necessary for our alpha texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat black[4] = { 0, 0, 0, 1 };
	GLfloat red[4] = { 1, 0, 0, 1 };
	GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

	// Set font size to 48 pixels, color to black
	FT_Set_Pixel_Sizes(face, 0, 48);
	glUniform4fv(uniform_color, 1, black);

	// Effects of alignment
	render_text("The Quick Brown Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
	render_text("The Misaligned Fox Jumps Over The Lazy Dog", -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

	// Scaling the texture versus changing the font size
	render_text("The Small Texture Scaled Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 175 * sy, sx * 0.5, sy * 0.5);
	FT_Set_Pixel_Sizes(face, 0, 24);
	render_text("The Small Font Sized Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 200 * sy, sx, sy);
	FT_Set_Pixel_Sizes(face, 0, 48);
	render_text("The Tiny Texture Scaled Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 235 * sy, sx * 0.25, sy * 0.25);
	FT_Set_Pixel_Sizes(face, 0, 12);
	render_text("The Tiny Font Sized Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 250 * sy, sx, sy);
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Colors and transparency
	render_text("The Solid Black Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 430 * sy, sx, sy);

	glUniform4fv(uniform_color, 1, red);
	render_text("The Solid Red Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 330 * sy, sx, sy);
	render_text("The Solid Red Fox Jumps Over The Lazy Dog", -1 + 28 * sx, 1 - 450 * sy, sx, sy);

	glUniform4fv(uniform_color, 1, transparent_green);
	render_text("The Transparent Green Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 380 * sy, sx, sy);
	render_text("The Transparent Green Fox Jumps Over The Lazy Dog", -1 + 18 * sx, 1 - 440 * sy, sx, sy);

	glutSwapBuffers();
}
*/
