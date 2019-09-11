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

	bool checkWin(const Stone &indexStone); // 레퍼런스로 받아서 수정하면 본체가 수정되기 때문에 그것을 방지하기 위해 const레퍼런스로
	                                         // 받아서 인자를 수정하지 않겠다는 것을 게런티

	bool isAvailableDrawStone(const Stone &indexStone); // 이 뒤에 const 붙으면 받는 파라미터의 인자값도 수정하지 않고
	                                                    // 이 함수 내부에서 멤버변수들의 수정도 없다는 것을 게런티해줌
	Drawing &getDrawing();

	//const Drawing &getDrawing(); // 이 함수 내에서는 멤버변수에 수정을 가할 수 있으나, 리턴값으로 받아지는 것은 const 레퍼런스로 받기 때문에 수정이 불가능하다.

	const Drawing *getDrawingPtr() const; // 함수 내에서도 멤버변수 수정안되고 리턴값도 수정안되고
	Drawing *getDrawingPtr(); // 함수 내부에서 수정가능 리턴값도 수정가능

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

	Stone *m_GridInMemory; // 메모리

	std::stack<int> m_dataInStack; // 스택
	
	Drawing m_drawing; // 선과 바둑돌 세팅, 버튼세팅 등 화면에 보여지는 부분을 처리하는 클래스의 인스턴스


	int m_xCount;
	int m_yCount;
	int m_xInterval;
	int m_yInterval;
};

