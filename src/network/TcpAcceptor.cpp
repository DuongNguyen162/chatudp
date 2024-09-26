#include "TcpAcceptor.h"
#include "TcpConnector.h"

TcpAcceptor::TcpAcceptor(
    boost::asio::io_context& ioContext,
    const tcp::endpoint& endpoint ) :
    m_acceptor( ioContext, endpoint )
{
}