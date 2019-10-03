#pragma once

#include <string>


class GameManager;

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    bool initialize( HWND hWnd, GameManager* gameManager );
    void connectToServer( const std::string& ip, int port );
    void disconnect();
    void setMyTurn(bool myTurn);
    bool isMyTurn() const;
    void sendStoneIndex( int x, int y, int color );

    void onSocketMessage( WPARAM wParam, LPARAM lParam );
    void onPacketRead( SOCKET socket );

    int getInitStoneColor() const;

private:
    HWND m_hWnd;
    SOCKET m_socket;
    GameManager* m_gameManagerRef;
    int m_initStoneColor;
    bool m_myTurn{false};
};

