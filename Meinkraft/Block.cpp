#include "Block.h"
#include <GL/freeglut.h>

Block::Block(Vec3f position, int textureTop, int textureSides)
{
	this->position = position;

	int rowNum = textureSides / 16;
	int columnNum = textureSides % 16;

	float part = (float)1 / 16;

	float row = rowNum * part;
	float column = columnNum * part;
	float rowEnd = row + part;
	float columnEnd = column + part;

	texColsSides = Vec2f(column, columnEnd);
	texRowsSides = Vec2f(row, rowEnd);

	if (textureTop == textureSides)
	{
		texColsTop = texColsSides;
		texRowsTop = texRowsSides;
	}
	else
	{
		if (textureTop != -1)
		{
			hasTopTexture = true;

			int rowNum = textureTop / 16;
			int columnNum = textureTop% 16;

			float part = (float)1 / 16;

			row = rowNum * part;
			column = columnNum * part;
			rowEnd = row + part;
			columnEnd = column + part;

			texColsTop = Vec2f(column, columnEnd);
			texRowsTop = Vec2f(row, rowEnd);
		}
	}
}

Block::~Block()
{
}

void Block::draw()
{
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);

	glBegin(GL_QUADS);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	float column = texColsSides.x;
	float columnEnd = texColsSides.y;

	float row = texRowsSides.x;
	float rowEnd = texRowsSides.y;

	//Side size
	glTexCoord2f(column, rowEnd);		glVertex3f(0, 0, 0);		//Linksonder
	glTexCoord2f(columnEnd, rowEnd);	glVertex3f(size, 0, 0);		//Rechtsonder
	glTexCoord2f(columnEnd, row);		glVertex3f(size, size, 0);		//Rechtsboven
	glTexCoord2f(column, row);			glVertex3f(0, size, 0);		//Linksboven

																			//Side 2
	glTexCoord2f(column, rowEnd);		glVertex3f(0, 0, size);
	glTexCoord2f(columnEnd, rowEnd);	glVertex3f(size, 0, size);
	glTexCoord2f(columnEnd, row);		glVertex3f(size, size, size);
	glTexCoord2f(column, row);			glVertex3f(0, size, size);

	//Side 3
	glTexCoord2f(column, rowEnd);		glVertex3f(0, 0, 0);
	glTexCoord2f(column, row);			glVertex3f(0, size, 0);
	glTexCoord2f(columnEnd, row);		glVertex3f(0, size, size);
	glTexCoord2f(columnEnd, rowEnd);	glVertex3f(0, 0, size);

	//Side 4
	glTexCoord2f(column, rowEnd);		glVertex3f(size, 0, 0);
	glTexCoord2f(column, row);			glVertex3f(size, size, 0);
	glTexCoord2f(columnEnd, row);		glVertex3f(size, size, size);
	glTexCoord2f(columnEnd, rowEnd);	glVertex3f(size, 0, size);

	if (!hasTopTexture)
	{
		glEnd();
		glPopMatrix();
		return;
	}

	//Different textures
	column = texColsTop.x;
	columnEnd = texColsTop.y;

	row = texRowsTop.x;
	rowEnd = texRowsTop.y;

	//Bottom
	glTexCoord2f(column, row);			glVertex3f(0, 0, 0);
	glTexCoord2f(column, rowEnd);		glVertex3f(size, 0, 0);
	glTexCoord2f(columnEnd, rowEnd);	glVertex3f(size, 0, size);
	glTexCoord2f(columnEnd, row);		glVertex3f(0, 0, size);

	//Top
	glTexCoord2f(column, row);			glVertex3f(0, size, 0);
	glTexCoord2f(column, rowEnd);		glVertex3f(size, size, 0);
	glTexCoord2f(columnEnd, rowEnd);	glVertex3f(size, size, size);
	glTexCoord2f(columnEnd, row);		glVertex3f(0, size, size);
	glEnd();
	glPopMatrix();
}
