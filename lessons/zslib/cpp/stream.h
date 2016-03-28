#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Streamer_;
typedef struct Streamer_ * Streamer;
int stream_to(const char * msg, size_t len, Streamer);

Streamer streamer_create(const char * addr, int port);
void streamer_destroy(Streamer s);

#ifdef __cplusplus
}
#endif

#endif // STREAM_H
