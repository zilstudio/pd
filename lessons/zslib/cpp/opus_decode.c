#include <m_pd.h>
#include <stdlib.h>
#include <assert.h>

#include "stream.h"

static t_class * opus_decode_class;

typedef struct _opus {
    t_object  x_obj;
    t_outlet *x_out;
    StreamClient *client;
} t_opus_decode;

void *opus_decode_new(void) {
    t_opus_decode *x = (t_opus_decode *)pd_new(opus_decode_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->client = stream_client_create(4321);
    return (void *)x;
}

t_int *opus_encode_tilde_perform(t_int *w) {
    t_opus_decode *x = (t_opus_decode*)(w[1]);
    t_sample *out = (t_sample*)(w[2]);
    int n = (int)(w[3]);

    return (w+4);
}

void opus_decode_free(t_opus_decode * x) {
    stream_client_destroy(x->client);
    outlet_free(x->x_out);
}

void opus_decode_dsp(t_opus_decode *x, t_signal **sp) {
    dsp_add(opus_encode_tilde_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

void opus_decode_setup(void) {
    opus_decode_class = class_new(gensym("opus_decode"),
                                  (t_newmethod) opus_decode_new,
                                  (t_method) opus_decode_free,
                                  sizeof(t_opus_decode),
                                  CLASS_DEFAULT, 0);
//    class_addbang(opus_encode_class, opus_encode_bang);

    class_addmethod(opus_decode_class,
                    (t_method)opus_decode_dsp, gensym("dsp"), 0);
}
