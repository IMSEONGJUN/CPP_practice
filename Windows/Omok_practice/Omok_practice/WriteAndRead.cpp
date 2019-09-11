#include "stdafx.h"
#include "WriteAndRead.h"


WriteAndRead::WriteAndRead()
{
}


WriteAndRead::~WriteAndRead()
{
}

//void WriteAndRead::setDrawing(Drawing *drawing)
//{
//	m_drawing = drawing;
//}

void WriteAndRead::writeFile(const std::string &fileName, const GameManager &gameManager)
{
	FILE *fp = nullptr;

	fopen_s(&fp, fileName.c_str(), "wb");

	if (fp == nullptr)
	{
		return;
	}

	int gridCount = gameManager.getXcount() * gameManager.getYcount();

	fwrite(&gridCount, sizeof(gridCount), 1, fp);
	
	Stone *GridInMemory = gameManager.getGridFromMemory();

	for (int i = 0; i < gridCount; i++)
	{
		fwrite(&(GridInMemory[i].x), sizeof(GridInMemory[i].x), 1, fp);
		fwrite(&(GridInMemory[i].y), sizeof(GridInMemory[i].y), 1, fp);
		fwrite(&(GridInMemory[i].color), sizeof(GridInMemory[i].color), 1, fp);

	}

	fclose(fp);
}

void WriteAndRead::readFile(const std::string &fileName, Stone *GridInMemory)
{
	FILE *fp = nullptr;

	fopen_s(&fp, fileName.c_str(), "rb");

	if (fp == nullptr)
	{
		return;
	}

	int arrCount;
	fread(&arrCount, sizeof(arrCount), 1, fp);

	for (int i = 0; i < arrCount; i++)
	{
		Stone temp;

		fread(&temp.x, sizeof(temp.x), 1, fp);
		fread(&temp.y, sizeof(temp.y), 1, fp);
		fread(&temp.color, sizeof(temp.color), 1, fp);

		GridInMemory[i] = temp;
	}

	fclose(fp);
}