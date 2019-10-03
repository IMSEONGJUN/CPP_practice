#pragma once

class GameManager
{
public:
    GameManager();
    ~GameManager();
    void initialize( int xCount, int yCount, int xInterval, int yInterval );
    void setGridInMemoryZero();
    void setStoneInMemory( const Stone &indexStone );
    bool checkWin( const Stone &indexStone );

    int getXcount() const;
    int getYcount() const;
    int getXinterval() const;
    int getYinterval() const;

private:

    int horizon( int x, int y, int color );

    int vertical( int x, int y, int color );

    int diagonalRight( int x, int y, int color );

    int diagonalLeft( int x, int y, int color );

    int getIndex( int x, int y );

private:
    Stone *m_GridInMemory;

    int m_xCount;
    int m_yCount;
    int m_xInterval;
    int m_yInterval;
};

