#include "binder_bus.h"

#include <algorithm>

void BinderBus::registerBinder(BnBinder *binder) {
    std::lock_guard<std::shared_mutex> lock(mutex);

    if ( !available.empty() ) {
        std::push_heap(available.begin(), available.end(), std::greater<>{});

        int id = available.back();

        available.pop_back();

        binder->id = id;
        binders[id] = binder;
    } else {
        binder->id = current++;

        binders.push_back(binder);
    }
}

void BinderBus::unregisterBinder(BnBinder *binder) {
    std::lock_guard<std::shared_mutex> lock(mutex);

    binders[binder->id] = nullptr;

    available.push_back(binder->id);

    std::push_heap(available.begin(), available.end());

    binder->id = -1;
}

bool BinderBus::transact(unsigned long id, uint64_t token, int code, Parcel *data, Parcel *reply) {
    Binder *binder;

    {
        std::shared_lock<std::shared_mutex> lock(mutex);

        binder = binders[id];
        if ( binder == nullptr || binder->getToken() != token )
            return false;
    }

    return binder->transact(code, data, reply);
}

void BinderBus::addService(const std::string &name, Binder *binder) {
    this->removeService(name);

    this->services[name] = new BpBinder(binder->getId(), binder->getToken());
}

Binder *BinderBus::getService(const std::string &name) {
    auto binder = this->services[name];
    if ( binder == nullptr )
        return nullptr;

    return new BpBinder(binder->getId(), binder->getId());
}

void BinderBus::removeService(const std::string &name) {
    auto binder = this->services[name];

    if ( binder != nullptr ) {
        delete binder;
        this->services.erase(name);
    }
}

BinderBus *BinderBus::getInstance() {
    return instance;
}

BinderBus *BinderBus::instance = new BinderBus();
