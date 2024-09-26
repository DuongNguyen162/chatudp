#define _CRT_SECURE_NO_WARNINGS

#include "NetworkMessage.h"
#include <cstring>
NetworkMessage::NetworkMessage()
	: m_data(), m_bodyLength( 0 ), m_readPosition( 0 )
{
}

const char* NetworkMessage::GetData() const
{
	return m_data;
}

char* NetworkMessage::GetData()
{
	return m_data;
}

size_t NetworkMessage::GetLength() const
{
	size_t length = HEADER_LENGTH + m_bodyLength;
	return length;
}


const char* NetworkMessage::GetBody() const
{
	const char* body = m_data + HEADER_LENGTH;
	return body;
}


char* NetworkMessage::GetBody()
{
	char* body = m_data + HEADER_LENGTH;
	return body;
}


size_t NetworkMessage::GetBodyLength() const
{
	return m_bodyLength;
}

bool NetworkMessage::DecodeHeader()
{
	char header[ HEADER_LENGTH + 1 ] = "";
	strncat( header, m_data, HEADER_LENGTH );
	m_bodyLength = atoi( header );

	bool success = true;
	if ( m_bodyLength > MAX_BODY_LENGTH )
	{
		m_bodyLength = 0;
		success = false;
	}
	return success;
}


void NetworkMessage::EncodeHeader()
{
	char header[ HEADER_LENGTH + 1 ] = "";
	sprintf( header, "%4d", static_cast<int>( m_bodyLength ) );
	memcpy( m_data, header, HEADER_LENGTH );
}


void NetworkMessage::ReadyToRead()
{
	m_readPosition = 0;
}


UInt8 NetworkMessage::ReadByte()
{
	if ( !_CanRead( 1 ) )
	{
		return 0;
	}
	return m_data[ HEADER_LENGTH + m_readPosition++ ];
}



std::string NetworkMessage::ReadString( uint16_t stringLen )
{
	if ( stringLen == 0 )
	{
		stringLen = Read<uint16_t>();
	}

	if ( !_CanRead( stringLen ) )
	{
		return std::string();
	}

	char* body = reinterpret_cast<char*>( m_data ) + HEADER_LENGTH + m_readPosition; //does not break strict aliasing
	m_readPosition += stringLen;
	return std::string( body, stringLen );
}


bool NetworkMessage::AppendByte( UInt8 value )
{
	if ( !_CanAdd( 1 ) )
	{
		return false;
	}
	m_data[ HEADER_LENGTH + m_bodyLength++ ] = value;
	return true;
}


bool NetworkMessage::AppendString( const std::string& value )
{
	size_t stringLength = value.length();
	if ( !_CanAdd( stringLength + PREFIX_BYTE_FOR_STRING ) )
	{
		return false;
	}
	Append<UInt16>( static_cast<UInt16>( stringLength ) );
	memcpy( m_data + HEADER_LENGTH + m_bodyLength, value.c_str(), stringLength );
	m_bodyLength += stringLength;

	return true;
}


bool NetworkMessage::_CanAdd( size_t size ) const
{
	return ( m_bodyLength + size ) < MAX_BODY_LENGTH;
}

bool NetworkMessage::_CanRead( size_t size ) const
{
	if ( ( m_readPosition + size ) > m_bodyLength )
	{
		return false;
	}
	return true;
}
