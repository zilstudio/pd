#include <queue>

#include "reblocker.h"


struct Reblocker
{
private:
    size_t in_size_;
    size_t out_size_;
    std::vector<float> queue_;

public:
    Reblocker(size_t in, size_t out) : in_size_(in), out_size_(out) {
        queue_.reserve(out);
    }

    void add(float sample) {
        queue_.push_back(sample);

        if(queue_.size() == out_size_) {

            queue_.clear();
        }
    }
};
