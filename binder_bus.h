#pragma once

#include <unordered_map>
#include <random>
#include <shared_mutex>
#include <string>

#include "binder.h"

class BnBinder;

class BinderBus {
public:
    BinderBus() noexcept = default;

public:
    bool transact(unsigned long id, uint64_t token, int code, Parcel *data, Parcel *reply);

public:
    void addService(std::string const &name, Binder *binder);
    Binder *getService(std::string const &name);
    void removeService(std::string const &name);

public:
    static BinderBus *getInstance();

private:
    friend BnBinder;
    void registerBinder(BnBinder *binder);
    void unregisterBinder(BnBinder *binder);

private:
    std::shared_mutex mutex;
    std::unordered_map<std::string, BpBinder*> services;
    std::vector<BnBinder *> binders;
    std::vector<int> available;
    int current = 0;

private:
    static BinderBus *instance;
};