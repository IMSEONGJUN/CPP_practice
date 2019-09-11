#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
}


GameManager::~GameManager()
{
	delete[] m_GridInMemory;
}


void GameManager::initialize(int xCount, int yCount, int xInterval, int yInterval)
{
	m_xCount = xCount;
	m_yCount = yCount;
	m_xInterval = xInterval;
	m_yInterval = yInterval;

	m_GridInMemory = new Stone[xCount * yCount];
}

void GameManager::insertDataInStack(const Stone &indexStone)
{
	int index = indexStone.y * m_xCount + indexStone.x;

	m_dataInStack.push(index);
}

int GameManager::getLastDataInStack()
{
	if (!m_dataInStack.empty())
	{
		return m_dataInStack.top();
	}

    //리턴0이 0을 보내는건지 함수 종료인지? 0을 보내려는 의도로 쓴것
	return 0; 
}

bool GameManager::isStackEmpty()
{


	return m_dataInStack.empty();
}

void GameManager::removeLastDataInStack()
{
	m_dataInStack.pop();
}

void GameManager::clearDataOnce(int indexValue)
{
	m_GridInMemory[indexValue].x = 0;
	m_GridInMemory[indexValue].y = 0;
	m_GridInMemory[indexValue].color = 0;
}

int GameManager::getXcount() const
{
	return m_xCount;
}

int GameManager::getYcount() const
{
	return m_yCount;
}

int GameManager::getXinterval() const
{
	return m_xInterval;
}

int GameManager::getYinterval() const
{
	return m_yInterval;
}


Drawing *GameManager::getDrawingPtr()
{
	return &m_drawing;
}

Drawing &GameManager::getDrawing()
{
	return m_drawing;
}

void GameManager::setGridInMemoryZero()
{
	for (int i = 0; i < getXcount() * getYcount(); i++)
	{
		m_GridInMemory[i].x = 0;
		m_GridInMemory[i].y = 0;
		m_GridInMemory[i].color = 0;
	}
}

void GameManager::transToIndexValue(Stone &stone)
{
	float oldX = (stone.x - getXinterval()) / (float)getXinterval();
	float oldY = (stone.y - getYinterval()) / (float)getYinterval();

	int newX = oldX < 0.0f ? -1 : oldX + 0.5f;
	int newY = oldY < 0.0f ? -1 : oldY + 0.5f;

	stone.x = newX;
	stone.y = newY;
	stone.color = stone.color;
}

void GameManager::setStoneInMemory(const Stone &indexStone)
{
	int address = indexStone.y * getXcount() + indexStone.x; // x, y 좌표는 인덱스 값이고, width값은 1부터 시작하는 정수값이다. 
	                                                         //바둑판에 세로줄이 1부터시작이고 인덱스는 그 위치가 0이다.
	m_GridInMemory[address].x = indexStone.x;
	m_GridInMemory[address].y = indexStone.y;
	m_GridInMemory[address].color = indexStone.color;

}

Stone *GameManager::getGridFromMemory() const
{
	return m_GridInMemory;
}

bool GameManager::isAvailableDrawStone(const Stone &indexStone)
{
	return (m_GridInMemory[indexStone.y * getXcount() + indexStone.x].color == 0
	&& indexStone.x < getXcount() && indexStone.x >= 0 && indexStone.y < getYcount() && indexStone.y >= 0);// 여기 질문
}

int GameManager::horizon(int x, int y, int color)
{
	int stoneCountHorizon = 0;

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x + i, y)].color == color)
		{
			stoneCountHorizon++;
		}
		else if (m_GridInMemory[getIndex(x + i, y)].color != color)
		{
			break;
		}
		if (x + i > getXcount())
		{
			break;
		}

	}

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x - i, y)].color == color)
		{
			stoneCountHorizon++;
		}
		else if (m_GridInMemory[getIndex(x - i, y)].color != color)
		{
			break;
		}
		if (x - i < 0)
		{
			break;
		}

	}

	return stoneCountHorizon;
}

int GameManager::vertical(int x, int y, int color)
{
	int stoneCountVertical = 0;

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x, y - i)].color == color)
		{
			stoneCountVertical++;
		}
		else if (m_GridInMemory[getIndex(x, y - i)].color != color)
		{
			break;
		}
		if (y - i < 0)
		{
			break;
		}

	}

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x, y + i)].color == color)
		{
			stoneCountVertical++;
		}
		else if (m_GridInMemory[getIndex(x, y + i)].color != color)
		{
			break;
		}
		if (y + i > getYcount())
		{
			break;
		}

	}
	return stoneCountVertical;
}

int GameManager::diagonalRight(int x, int y, int color)
{
	int stoneCountDiagonalRight = 0;

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x + i, y - i)].color == color)
		{
			stoneCountDiagonalRight++;
		}
		else if (m_GridInMemory[getIndex(x + i, y - i)].color != color)
		{
			break; 
		}
		if (y - i < 0 || x + i > getXcount())
		{
			break;
		}

	}

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x - i, y + i)].color == color)
		{
			stoneCountDiagonalRight++;
		}
		else if (m_GridInMemory[getIndex(x - i, y + i)].color != color)
		{
			break;
		}
		if (y + i > getYcount() || x - i < 0)
		{
			break;
		}

	}
	return stoneCountDiagonalRight;
}

int GameManager::diagonalLeft(int x, int y, int color)
{
	int stoneCountDiagonalLeft = 0;

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x - i, y - i)].color == color)
		{
			stoneCountDiagonalLeft++;
		}
		else if (m_GridInMemory[getIndex(x - i, y - i)].color != color)
		{
			break;
		}
		if (y - i < 0 || x - i < 0)
		{
			break;
		}

	}

	for (int i = 1; i < 5; i++)
	{
		if (m_GridInMemory[getIndex(x + i, y + i)].color == color)
		{
			stoneCountDiagonalLeft++;
		}
		else if (m_GridInMemory[getIndex(x + i, y + i)].color != color)
		{
			break;
		}
		if (y + i > getYcount() || x + i > getXcount())
		{
			break;
		}

	}

	return stoneCountDiagonalLeft;
}

bool GameManager::checkWin(const Stone &indexStone)
{
	
	int x = indexStone.x;
	int y = indexStone.y;
	int color = indexStone.color;
	
	int stoneCountHorizon = horizon(x,y,color);
	int stoneCountVertical = vertical(x,y,color);
	int stoneCountDiagonalRight = diagonalRight(x,y,color);
	int stoneCountDiagonalLeft = diagonalLeft(x,y,color);

	return (stoneCountHorizon >= 4 || stoneCountVertical >= 4 ||
		stoneCountDiagonalRight >= 4 || stoneCountDiagonalLeft >= 4);

}

int GameManager::getIndex(int x, int y)
{
	return y * getXcount() + x;
}

void GameManager::drawGrid(HDC hdc)
{
	m_drawing.drawGrid(hdc, m_xCount, m_yCount, m_xInterval, m_yInterval);
}

void GameManager::drawStone(HDC hdc, const Stone &temp)
{
	m_drawing.drawStone2(hdc, temp.x, temp.y, temp.color, m_xInterval, m_yInterval);
}