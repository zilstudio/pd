#include <m_pd.h>
#include <stdlib.h>
#include <assert.h>

#include "resample.h"
#include "encoder.h"
#include "stream.h"

static t_class * opus_encode_class;

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

void opus_encode_bang(t_opus_encode *x)
{
    post("Hello world !!");
}

void *opus_encode_new(void)
{
    // INLETS
    t_opus_encode *x = (t_opus_encode *)pd_new(opus_encode_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);

    // OPUS
    x->encoder = encoder_create(48000, sys_getblksize());

    // samplerate
    x->resampler = resampler_create(48000.0/44100.0, sys_getblksize());

    // streamer
    x->streamer = streamer_create("192.168.0.90", 4321);

    // reblock
    x->reblock_buf_size = 240;
    x->reblock_buf = malloc(x->reblock_buf_size * sizeof(t_sample));
    x->cur_block = x->reblock_buf;

    return (void *)x;
}

void opus_encode_free(t_opus_encode * x) {
    free(x->reblock_buf);
    streamer_destroy(x->streamer);
    resampler_destroy(x->resampler);
    encoder_destroy(x->encoder);
    outlet_free(x->x_out);
}

t_int *opus_encode_tilde_perform(t_int *w) {
    t_opus_encode *x = (t_opus_encode*)(w[1]);
    t_sample *in1 = (t_sample*)(w[2]);
    t_sample *out = (t_sample*)(w[3]);
    int n = (int)(w[4]);

    resampler_set_input(x->resampler, in1);
    resampler_process(x->resampler);

    t_float *pcm = resampler_get_output(x->resampler);
    t_float *cur_pcm = pcm;
    size_t pcm_size = resampler_get_output_size(x->resampler);

    assert(pcm_size > sys_getblksize());

    streamer_sendto(x->streamer, in1, n * sizeof(t_sample));

//    while (n--) *out++ = (*++)* 0.5;
    return (w+5);
}

void opus_encode_dsp(t_opus_encode *x, t_signal **sp) {
    dsp_add(opus_encode_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void opus_encode_setup(void) {
    opus_encode_class = class_new(gensym("opus_encode"),
                                  (t_newmethod) opus_encode_new,
                                  (t_method) opus_encode_free,
                                  sizeof(t_opus_encode),
                                  CLASS_DEFAULT, 0);
    class_addbang(opus_encode_class, opus_encode_bang);

    class_addmethod(opus_encode_class,
                    (t_method)opus_encode_dsp, gensym("dsp"), 0);
    CLASS_MAINSIGNALIN(opus_encode_class, t_opus_encode, f);
}
