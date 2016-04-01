#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Streamer_;
typedef struct Streamer_ Streamer;

Streamer *streamer_create(const char * addr, int port);
void streamer_destroy(Streamer *s);

void streamer_sendto(Streamer *s, const void *data, size_t len);

struct StreamClient_;
typedef struct StreamClient_ StreamClient;

StreamClient *stream_client_create(int port);
void stream_client_destroy(StreamClient *s);

#ifdef __cplusplus
}
#endif

#endif // STREAM_H
