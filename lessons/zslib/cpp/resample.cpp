#include <samplerate.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <m_pd.h>

#define POST post
//#define POST

#include "resample.h"

struct Resampler_
{
private:
    int err_;
    SRC_DATA data_;
    SRC_STATE * state_;
    float * buf_;
public:
    Resampler_(double ratio, size_t in_blocksize) : err_(0), state_(0), buf_(0) {
        // init state
        state_ = src_new(SRC_SINC_BEST_QUALITY, 1, &err_);
        src_set_ratio(state_, ratio);

        // alloc buffer
        size_t bufsize = (size_t) ceil(in_blocksize * ratio);
        buf_ = new float[bufsize];

        // init data
        memset(&data_, 0, sizeof(data_));
        data_.src_ratio = ratio;
        data_.input_frames = in_blocksize;
        data_.data_out = buf_;
        data_.output_frames = bufsize;
    }

    void setIn(const float *in) {
        data_.data_in = (float*) in;
    }

    float * out() {
        return buf_;
    }

    size_t outSize() {
        return data_.output_frames;
    }

    ~Resampler_() {
        src_delete(state_);
        delete buf_;
    }

    int process() {
        return src_process(state_, &data_);
    }
};

Resampler * resampler_create(double ratio, size_t in_blocksize)
{
    POST("resampler_create(%f, %i)", ratio, in_blocksize);
    Resampler * r = new Resampler(ratio, in_blocksize);
    return r;
}

void resampler_destroy(Resampler *r)
{
    POST("resampler_destroy");
    delete r;
}

int resampler_process(Resampler *r)
{
    assert(r);
    return r->process();
}

void resampler_set_input(Resampler *rs, float *in)
{
    rs->setIn(in);
}

float *resampler_get_output(Resampler *rs)
{
    assert(rs);
    return rs->out();
}

size_t resampler_get_output_size(Resampler *rs)
{
    assert(rs);
    return rs->outSize();
}
