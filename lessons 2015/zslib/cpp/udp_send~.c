#include <m_pd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static t_class * udp_send_tilde_class;

typedef struct _opus {
    t_object  x_obj;
    t_float f;
    char connected;
    char has_errors;
    struct hostent *hp;
    struct sockaddr_in servaddr;
    int fd;
} t_udp_send_tilde;

void *udp_send_tilde_new()
{
    // INLETS
    t_udp_send_tilde *x = (t_udp_send_tilde *)pd_new(udp_send_tilde_class);
    outlet_new(&x->x_obj, &s_float);

    x->connected = 0;
    x->has_errors = 0;
    /* fill in the server's address and data */
    memset((char*)&x->servaddr, 0, sizeof(x->servaddr));
    x->servaddr.sin_family = AF_INET;
    x->fd = 0;

    return (void *)x;
}

void udp_send_tilde_free(t_udp_send_tilde * x) {
    if(x->connected) {
        close(x->fd);
    }
}

t_int *udp_send_tilde_perform(t_int *w) {
    t_udp_send_tilde *x = (t_udp_send_tilde*)(w[1]);
    t_sample *in1 = (t_sample*)(w[2]);
    int n = (int)(w[3]);

    if(x->connected) {
        if (sendto(x->fd, in1, n * sizeof(t_sample), 0,
                   (struct sockaddr *)&x->servaddr,
                   sizeof(x->servaddr)) < 0) {
            if(!x->has_errors) {
                error("[udp_send~] sendto failed");
                outlet_float(x->x_obj.ob_outlet, 0);
                x->has_errors = 1;
            }
        }
        else {
            x->has_errors &= 0;
        }
    }

    return (w+4);
}

void udp_send_tilde_connect(t_udp_send_tilde *x, t_symbol * host, t_floatarg port) {
    const char * host_;
    int port_ = (int)port;

    host_ = host->s_name;
    x->hp = gethostbyname(host_);

    if (!x->hp) {
        error("[udp_send~] could not obtain address of \"%s\"\n", host_);
        x->connected = 0;
        return;
    }

    post("[udp_send~] connect to: %s:%i", inet_ntoa(*((struct in_addr *)x->hp->h_addr_list[0])), port_);

    x->servaddr.sin_port = htons(port_);
    /* put the host's address into the server address structure */
    memcpy((void *)&x->servaddr.sin_addr, x->hp->h_addr_list[0], x->hp->h_length);

    if(x->fd != 0) {
        close(x->fd);
    }

    /* create a UDP socket */
    if ((x->fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        x->connected = 0;
        error("[udp_send~] cannot create socket\n");
        return;
    }

    int broadcast = 1;
    if (setsockopt(x->fd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof(broadcast)) == -1) {
        error("[udp_send~] broacast failed: %s", strerror(errno));
    }

    x->connected = 1;
}

void udp_send_tilde_dsp(t_udp_send_tilde *x, t_signal **sp) {
    dsp_add(udp_send_tilde_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

void udp_send_tilde_setup(void) {
    udp_send_tilde_class = class_new(gensym("udp_send~"),
                                     (t_newmethod) udp_send_tilde_new,
                                     (t_method) udp_send_tilde_free,
                                     sizeof(t_udp_send_tilde),
                                     CLASS_DEFAULT,
                                     A_DEFFLOAT,
                                     0);

    class_addmethod(udp_send_tilde_class,
                    (t_method)udp_send_tilde_dsp, gensym("dsp"), 0);
    class_addmethod(udp_send_tilde_class,
                    (t_method)udp_send_tilde_connect, gensym("connect"),
                    A_DEFSYM,
                    A_DEFFLOAT, 0);

    CLASS_MAINSIGNALIN(udp_send_tilde_class, t_udp_send_tilde, f);
}
