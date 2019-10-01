#include "stdafx.h"
#include "NetworkManager.h"


#pragma warning(disable:4996)



NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::initialize( HWND hWnd )
{
    WSADATA wsaData;
    if( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
        return false;

    m_hWnd = hWnd;

    return true;
}

void NetworkManager::connectToServer( const std::string& ip, int port )
{
    m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    WSAAsyncSelect( m_socket, m_hWnd, WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE );

    SOCKADDR_IN sockAddrIn;
    ZeroMemory( &sockAddrIn, sizeof( sockAddrIn ) );

    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_port = htons( port );
    sockAddrIn.sin_addr.s_addr = inet_addr( ip.c_str() );

    connect( m_socket, (sockaddr *)&sockAddrIn, sizeof( sockAddrIn ) );
}

void NetworkManager::onSocketMessage( WPARAM wParam, LPARAM lParam )
{
    SOCKET socket = (SOCKET)wParam;

    switch( WSAGETSELECTEVENT( lParam ) )
    {
    case FD_CONNECT:
        MessageBox(m_hWnd, "Connected!!", "Network", MB_OK );
        break;
    case FD_CLOSE:
        MessageBox( m_hWnd, "Disconnected!!", "Network", MB_OK );
        break;


    default:
        break;
    }
}
