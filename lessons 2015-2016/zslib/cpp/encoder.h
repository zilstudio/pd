#ifndef ENCODER_H
#define ENCODER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Encoder_;
typedef struct Encoder_ Encoder;

Encoder *encoder_create(size_t samplerate, size_t framesize);
void encoder_destroy(Encoder *enc);

int encoder_process(Encoder *enc, const float *pcm);
unsigned char *encoder_out(Encoder *enc);

#ifdef __cplusplus
}
#endif


#endif // ENCODER_H
