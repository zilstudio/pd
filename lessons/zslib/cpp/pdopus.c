#include <opus/opus.h>
#include "stream.h"
#include <m_pd.h>

void t() {
    Streamer s = streamer_create("123", 200);
    streamer_destroy(s);
}

static t_class * opus_encode_class;

typedef struct _opus {
    t_object  x_obj;
} t_opus_encode;

void opus_encode_bang(t_opus_encode *x)
{
    t();
    post("Hello world !!");
}

void *opus_encode_new(void)
{
    t_opus_encode *x = (t_opus_encode *)pd_new(opus_encode_class);

    return (void *)x;
}

void opus_encode_setup(void) {
    opus_encode_class = class_new(gensym("opus_encode"),
                                 (t_newmethod)opus_encode_new,
                                 0, sizeof(t_opus_encode),
                                 CLASS_DEFAULT, 0);
    class_addbang(opus_encode_class, opus_encode_bang);
}
