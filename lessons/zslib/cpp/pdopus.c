#include <opus/opus.h>
#include "stream.h"
#include <samplerate.h>
#include <m_pd.h>
#include <stdlib.h>
#include <math.h>

void t() {
    Streamer s = streamer_create("123", 200);
    streamer_destroy(s);
}

static t_class * opus_encode_class;

typedef struct _opus {
    t_object  x_obj;
    t_float f;
    t_outlet* x_out;
    Streamer  encoder;
    SRC_DATA samplerate_data;
} t_opus_encode;

void opus_encode_bang(t_opus_encode *x)
{
    post("Hello world !!");
}

void *opus_encode_new(void)
{
    t_opus_encode *x = (t_opus_encode *)pd_new(opus_encode_class);
    x->encoder = streamer_create("123", 200);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->samplerate_data.src_ratio = 48000.0/sys_getsr();
    x->samplerate_data.data_in = 0;
    x->samplerate_data.input_frames = sys_getblksize();
    x->samplerate_data.output_frames = ceil(x->samplerate_data.input_frames * x->samplerate_data.src_ratio);
    x->samplerate_data.data_out = calloc(x->samplerate_data.output_frames, 1);
    return (void *)x;
}

void opus_encode_free(t_opus_encode * x) {
    streamer_destroy(x->encoder);
    outlet_free(x->x_out);
    free(x->samplerate_data.data_out);
    //    resample_free(&x->resample);
}

t_int *opus_encode_tilde_perform(t_int *w) {
    t_opus_encode *x = (t_opus_encode*)(w[1]);
    t_sample *in1 = (t_sample*)(w[2]);
    t_sample *out = (t_sample*)(w[3]);
    int n = (int)(w[4]);
    while (n--) *out++ = (*in1++)* 0.5;


    //    resamplefrom_dsp(x->resample, in1, n, x->resample, 0);

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
