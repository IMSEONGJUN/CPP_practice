#pragma once

#include <string>


class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    bool initialize( HWND hWnd );
    void connectToServer( const std::string& ip, int port );

    void onSocketMessage( WPARAM wParam, LPARAM lParam );

private:
    HWND m_hWnd;
    SOCKET m_socket;
};

