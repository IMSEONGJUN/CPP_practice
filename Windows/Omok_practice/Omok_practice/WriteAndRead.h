#pragma once

#include <string>
#include "Omok_practice.h"
#include "GameManager.h"


class WriteAndRead
{
public:
	WriteAndRead();
	~WriteAndRead();

	//void setDrawing(Drawing *drawing);

	void writeFile(const std::string &fileName, const GameManager &gameManager);
	void readFile(const std::string &fileName, Stone *GridInMemory);

private:

	//Drawing *m_drawing;
};

