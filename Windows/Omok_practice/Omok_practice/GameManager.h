#pragma once

#include "Omok_practice.h"
#include "Drawing.h"
#include <stack>

class GameManager
{
public:
	GameManager();
	~GameManager();

	void initialize(int xCount, int yCount, int xInterval, int yInterval);

	void setGridInMemoryZero();

	void transToIndexValue(Stone &stone);

	void setStoneInMemory(const Stone &indexStone);

	Stone *getGridFromMemory() const; 

	bool checkWin(const Stone &indexStone);

	bool isAvailableDrawStone(const Stone &indexStone);
	
    Drawing &getDrawing();
	Drawing *getDrawingPtr();

	void drawGrid(HDC hdc);
	void drawStone(HDC hdc, const Stone &temp);

	void insertDataInStack(const Stone &indexStone);
	void clearDataOnce(int indexValue);
	int getLastDataInStack();
	void removeLastDataInStack();
	bool isStackEmpty();

	int getXcount() const;
	int getYcount() const;
	int getXinterval() const;
	int getYinterval() const;


private:

	int horizon(int x, int y, int color);

	int vertical(int x, int y, int color);

	int diagonalRight(int x, int y, int color);
	
	int diagonalLeft(int x, int y, int color);

	int getIndex(int x, int y);
	
private:

	Drawing m_drawing; // ���� �ٵϵ� ����, ��ư���� �� ȭ�鿡 �������� �κ��� ó���ϴ� Ŭ������ �ν��Ͻ�

	Stone *m_GridInMemory; // �޸�

	std::stack<int> m_dataInStack; // ����
	
	int m_xCount;
	int m_yCount;
	int m_xInterval;
	int m_yInterval;
};

