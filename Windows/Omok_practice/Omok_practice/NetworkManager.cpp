#include "stdafx.h"
#include "NetworkManager.h"
#include "Packet.h"
#include "Logger.h"
#include "GameManager.h"

#pragma warning(disable:4996)



NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::initialize(HWND hWnd, GameManager* gameManager)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	m_hWnd = hWnd;
	m_gameManagerRef = gameManager;

	return true;
}

void NetworkManager::setMyTurn(bool myTurn)
{
	m_myTurn = myTurn;
}

bool NetworkManager::isMyTurn() const
{
	return m_myTurn;
}

int NetworkManager::getInitStoneColor() const
{
	return m_initStoneColor;
}

void NetworkManager::connectToServer(const std::string& ip, int port)
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	WSAAsyncSelect(m_socket, m_hWnd, WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE);

	SOCKADDR_IN sockAddrIn;
	ZeroMemory(&sockAddrIn, sizeof(sockAddrIn));

	sockAddrIn.sin_family = AF_INET;
	sockAddrIn.sin_port = htons(port);
	sockAddrIn.sin_addr.s_addr = inet_addr(ip.c_str());

	connect(m_socket, (sockaddr *)&sockAddrIn, sizeof(sockAddrIn));
}

void NetworkManager::disconnect()
{
	closesocket(m_socket);
}

void NetworkManager::sendStoneIndex(int x, int y, int color)
{
	Logger::debug("sendStoneIndex: %d, %d, %d", x, y, color);

	Packet packet(PacketTypePut);
	packet.write(&x, sizeof(x));
	packet.write(&y, sizeof(y));
	packet.write(&color, sizeof(color));

	Packet::send(m_socket, packet);
}

void NetworkManager::sendMessage()
{
	Drawing &draw = m_gameManagerRef->getDrawing();
	HWND edit = draw.getEditbox();
	
	char* buff;
	GetWindowText(edit, buff, 256);

	m_recentylSentMsg = buff;
	
	Packet packet(PacketTypeMsg);
	packet.write(buff, strlen(buff));

	Packet::send(m_socket, packet);
}

void NetworkManager::onSocketMessage(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		MessageBox(m_hWnd, "Connected!!", "Network", MB_OK);
		break;
	case FD_CLOSE:
		MessageBox(m_hWnd, "Disconnected!!", "Network", MB_OK);
		break;
	case FD_READ:
		onPacketRead(socket);
		break;

	default:
		break;
	}
}

void NetworkManager::onPacketRead(SOCKET socket)
{
	char buf[256] = { 0, };
	int received = recv(socket, buf, 256, 0);

	Packet packet(buf, received);
	PacketType type = packet.getType();

	Logger::debug("Packet Received:%d, size:%d", type, received);

	if (type == PacketTypeStart)
	{
		onStartMsgRecv(packet);
	}
	else if (type == PacketTypeSetBeginnigStoneColor)
	{
		setStoneColor(packet);
	}
	else if (type == PacketTypePut)
	{
		whenTypeIsPut(packet);
	}
	else if (type == PacketTypeJudgement)
	{
		onJudgementMsgRecv(packet);
	}
	else if (type == PacketTypeDeleteIndex)
	{
		onDeleteMsgRecv(packet);
	}
	else if (type == PacketTypeNoStones)
	{
		MessageBox(m_hWnd, "no stones", "Message", MB_OK);
	}
	else if (PacketTypeMsg)
	{
		printMessageOnMyList(packet);
	}
}

void NetworkManager::onStartMsgRecv(Packet& packet)
{
	int color;
	packet.read(&color, sizeof(color));
	setMyTurn(color == m_initStoneColor);
}

void NetworkManager::setStoneColor(Packet& packet)
{
	int temp;
	packet.read(&temp, sizeof(temp));
	m_initStoneColor = temp;
}

void NetworkManager::whenTypeIsPut(Packet& packet)
{
	Stone stone;
	packet.read(&stone.x, sizeof(stone.x));
	packet.read(&stone.y, sizeof(stone.y));
	packet.read(&stone.color, sizeof(stone.color));

	if (stone.color != m_initStoneColor)
	{
		setMyTurn(true);
	}

	m_gameManagerRef->setStoneInMemory(stone);
	InvalidateRect(m_hWnd, NULL, TRUE);

	Logger::debug("onPacketRead: %d, %d, %d", stone.x, stone.y, stone.color);
}

void NetworkManager::onJudgementMsgRecv(Packet& packet)
{
	int winColor;
	packet.read(&winColor, sizeof(winColor));

	Logger::debug("Judgement -> %d", winColor);

	if (winColor == m_initStoneColor)
		MessageBox(m_hWnd, "You Win", "Message", MB_OK);
	else
		MessageBox(m_hWnd, "You Lose", "Message", MB_OK);
}

void NetworkManager::sendPacketTypeDelete()
{
	Packet packet(PacketTypeDeleteColor);
	packet.write(&m_initStoneColor, sizeof(m_initStoneColor));
	Packet::send(m_socket, packet);
}

void NetworkManager::onDeleteMsgRecv(Packet& packet)
{
	int deleteIndex;
	packet.read(&deleteIndex, sizeof(deleteIndex));
	
	Stone* temp = m_gameManagerRef->getGridFromMemory();
	temp[deleteIndex].x = 0;
	temp[deleteIndex].y = 0;
	temp[deleteIndex].color = 0;

	InvalidateRect(m_hWnd, NULL, TRUE);
}

void NetworkManager::printMessageOnMyList(Packet& packet)
{
	std::string buffer;
	packet.read(&buffer, sizeof(buffer));
	if (buffer == m_recentylSentMsg)
	{
		Drawing draw = m_gameManagerRef->getDrawing();
		HWND list = draw.getListbox();
		
		m_recentylSentMsg = "";
	}
	else
	{

	}
}