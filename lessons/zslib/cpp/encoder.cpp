#include <opus/opus.h>
#include <assert.h>
#include <m_pd.h>

#include "encoder.h"

#define POST post

struct Encoder_
{
private:
    size_t samplerate_;
    size_t framesize_;
    int err_;
    OpusEncoder * encoder_;
    unsigned char * buf_;
public:
    Encoder_(size_t samplerate, size_t framesize) : samplerate_(samplerate), framesize_(framesize), err_(0) {
        encoder_ = opus_encoder_create(samplerate_, 1, OPUS_APPLICATION_RESTRICTED_LOWDELAY, &err_);
        buf_ = new unsigned char[framesize];
    }

    ~Encoder_() {
        opus_encoder_destroy(encoder_);
        delete buf_;
    }

    int process(const float * pcm) {
        return opus_encode_float(encoder_, pcm, 120, buf_, framesize_);
    }

    unsigned char * out() {
        return buf_;
    }
};

Encoder *encoder_create(size_t samplerate, size_t framesize)
{
    POST("encoder_create(%ld, %ld)", samplerate, framesize);
    return new Encoder(samplerate, framesize);
}

void encoder_destroy(Encoder *enc)
{
    POST("encoder_destroy");
    delete enc;
}

int encoder_process(Encoder *enc, const float *pcm)
{
    assert(enc);
    return enc->process(pcm);
}

unsigned char *encoder_out(Encoder *enc)
{
    assert(enc);
    return enc->out();
}
