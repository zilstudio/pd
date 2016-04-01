#include <m_pd.h>
#include <stdlib.h>
#include <stdio.h>

#include "stream.h"

static t_class * udp_send_broadcast_tilde_class;

typedef struct _opus {
    t_object  x_obj;
    t_float f;
    Streamer * streamer;
} t_udp_send_broadcast_tilde;

void *udp_send_broadcast_tilde_new(t_floatarg port)
{
    // INLETS
    t_udp_send_broadcast_tilde *x = (t_udp_send_broadcast_tilde *)pd_new(udp_send_broadcast_tilde_class);
    outlet_new(&x->x_obj, &s_float);

    x->streamer = streamer_create("255.255.255.255", port);

    return (void *)x;
}

void udp_send_broadcast_tilde_free(t_udp_send_broadcast_tilde * x) {
    streamer_destroy(x->streamer);
}

t_int *udp_send_broadcast_tilde_perform(t_int *w) {
    t_udp_send_broadcast_tilde *x = (t_udp_send_broadcast_tilde*)(w[1]);
    t_sample *in1 = (t_sample*)(w[2]);
    int n = (int)(w[3]);

    streamer_sendto(x->streamer, in1, n * sizeof(t_sample));

    return (w+4);
}

void udp_send_broadcast_tilde_port(t_udp_send_broadcast_tilde *x, t_floatarg port) {
    int port_ = (int)port;
}

void udp_send_broadcast_tilde_dsp(t_udp_send_broadcast_tilde *x, t_signal **sp) {
    dsp_add(udp_send_broadcast_tilde_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

void udp_send_broadcast_tilde_setup(void) {
    udp_send_broadcast_tilde_class = class_new(gensym("udp_send_broadcast~"),
                                     (t_newmethod) udp_send_broadcast_tilde_new,
                                     (t_method) udp_send_broadcast_tilde_free,
                                     sizeof(t_udp_send_broadcast_tilde),
                                     CLASS_DEFAULT,
                                     A_DEFFLOAT,
                                     0);

    class_addmethod(udp_send_broadcast_tilde_class,
                    (t_method)udp_send_broadcast_tilde_dsp, gensym("dsp"), 0);
    class_addmethod(udp_send_broadcast_tilde_class,
                    (t_method)udp_send_broadcast_tilde_port, gensym("port"),
                    A_DEFSYM,
                    A_DEFFLOAT, 0);

    CLASS_MAINSIGNALIN(udp_send_broadcast_tilde_class, t_udp_send_broadcast_tilde, f);
}
