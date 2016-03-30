#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Resampler_;
typedef struct Resampler_ Resampler;

Resampler *resampler_create(double ratio, size_t in_blocksize);
void resampler_destroy(Resampler *rs);

void resampler_set_input(Resampler *rs, float *in);
float *resampler_get_output(Resampler *rs);
size_t resampler_get_output_size(Resampler *rs);
int resampler_process(Resampler *rs);


#ifdef __cplusplus
}
#endif

#endif // RESAMPLE_H
