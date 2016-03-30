#include <m_pd.h>
#include <stdlib.h>
#include <assert.h>

static t_class * opus_decode_class;

typedef struct _opus {
    t_object  x_obj;
    t_float f;
    t_outlet* x_out;
    Resampler * resampler;
    Encoder * encoder;
    Streamer * streamer;

    // reblock
    t_sample * reblock_buf;
    size_t reblock_buf_size;
    t_sample * cur_block;
} t_opus_encode;
