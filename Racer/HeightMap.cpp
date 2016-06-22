#include "HeightMap.h"
#include "stb_image.h"
#include "Vector.h"

#include "LevelObject.h"

#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include "World.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

HeightMap::HeightMap(const std::string &file, World* world)
{
	int bpp;
	unsigned char* imgData = stbi_load(file.c_str(), &width, &height, &bpp, 4);

	auto heightAt = [&](int x, int y)
	{
		return (imgData[(x + y * width) * 4 ] / 256.0f) * 10.0f;
	};

	auto valueAt = [&](int x, int y, int offset = 0)
	{
		return imgData[(x + y * width) * 4 + offset];
	};

	std::vector<std::vector<Vec3f>> faceNormals(width-1, std::vector<Vec3f>(height-1, Vec3f(0,1,0)));
	for (int y = 0; y < height - 1; y++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			int offsets[4][2] = { { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } };
			Vec3f ca(0, heightAt(x, y + 1) - heightAt(x, y), 1);
			Vec3f ba(1, heightAt(x + 1, y) - heightAt(x, y), 0);
			Vec3f normal = ca.cross(ba);
			normal.Normalize();
			faceNormals[x][y] = normal;
		}
	}

	for (int y = 0; y < height-1; y++)
	{
		for (int x = 0; x < width-1; x++)
		{
			int offsets[4][2] = { { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } };

			if (valueAt(x, y, GREEN) > 5)
			{
				ObjectTemplate obtp = world->getObjectFromValue(valueAt(x, y, GREEN));
				LevelObject* p = new LevelObject(obtp.file, Vec3f(x, heightAt(x, y), y), obtp.rotation, obtp.scale, obtp.canCollide);

				world->addLevelObject(p);
			}
			if (valueAt(x, y, BLUE) > 10)
			{
				world->addStar(new Star(Vec3f(x, heightAt(x, y), y)));
			}

			for (int i = 0; i < 4; i++)
			{
				int xx = x + offsets[i][0];
				int yy = y + offsets[i][1];

				Vec3f normal(0, 0, 0);
				if(xx < width-1 && yy < height-1)
					normal = normal + faceNormals[xx][yy];
				if(xx > 0 && yy < height-1)
					normal = normal + faceNormals[xx-1][yy];
				if (xx > 0 && yy > 0)
					normal = normal + faceNormals[xx-1][yy-1];
				if (yy > 0 && xx < width-1)
					normal = normal + faceNormals[xx][yy-1];
				normal.Normalize();

				float h = heightAt(xx, yy);
				vertices.push_back(Vertex{ (float)(xx), h, (float)(yy),
									normal.x, normal.y, normal.z,
									(xx) / (float)height, (yy) / (float)width } );
			}
		}
	}

	glGenTextures(1, &imageIndex);
	glBindTexture(GL_TEXTURE_2D, imageIndex);

	glTexImage2D(GL_TEXTURE_2D,
		0,					//level
		GL_RGBA,			//internal format
		width,				//width
		height,				//height
		0,					//border
		GL_RGBA,			//data format
		GL_UNSIGNED_BYTE,	//data type
		imgData);			//data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(imgData);
}

HeightMap::~HeightMap()
{
	glDeleteTextures(1, &imageIndex);
}

void HeightMap::Draw()
{
	glEnable(GL_LIGHTING);
	float color[] = { 0.7f, 0.7f, 0.7f, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, imageIndex);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), ((float*)vertices.data()) + 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), ((float*)vertices.data()) + 6);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), ((float*)vertices.data()) + 3);
	glDrawArrays(GL_QUADS, 0, vertices.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

float HeightMap::GetHeight(float x, float y)
{
	int ix = x;
	int iy = y;

	int index = (ix + (width - 1) * iy) * 4;

	if (index + 3 >= vertices.size())
		index = vertices.size() - 4;

	if (index < 0)
		index = 0;

	Vertex& a = vertices[index];
	Vertex& b = vertices[index+1];
	Vertex& c = vertices[index+3];

	float lowervalue = ((b.z - c.z)*(a.x - c.x) + (c.x - b.x)*(a.z - c.z));
	float labda1 = ((b.z - c.z)*(x - c.x) + (c.x - b.x)*(y - c.z)) / lowervalue;
	float labda2 = ((c.y - a.y)*(x - c.x) + (a.x - c.x)*(y - c.y)) / lowervalue;
	float labda3 = 1 - labda1 - labda2;

	Vertex z = a * labda1 + b * labda2 + c * labda3;
//	Vertex z = (a * labda1) + (b * labda2) ;

	return z.y;
}


int HeightMap::GetSize()
{
	return height >= width ? height : width;
}

void HeightMap::SetTexture(const std::string &file)
{
	int bpp, width2, height2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load(file.c_str(), &width2, &height2, &bpp, 4);

	glBindTexture(GL_TEXTURE_2D, imageIndex);

	glTexImage2D(GL_TEXTURE_2D,
		0,		//level
		GL_RGBA,		//internal format
		width2,		//width
		height2,		//height
		0,		//border
		GL_RGBA,		//data format
		GL_UNSIGNED_BYTE,	//data type
		imgData);		//data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(imgData);
}