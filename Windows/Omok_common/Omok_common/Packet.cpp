#include "stdafx.h"
#include "Packet.h"

namespace
{
    const size_t DefaultPacketSize = 128;
    const size_t DefaultHeaderSize = 8;
}

Packet::Packet( void* data, size_t size )
{
    m_buffer.reserve( size );
    memcpy( m_buffer.data(), data, size );
    m_writePosition = size - DefaultHeaderSize;
}

Packet::Packet( PacketType type )
{
    m_buffer.resize( DefaultPacketSize );
    memcpy( m_buffer.data(), &type, sizeof( type ) );

    char* p = &m_buffer[4];
    m_bodySizeLocator = reinterpret_cast<int*>(p);
}

Packet::~Packet()
{

}

void Packet::write( const void* data, size_t size )
{
    size_t pos = m_writePosition + DefaultHeaderSize;
    memcpy( m_buffer.data() + pos, data, size );
    m_writePosition += size;

    *m_bodySizeLocator = m_writePosition;
}

void Packet::read( void* data, int size )
{
    size_t pos = m_readPosition + DefaultHeaderSize;
    memcpy( data, m_buffer.data() + pos, size );

    m_readPosition += size;
}

const char* Packet::getBuffer() const
{
    return m_buffer.data();
}

int Packet::getPacketSize() const
{
    return m_writePosition + DefaultHeaderSize;
}

PacketType Packet::getType() const
{
    const char* p = m_buffer.data();
    const int* value = reinterpret_cast<const int*>(p);

    return (PacketType)*value;
}

size_t Packet::getBodySize() const
{
    return m_writePosition;
}

void Packet::send( SOCKET socket, const Packet& packet )
{
    ::send( socket, packet.getBuffer(), packet.getPacketSize(), 0 );
}