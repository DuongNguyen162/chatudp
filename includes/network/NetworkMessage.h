/**
 * MinigameServer - a simple flip card game server
 * Copyright (C) 2020  clucle <wjdenwls123@gmail.com>
 * Github : http://github.com/clucle
 */

#ifndef NETWORK_MESSAGE_HPP
#define NETWORK_MESSAGE_HPP

#include <string>
#include <cstdio>
#include <cstring>
#include "core/types.h"

 /**
  * @file NetworkMessage.h
  * @class NetworkMessage
  * @author clucle (github.com/clucle)
  * @brief ��Ŷ ��ü�� ���� ������ �����ϴ� Ŭ����
  * ��Ŷ�� ���� ���� �� �ֵ��� �Լ��� �����ϰ�
  * ������� ��Ŷ�� ���� �� �ְ� �Ѵ�
  *
  */

class NetworkMessage
{
public:
	NetworkMessage();

	/* ---------  Packet Info --------- */
	// [HEADER_LENGTH][MAX_BODY_LENGTH] //
	enum { BODY_LENGTH = 4 };
	enum { HEADER_LENGTH = BODY_LENGTH };
	enum { MAX_BODY_LENGTH = 2048 };

	// ����迭 + �ٵ�迭 ���� (���� �Ұ�)
	const char* GetData() const;
	// ����迭 + �ٵ�迭 ���� (���� ����)
	char* GetData();

	// ����� �ٵ� ��ģ ���� ����
	size_t GetLength() const;

	// �ٵ� �迭 ���� (���� �Ұ�)
	const char* GetBody() const;
	// �ٵ� �迭 ���� (���� ����)
	char* GetBody();
	// �ٵ� ���� ����
	size_t GetBodyLength() const;

	// ��Ŷ -> ���� : ��Ŷ���� ������ �� (���ڵ�)
	bool DecodeHeader();
	// ���� -> ��Ŷ : ������ ��Ŷ���� ���� (���ڵ�)
	void EncodeHeader();

	// ��Ŷ �б� ������ �ʱ�ȭ
	void ReadyToRead();
	// �ڷ��� ���缭 �б�
	template<typename T>
	T Read()
	{
		if ( !_CanRead( sizeof( T ) ) )
		{
			return T();
		}

		T v;
		memcpy( &v, m_data + HEADER_LENGTH + m_readPosition, sizeof( T ) );
		m_readPosition += sizeof( T );
		return v;
	}
	// 1 ����Ʈ�� �б�
	UInt8 ReadByte();
	// PREFIX_BYTE_FOR_STRING �� ���� �� �� ���̸�ŭ ���� ��Ʈ�� ���
	std::string ReadString( uint16_t stringLen = 0 );

	/**
	 * @brief ��Ŷ�� �⺻ �ڷ��� ������ �߰���Ŵ
	 * param T value : ��Ŷ�� �߰��� ��
	 * @return bool : �߰��� ���� �ߴ��� ����
	 */
	template<typename T>
	bool Append( T value )
	{
		if ( !_CanAdd( sizeof( T ) ) )
		{
			// �߰��� �ڷ����� ���̰� �������� �ʴٸ� ����
			return false;
		}

		// �� �ڸ��� T value�� �߰� ���� Length�� T ���� ��ŭ �߰�
		memcpy( m_data + HEADER_LENGTH + m_bodyLength, &value, sizeof( T ) );
		m_bodyLength += sizeof( T );
		return true;
	}

	// 1Byte �� ��Ŷ�� �߰�
	bool AppendByte( UInt8 value );
	// string �� ��Ŷ�� �߰�
	bool AppendString( const std::string& value );

private:
	// string �� �߰��Ҷ��� ��������� �տ� ����ΰ� �����
	// [n][n ���� ��Ʈ��]
	const int PREFIX_BYTE_FOR_STRING = sizeof( UInt16 );

	// ���� ������ char �迭�� ����
	char m_data[ HEADER_LENGTH + MAX_BODY_LENGTH ];
	// ���ڵ�, ���ڵ��� ����� body�� ����
	std::size_t m_bodyLength;

	// ������ ���� �ڸ��� ��Ŷ�� �߰� �� �� �ִ��� Ȯ��
	bool _CanAdd( size_t size ) const;
	// ���� ���̸�ŭ�� ���۰� �ִ��� Ȯ��
	bool _CanRead( size_t size ) const;

	// �޾ƿ� ��Ŷ���� �Ľ��ϱ� ���� �ʿ��� ������
	int m_readPosition;
};

#endif