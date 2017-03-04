#include <iostream>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cstdlib.hpp>
#include <opus/opus.h>
#include <assert.h>

using boost::asio::ip::udp;

enum { max_length = 1024 };

void server(boost::asio::io_service& io_service, unsigned short port, OpusDecoder * dec){
    udp::socket sock(io_service, udp::endpoint(udp::v4(), port));
    sock.set_option(boost::asio::ip::udp::socket::reuse_address(true));

    opus_int16 pcm[240];

    for (;;) {
        boost::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = sock.receive_from(
                    boost::asio::buffer(recv_buf), sender_endpoint);

        int bytes = opus_decode(dec, (unsigned char*)recv_buf.data(), len, pcm, 240, 0);
        std::cout.write((char*)pcm, bytes * sizeof(opus_int16));

        //        std::cout.write(recv_buf.data(), len);
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <port>\n";
            return boost::exit_failure;
        }

        int err = 0;
        OpusDecoder * decoder = opus_decoder_create(48000, 1, &err);
        assert(err == 0);

        boost::asio::io_service io_service;
        server(io_service, std::atoi(argv[1]), decoder);
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return boost::exit_success;
}

