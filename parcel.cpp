#include "parcel.h"
#include "binder.h"

#include <cstring>
#include <algorithm>
#include <exception>

Parcel::Parcel() {
    this->position = 0;
    this->size = 0;
    this->reset();
}

void Parcel::resize(uint64_t capacity) {
    while (true) {
        if (capacity < buffers.size() * pool->getBufferSize())
            return;

        buffers.push_back(pool->obtain());
    }
}

Parcel::~Parcel() {
    this->reset();
}

void Parcel::reset() {
    this->position = 0;
    this->size = 0;

    for (auto buf : buffers) {
        pool->recycle(buf);
    }

    buffers.clear();
}

void Parcel::writeBytes(const void *bytes, uint64_t length) {
    resize(size + length);

    uint64_t bufferSize = pool->getBufferSize();

    while (length > 0) {
        int fragment = static_cast<int>(size / bufferSize);
        uint64_t offset = size % bufferSize;
        uint64_t copied = std::min(bufferSize - offset, length);

        memcpy(((uint8_t *)buffers[fragment]) + offset, bytes, copied);

        length -= copied;
        size += copied;
        bytes = static_cast<const void *>((reinterpret_cast<const uint8_t *>(bytes) + copied));
    }
}

bool Parcel::readBytes(void *bytes, uint64_t length) {
    if (position + length > size)
        throw std::runtime_error("parcel: out of bound");

    uint64_t bufferSize = pool->getBufferSize();

    while (length > 0) {
        int fragment = static_cast<int>(position / bufferSize);
        uint64_t offset = position % bufferSize;
        uint64_t copied = std::min(bufferSize - offset, length);

        memcpy(bytes, ((uint8_t *)buffers[fragment]) + offset, copied);

        length -= copied;
        position += copied;
        bytes = static_cast<void *>((reinterpret_cast<uint8_t *>(bytes) + copied));
    }

    return true;
}

void Parcel::writeInt(int32_t value) {
    writeBytes(&value, sizeof(value));
}

void Parcel::writeLong(int64_t value) {
    writeBytes(&value, sizeof(value));
}

void Parcel::writeChar(wchar_t value) {
    writeBytes(&value, sizeof(value));
}

void Parcel::writeString(const std::string &str) {
    uint32_t length = str.size();
    writeBytes(&length, sizeof(length));
    writeBytes(str.c_str(), length);
}

void Parcel::writeBoolean(bool value) {
    writeBytes(&value, sizeof(value));
}

void Parcel::writeBinder(const Binder *binder) {
    auto id = binder->getId();
    auto token = binder->getToken();

    writeBytes(&id, sizeof(id));
    writeBytes(&token, sizeof(token));
}

int32_t Parcel::readInt() {
    int32_t result = 0;

    readBytes(&result, sizeof(result));

    return result;
}

int64_t Parcel::readLong() {
    int64_t result = 0;

    readBytes(&result, sizeof(result));

    return result;
}

wchar_t Parcel::readChar() {
    wchar_t result = 0;

    readBytes(&result, sizeof(result));

    return result;
}

std::string Parcel::readString() {
    uint32_t length = 0;

    readBytes(&length, sizeof(length));

    char *buf = new char[length];

    readBytes(buf, length);

    std::string s(buf, length);

    delete[] buf;

    return s;
}

bool Parcel::readBoolean() {
    bool result = false;

    readBytes(&result, sizeof(result));

    return result;
}

Binder *Parcel::readBinder() {
    unsigned long id;
    uint64_t token;

    readBytes(&id, sizeof(id));
    readBytes(&token, sizeof(token));

    return new BpBinder(id, token);
}

BufferPool *Parcel::pool = new BufferPool(DEFAULT_BUFFER_SIZE, DEFAULT_BUFFER_COUNT);