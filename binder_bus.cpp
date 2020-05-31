#include "binder_bus.h"

#include <climits>

void BinderBus::registerBinder(BnBinder *binder) {
    std::lock_guard<std::shared_mutex> lock(mutex);

    auto existed = binders.find(binder->id);

    if (existed != binders.end() && existed->second->token == binder->token)
        return;

    std::uniform_int_distribution<unsigned long> dist(0, ULONG_MAX);

    unsigned long id = dist(randomDevice);

    binder->id = id;
    binders[id] = binder;
}

void BinderBus::unregisterBinder(BnBinder *binder) {
    std::lock_guard<std::shared_mutex> lock(mutex);

    binder->id = 0;
    binders.erase(binder->id);
}

bool BinderBus::transact(unsigned long id, uint64_t token, int code, Parcel *data, Parcel *reply) {
    Binder *binder;

    {
        std::shared_lock<std::shared_mutex> lock(mutex);

        auto iterator = binders.find(id);
        if (iterator == binders.end() || iterator->second->token != token)
            return false;

        binder = iterator->second;
    }

    return binder->transact(code, data, reply);
}

BinderBus *BinderBus::getInstance() {
    return instance;
}

BinderBus *BinderBus::instance = new BinderBus();