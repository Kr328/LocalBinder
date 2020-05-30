#pragma once

#include <vector>
#include <mutex>

class BufferPool {
public:
    BufferPool(int singleSize, int cacheSize) noexcept;

public:
    inline uint64_t getBufferSize() const {
        return singleSize;
    }

public:
    void *obtain() noexcept;
    void recycle(void *buffer) noexcept;

private:
    uint64_t singleSize;
    uint64_t cachedSize;
    std::vector<void *> cached;
    std::mutex mutex;
};
