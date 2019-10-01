#pragma once

#include <vector>

enum PacketType
{
    Conversation = 0,
    SystemMessage,
    NickNameSetting,
    GenderSetting,
};

class Packet
{
public:
    Packet( void* data, size_t size );
    Packet( PacketType type );
    virtual ~Packet();

    void write( const void* data, size_t size );
    char* getBuffer();
    int getPacketSize() const;

    void read( void* data, int size );

    PacketType getType() const;
    size_t getBodySize() const;

private:
    std::vector<char> m_buffer;
    size_t m_writePosition{ 0 };
    size_t m_readPosition{ 0 };

    int* m_bodySizeLocator{ nullptr };
};