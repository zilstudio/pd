//#include <boost/asio.hpp>
#include "stream.h"
#include <m_pd.h>

//using namespace boost::asio;

struct Streamer_ {
    int port_;
    Streamer_(int port) : port_(port) {
        post("CREATE");
    }

    bool test( ) {
        return true;
    }
};

int stream_to(const char *msg, size_t len, Streamer)
{
    return 0;
}

Streamer streamer_create(const char * addr, int port)
{
    return new Streamer_(port);
}

void streamer_destroy(Streamer s)
{
    post("DESTROY");
    delete s;
}
