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

#ifdef __cplusplus
}
#endif

#endif // STREAM_H
