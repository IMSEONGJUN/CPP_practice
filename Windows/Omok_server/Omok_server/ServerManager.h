#pragma once

#include <vector>
#include "Packet.h"
#include "GameManager.h"

class ServerManager
{
public:
	ServerManager();
	~ServerManager();

	bool initialize(HWND hWnd);
	void listen(int port);
	void onSocketMessage(WPARAM wParam, LPARAM lParam);
	void startGame();
	size_t getConnectedClientCount() const;

private:
	void onAccept(SOCKET listenSocket);
	void onClose(SOCKET clientSocket);
	void onPacketRead(SOCKET clientSocket);
	void broadcastPacket(const Packet& packet) const;
	void sendingWinStoneColor(Stone stone) const;
	void putStoneInMemAndSendtoClient(Packet& packet);
	void checkDeleteStoneColorAndDelete(Packet& packet);
	

private:
	HWND m_hWnd;
	SOCKET m_listenSocket;
	std::vector<SOCKET> m_clientSockets;
	GameManager m_gameManager;
};

