#pragma once

#include <unordered_map>
#include <random>
#include <shared_mutex>

#include "binder.h"

class BinderBus {
public:
    BinderBus() noexcept = default;

public:
    void registerBinder(BnBinder *binder);
    void unregisterBinder(BnBinder *binder);

public:
    bool transact(unsigned long id, uint64_t token, int code, Parcel *data, Parcel *reply);

public:
    static BinderBus *getInstance();

private:
    std::shared_mutex mutex;
    std::unordered_map<unsigned long, BnBinder*> binders;
    std::random_device randomDevice;

private:
    static BinderBus *instance;
};