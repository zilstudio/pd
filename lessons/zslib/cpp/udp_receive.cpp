#include <iostream>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cstdlib.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

void server(boost::asio::io_service& io_service, unsigned short port){
    udp::socket sock(io_service, udp::endpoint(udp::v4(), port));
    sock.set_option(boost::asio::ip::udp::socket::reuse_address(true));

    for (;;) {
        boost::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = sock.receive_from(
                    boost::asio::buffer(recv_buf), sender_endpoint);
        std::cout.write(recv_buf.data(), len);
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <port>\n";
            return boost::exit_failure;
        }

        boost::asio::io_service io_service;
        server(io_service, std::atoi(argv[1]));
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return boost::exit_success;
}

