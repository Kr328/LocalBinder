#include "binder.h"
#include "binder_bus.h"

#include <climits>

BnBinder::BnBinder() {
    std::uniform_int_distribution<uint64_t> dist(0, ULONG_MAX);

    this->id = 0;
    this->token = dist(randomDevice);

    BinderBus::getInstance()->registerBinder(this);
}

BnBinder::~BnBinder() {
    BinderBus::getInstance()->unregisterBinder(this);
}

bool BnBinder::transact(int code, Parcel *data, Parcel *reply) {
    this->onTransact(code, data, reply);

    return true;
}

unsigned long BnBinder::getToken() const {
    return this->token;
}

unsigned long BnBinder::getId() const {
    return this->id;
}

BpBinder::BpBinder(uint64_t id, uint64_t token) {
    this->id = id;
    this->token = token;
}

bool BpBinder::transact(int code, Parcel *data, Parcel *reply) {
    return BinderBus::getInstance()->transact(id, token, code, data, reply);
}

unsigned long BpBinder::getId() const {
    return this->id;
}

uint64_t BpBinder::getToken() const {
    return this->token;
}

std::random_device BnBinder::randomDevice;