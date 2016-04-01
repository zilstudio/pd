#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

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

struct StreamClient_ {
private:
    int port_;
    io_service service_;
    ip::udp::socket socket_;
    ip::udp::endpoint endpoint_;
    boost::array<char, 1024> recv_buffer_;
    boost::scoped_ptr<boost::thread> thread;
public:
    StreamClient_(int port) :
        port_(port),
        service_(),
        socket_(service_, ip::udp::endpoint(ip::udp::v4(), port_))
    {
        start();
        do_receive();
        //        service_.run();
    }

    ~StreamClient_() {
       stop();
    }

    void start() {
        if (thread) return; // running

        thread.reset(new boost::thread(
                         boost::bind(&boost::asio::io_service::run, &service_)));
    }

    void stop() {
        if (!thread) return; // stopped

        service_.stop();
        thread->join();
        service_.reset();
        thread.reset();
    }

    void do_receive() {
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_), endpoint_,
                                   boost::bind(&StreamClient_::handle_receive, this,
                                               boost::asio::placeholders::error,
                                               boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
    {
        post("get packet: %ld bytes", bytes_transferred);

        //        std::cout << "Received: '" << std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred) << "'\n";

        if (!error || error == boost::asio::error::message_size)
            do_receive();
    }
};

StreamClient *stream_client_create(int port)
{
    try {
        return new StreamClient(port);
    }
    catch(boost::system::system_error& e) {
        ::error("[opus] error while connecting to UDP port %i: \"%s\"", port, e.what());
        return 0;
    }
}

void stream_client_destroy(StreamClient *s)
{
    delete s;
}
