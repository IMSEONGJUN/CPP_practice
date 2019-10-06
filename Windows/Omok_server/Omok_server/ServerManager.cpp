#include "stdafx.h"
#include "ServerManager.h"
#include "Logger.h"

#pragma warning(disable:4996)


ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
}

bool ServerManager::initialize(HWND hWnd)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	m_hWnd = hWnd;
	m_gameManager.initialize(25, 25, 30, 30);
	m_gameManager.setGridInMemoryZero();

	Logger::debug("Socket and grid in memory are initialized! (hWnd:0x%p)", hWnd);

	return true;
}

void ServerManager::listen(int port)
{
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	WSAAsyncSelect(m_listenSocket, m_hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);

	SOCKADDR_IN sockAddrIn;
	ZeroMemory(&sockAddrIn, sizeof(sockAddrIn));
	sockAddrIn.sin_family = AF_INET;
	sockAddrIn.sin_port = htons(port);
	sockAddrIn.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(m_listenSocket, (sockaddr *)&sockAddrIn, sizeof(sockAddrIn));

	::listen(m_listenSocket, SOMAXCONN);

	Logger::debug("on listening! (socket:0x%p)", m_listenSocket);
}

void ServerManager::onSocketMessage(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;

	switch (lParam)
	{
	case FD_ACCEPT:
		onAccept(socket);
		break;
	case FD_CLOSE:
		onClose(socket);
		break;
	case FD_READ:
		onPacketRead(socket);
		break;
	}
}

void ServerManager::startGame()
{
	m_gameManager.setGridInMemoryZero();

	int stoneColor = 1; // 항상 흑돌이 먼저 시작
	Packet packet(PacketTypeStart);
	packet.write(&stoneColor, sizeof(stoneColor));

	broadcastPacket(packet);
}

void ServerManager::onAccept(SOCKET listenSocket)
{
	SOCKADDR addr;
	int lenAddr = sizeof(addr);

	SOCKET clientSocket = accept(listenSocket, (SOCKADDR *)&addr, &lenAddr);
	m_clientSockets.push_back(clientSocket);

	int retval = WSAAsyncSelect(clientSocket, m_hWnd,
		WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);

	Logger::debug("onAccept => client socket added: 0x%p", clientSocket);

	int stoneColor = (getConnectedClientCount() % 2) + 1;

	Packet packet(PacketTypeStoneColor);
	packet.write(&stoneColor, sizeof(stoneColor));
	Packet::send(clientSocket, packet);

	Logger::debug("sendStoneColor => sent stone color: %d", stoneColor);
}

void ServerManager::onClose(SOCKET clientSocket)
{
	auto iter = std::find(m_clientSockets.begin(), m_clientSockets.end(), clientSocket);
	m_clientSockets.erase(iter);

	Logger::debug("onClose => client socket removed: 0x%p", clientSocket);
}

void ServerManager::onPacketRead(SOCKET clientSocket)
{
	char buf[256] = { 0, };
	int received = recv(clientSocket, buf, 256, 0);

	Packet packet(buf, received);

	Stone stone;
	packet.read(&stone.x, sizeof(stone.x));
	packet.read(&stone.y, sizeof(stone.y));
	packet.read(&stone.color, sizeof(stone.color));

	m_gameManager.setStoneInMemory(stone);

	Logger::debug("onPacketRead: Put: x:%d, y:%d, color:%d", stone.x, stone.y, stone.color);

	broadcastPacket(packet);

	if (m_gameManager.checkWin(stone))
	{
		sendingWinStoneColor(stone);
	}
}

void ServerManager::broadcastPacket(const Packet& packet) const
{
	for (auto clientSocket : m_clientSockets)
	{
		Packet::send(clientSocket, packet);
	}
}

size_t ServerManager::getConnectedClientCount() const
{
	return m_clientSockets.size();
}

void ServerManager::sendingWinStoneColor(Stone stone) const
{
	int color = stone.color;
	Packet p(PacketTypeJudgement);
	p.write(&color, sizeof(color));

	::Sleep(500);

	broadcastPacket(p);

	Logger::debug("check win -> %d, broadcasted", color);
}
