#pragma once

#include <vector>
#include <WinSock2.h>

enum PacketType
{
    PacketTypePut = 1,
    PacketTypeJudgement,
    PacketTypeStoneColor,
    PacketTypeStart,
};

class Packet
{
public:
    Packet( void* data, size_t size );
    Packet( PacketType type );
    virtual ~Packet();

    void write( const void* data, size_t size );
    const char* getBuffer() const;
    int getPacketSize() const;

    void read( void* data, int size );

    PacketType getType() const;
    size_t getBodySize() const;

    static void send( SOCKET socket, const Packet& packet );

private:
    std::vector<char> m_buffer;
    size_t m_writePosition{ 0 };
    size_t m_readPosition{ 0 };

    int* m_bodySizeLocator{ nullptr };
};