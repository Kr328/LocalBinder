#pragma once

#include <random>

#include "parcel.h"

class BinderBus;

class Binder {
public:
    virtual ~Binder() = default;

public:
    virtual bool transact(int code, Parcel *data, Parcel *reply) = 0;
    [[nodiscard]] virtual unsigned long getId() const = 0;
    [[nodiscard]] virtual uint64_t getToken() const = 0;
};

class BnBinder: public Binder {
public:
    BnBinder();
    ~BnBinder() override;

public:
    bool transact(int code, Parcel *data, Parcel *reply) override;

protected:
    virtual void onTransact(int code, Parcel *data, Parcel *reply) = 0;

public:
    [[nodiscard]] unsigned long getId() const override;
    [[nodiscard]] uint64_t getToken() const override;

private:
    friend BinderBus;
    unsigned long id;
    uint64_t token;

private:
    static std::random_device randomDevice;
};

class BpBinder: public Binder {
public:
    explicit BpBinder(unsigned long id, uint64_t token);

public:
    bool transact(int code, Parcel *data, Parcel *reply) override;

public:
    [[nodiscard]] unsigned long getId() const override;
    [[nodiscard]] uint64_t getToken() const override;

private:
    friend BinderBus;
    unsigned long id;
    uint64_t token;
};