#include <boost/asio.hpp>
#include <m_pd.h>

#include "stream.h"

#define POST post

using namespace boost::asio;

struct Streamer_ {
private:
    int port_;
    ip::address addr_;
    io_service service_;
    ip::udp::socket socket_;
    ip::udp::endpoint receiver_;
public:
    Streamer_(const char * addr, int port) :
        port_(port),
        addr_(ip::address::from_string(addr)),
        service_(),
        socket_(service_),
        receiver_(addr_, port)
    {
        socket_.open(ip::udp::v4());
    }

    void send(const std::string& msg) {
        socket_.send_to(buffer(msg), receiver_);
    }

    void send(const void *data, size_t len) {
        socket_.send_to(buffer(data, len), receiver_);
    }
};

int stream_to(const char *msg, size_t len, Streamer)
{
    return 0;
}

Streamer *streamer_create(const char * addr, int port)
{
    POST("streamer_destroy(%s, %i)", addr, port);
    return new Streamer_(addr, port);
}

void streamer_destroy(Streamer *s)
{
    POST("streamer_destroy");
    delete s;
}

void streamer_sendto(Streamer *s, const void *data, size_t len)
{
    assert(s);
    s->send(data, len);
}
