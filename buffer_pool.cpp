#include "buffer_pool.h"

#include "malloc.h"

BufferPool::BufferPool(int singleSize, int cacheSize) noexcept {
    this->singleSize = singleSize;
    this->cachedSize = cacheSize;
}

void *BufferPool::obtain() noexcept {
    std::unique_lock<std::mutex> lock(mutex);

    if (cached.empty())
        return malloc(singleSize);

    void *buf = cached.back();
    cached.pop_back();

    return buf;
}

void BufferPool::recycle(void *buffer) noexcept {
    std::unique_lock<std::mutex> lock(mutex);

    if (cached.size() >= cachedSize) {
        free(buffer);
        return;
    }

    cached.push_back(buffer);
}