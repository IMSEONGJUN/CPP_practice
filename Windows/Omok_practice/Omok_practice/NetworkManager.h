#pragma once

#include <string>
#include "Packet.h"
#include <string>
#include <Windows.h>

class GameManager;

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	bool initialize(HWND hWnd, GameManager* gameManager);
	void connectToServer(const std::string& ip, int port);
	void disconnect();
	void setMyTurn(bool myTurn);
	bool isMyTurn() const;

	void sendStoneIndex(int x, int y, int color);
	void sendMessage();

	void onSocketMessage(WPARAM wParam, LPARAM lParam);
	void onPacketRead(SOCKET socket);
	void sendPacketTypeDelete();
	int getInitStoneColor() const;

private:
	void onDeleteMsgRecv(Packet& packet);
	void onStartMsgRecv(Packet& packet);
	void setBeginningStoneColor(Packet& packet);
	void whenTypeIsPut(Packet& packet);
	void onJudgementMsgRecv(Packet& packet);
	void printMessageOnMyList(Packet& packet);

private:
	HWND m_hWnd;
	SOCKET m_socket;
	GameManager* m_gameManagerRef;
	int m_initStoneColor;
	bool m_myTurn{ false };
};

