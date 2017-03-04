#include <m_pd.h>
#include <stdlib.h>
#include <stdio.h>
#include <opus/opus.h>
#include <string.h>

#include "stream.h"
#include "encoder.h"

#define ENCODER_FRAME_SIZE 240

static t_class * udp_send_broadcast_tilde_class;

typedef struct _opus {
    t_object  x_obj;
    t_float f;
    Streamer * streamer;
    OpusEncoder * encoder;
    t_sample in_buf[ENCODER_FRAME_SIZE];
    int int_counter;
    unsigned char out_buf[4000];
} t_udp_send_broadcast_tilde;

void *udp_send_broadcast_tilde_new(t_floatarg port)
{
    // INLETS
    t_udp_send_broadcast_tilde *x = (t_udp_send_broadcast_tilde *)pd_new(udp_send_broadcast_tilde_class);
    outlet_new(&x->x_obj, &s_float);

    x->streamer = streamer_create("255.255.255.255", port);
    int err = 0;
    x->encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_AUDIO, &err);

    if (err < 0){
        error("[udp_send_broadcast~] failed to create an encoder: %s\n", opus_strerror(err));
        return 0;
    }

    err = opus_encoder_ctl(x->encoder, OPUS_SET_BITRATE(128000));
    if (err < 0) {
        error("[udp_send_broadcast~] failed to set bitrate: %s\n", opus_strerror(err));
        return 0;
    }

    err = opus_encoder_ctl(x->encoder, OPUS_SET_VBR_CONSTRAINT(1));
    if(err != OPUS_OK){
        error("[udp_send_broadcast~] Error OPUS_SET_VBR_CONSTRAINT returned: %s\n", opus_strerror(err));
        return 0;
    }

    err = opus_encoder_ctl(x->encoder, OPUS_SET_COMPLEXITY(4));
    if(err != OPUS_OK){
        error("[udp_send_broadcast~] Error OPUS_SET_COMPLEXITY returned: %s\n", opus_strerror(err));
        return 0;
    }

//    ret=opus_multistream_encoder_ctl(st, OPUS_SET_PACKET_LOSS_PERC(expect_loss));
//    if(ret!=OPUS_OK){
//        fprintf(stderr,"Error OPUS_SET_PACKET_LOSS_PERC returned: %s\n",opus_strerror(ret));
//        exit(1);
//    }

    x->int_counter = 0;

    return (void *)x;
}

void udp_send_broadcast_tilde_free(t_udp_send_broadcast_tilde * x) {
    opus_encoder_destroy(x->encoder);
    streamer_destroy(x->streamer);
}

t_int *udp_send_broadcast_tilde_perform(t_int *w) {
    t_udp_send_broadcast_tilde *x = (t_udp_send_broadcast_tilde*)(w[1]);
    t_sample *in1 = (t_sample*)(w[2]);
    int n = (int)(w[3]);

    while(n--) {
        if(x->int_counter == (ENCODER_FRAME_SIZE - 1)) {
            int nbBytes = opus_encode_float(x->encoder, x->in_buf, ENCODER_FRAME_SIZE, x->out_buf, sizeof(x->out_buf));
            if (nbBytes < 0) {
                error("encode failed: %s\n", opus_strerror(nbBytes));
            }
            //            post("%i", nbBytes);
            streamer_sendto(x->streamer, x->out_buf, nbBytes);
            x->int_counter = 0;
        }

        x->in_buf[x->int_counter++] = *in1++;
    }

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
