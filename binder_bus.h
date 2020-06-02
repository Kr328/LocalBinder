#pragma once

#include <unordered_map>
#include <random>
#include <shared_mutex>

#include "binder.h"

class BnBinder;

class BinderBus {
public:
    BinderBus() noexcept = default;

public:
    bool transact(unsigned long id, uint64_t token, int code, Parcel *data, Parcel *reply);

public:
    static BinderBus *getInstance();

private:
    friend BnBinder;
    void registerBinder(BnBinder *binder);
    void unregisterBinder(BnBinder *binder);

private:
    std::shared_mutex mutex;
    std::vector<BnBinder *> binders;
    std::vector<int> available;
    int current = 0;

private:
    static BinderBus *instance;
};