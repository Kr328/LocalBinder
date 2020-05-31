#pragma once

#include <vector>
#include <cctype>
#include <string>

#include "buffer_pool.h"

class Binder;

class Parcel {
public:
    Parcel();

    ~Parcel();

public:
    void reset();

public:
    [[nodiscard]] inline uint64_t getPosition() const {
        return position;
    }
    [[nodiscard]] inline uint64_t getSize() const {
        return size;
    }

public:
    void writeInt(int32_t value);
    void writeLong(int64_t value);
    void writeChar(wchar_t value);
    void writeString(std::string const &str);
    void writeBoolean(bool value);
    void writeBinder(const Binder *binder);
    void writeBytes(const void *bytes, uint64_t length);

public:
    int32_t readInt();
    int64_t readLong();
    wchar_t readChar();
    std::string readString();
    bool readBoolean();
    Binder *readBinder();
    bool readBytes(void *bytes, uint64_t length);

private:
    void resize(uint64_t capacity);

private:
    std::vector<void *> buffers;
    uint64_t position;
    uint64_t size;

private:
    static BufferPool *pool;
};