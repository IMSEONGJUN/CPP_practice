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

void GameManager::setGridInMemoryZero()
{
	for (int i = 0; i < getXcount() * getYcount(); i++)
	{
		m_GridInMemory[i].x = 0;
		m_GridInMemory[i].y = 0;
		m_GridInMemory[i].color = 0;
	}
}

void GameManager::setStoneInMemory(const Stone &indexStone)
{
	int address = indexStone.y * getXcount() + indexStone.x;

	m_GridInMemory[address].x = indexStone.x;
	m_GridInMemory[address].y = indexStone.y;
	m_GridInMemory[address].color = indexStone.color;
}

int GameManager::getIndex(int x, int y)
{
	return y * getXcount() + x;
}

int GameManager::horizon(int x, int y, int color)
{
	int stoneCountHorizon = 0;

	for (int i = 1; i < 5; i++)
	{
		if (x + i > getXcount())
		{
			break;
		}

		if (m_GridInMemory[getIndex(x + i, y)].color == color)
		{
			stoneCountHorizon++;
		}
		else if (m_GridInMemory[getIndex(x + i, y)].color != color)
		{
			break;
		}


	}

	for (int i = 1; i < 5; i++)
	{
		if (x - i < 0)
		{
			break;
		}

		if (m_GridInMemory[getIndex(x - i, y)].color == color)
		{
			stoneCountHorizon++;
		}
		else if (m_GridInMemory[getIndex(x - i, y)].color != color)
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
		if (y - i < 0)
		{
			break;
		}

		if (m_GridInMemory[getIndex(x, y - i)].color == color)
		{
			stoneCountVertical++;
		}
		else if (m_GridInMemory[getIndex(x, y - i)].color != color)
		{
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		if (y + i > getYcount())
		{
			break;
		}

		if (m_GridInMemory[getIndex(x, y + i)].color == color)
		{
			stoneCountVertical++;
		}
		else if (m_GridInMemory[getIndex(x, y + i)].color != color)
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
		if (y - i < 0 || x + i > getXcount())
		{
			break;
		}

		if (m_GridInMemory[getIndex(x + i, y - i)].color == color)
		{
			stoneCountDiagonalRight++;
		}
		else if (m_GridInMemory[getIndex(x + i, y - i)].color != color)
		{
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		if (y + i > getYcount() || x - i < 0)
		{
			break;
		}

		if (m_GridInMemory[getIndex(x - i, y + i)].color == color)
		{
			stoneCountDiagonalRight++;
		}
		else if (m_GridInMemory[getIndex(x - i, y + i)].color != color)
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
		if (y - i < 0 || x - i < 0)
		{
			break;
		}

		if (m_GridInMemory[getIndex(x - i, y - i)].color == color)
		{
			stoneCountDiagonalLeft++;
		}
		else if (m_GridInMemory[getIndex(x - i, y - i)].color != color)
		{
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		if (y + i > getYcount() || x + i > getXcount())
		{
			break;
		}

		if (m_GridInMemory[getIndex(x + i, y + i)].color == color)
		{
			stoneCountDiagonalLeft++;
		}
		else if (m_GridInMemory[getIndex(x + i, y + i)].color != color)
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

	int stoneCountHorizon = horizon(x, y, color);
	int stoneCountVertical = vertical(x, y, color);
	int stoneCountDiagonalRight = diagonalRight(x, y, color);
	int stoneCountDiagonalLeft = diagonalLeft(x, y, color);

	return (stoneCountHorizon >= 4 || stoneCountVertical >= 4 ||
		stoneCountDiagonalRight >= 4 || stoneCountDiagonalLeft >= 4);

}

Stone* GameManager::getGridInMemory() const
{
	return m_GridInMemory;
}

void GameManager::insertDataInStack(const Stone& stone)
{
	int index = stone.y * getXcount() + stone.x;
	m_stackData.push(index);
}

int GameManager::getLastDataInStack()
{
	if (!isStackEmpty())
	{
		return m_stackData.top();
	}
	else
	{
		return -1;
	}
}

void GameManager::removeTopDataInStackOnce()
{
	m_stackData.pop();
}

void GameManager::deleteOneStoneInMemory(int index)
{
	m_GridInMemory[index].x = 0;
	m_GridInMemory[index].y = 0;
	m_GridInMemory[index].color = 0;
}

bool GameManager::isStackEmpty()
{
	return m_stackData.empty();
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