#include "Rectangle.h"
#include "Game.h"
#include "Utility.cpp"

void RectangleShape::init(frect r, Texture* tex)
{
	rectangle = r;

	glGenBuffers(1, &vbo);

	texture = tex;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float vertices[] = { r.left, r.bot , -1, 0, (r.height) / tex->getHeight(),
		r.left + r.width, r.bot + r.height, -1, (r.width) / tex->getWidth(), 0,
		r.left, r.bot + r.height, -1, 0, 0,
		r.left, r.bot, -1, 0, (r.height) / tex->getHeight(),
		r.left + r.width, r.bot, -1, (r.width) / tex->getWidth(), (r.height) / tex->getHeight(),
		r.left + r.width, r.bot + r.height, -1, (r.width) / tex->getWidth(), 0 };

	vboSize = 6;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	auto shader = Game::shaderManager.getShader("minTex");

	c = Colour(0.05, 0.05, 0.05, 1.0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	registerVAO(VERT_3D_TEX, shader->getID());
	glBindVertexArray(0);
}

void RectangleShape::updateVBO()
{
	const frect r = rectangle;
	const Texture* tex = texture;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float vertices[] = { r.left, r.bot , -1, 0, (r.height) / tex->getHeight(),
		r.left + r.width, r.bot + r.height, -1, (r.width) / tex->getWidth(), 0,
		r.left, r.bot + r.height, -1, 0, 0,
		r.left, r.bot, -1, 0, (r.height) / tex->getHeight(),
		r.left + r.width, r.bot, -1, (r.width) / tex->getWidth(), (r.height) / tex->getHeight(),
		r.left + r.width, r.bot + r.height, -1, (r.width) / tex->getWidth(), 0 };

	//vboSize = 6;
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//auto shader = Game::shaderManager.getShader("minTex");

	//c = Colour(0.05, 0.05, 0.05, 1.0);

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//registerVAO(VERT_3D_TEX, shader->getID());
	//glBindVertexArray(0);
}

void RectangleShape::draw()
{
	glBindVertexArray(vao);

	auto shader = Game::shaderManager.getShader("minTex");
	shader->use();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBindTexture(GL_TEXTURE_2D, texture->getID());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	auto col = glGetUniformLocation(shader->getID(), "colour");
	glUniform4f(col, c.r, c.g, c.b, c.a);

	auto model = glGetUniformLocation(shader->getID(), "model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::fmat4()));

	glDrawArrays(GL_TRIANGLES, 0, vboSize);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RectangleShape::draw(Transform& t)
{
	glBindVertexArray(vao);

	auto shader = Game::shaderManager.getShader("minTex");
	shader->use();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBindTexture(GL_TEXTURE_2D, texture->getID());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	auto col = glGetUniformLocation(shader->getID(), "colour");
	glUniform4f(col, c.r, c.g, c.b, c.a);

	auto model = glGetUniformLocation(shader->getID(), "model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(t.getGLTransform()));

	glDrawArrays(GL_TRIANGLES, 0, vboSize);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
