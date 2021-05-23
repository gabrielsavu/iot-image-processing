//
// Created by hyperledger on 5/23/21.
//

#include "Stateful.h"

bool Stateful::isState(std::string key) {
    int count = this->states->count(key);
    if (count > 0) {
        return true;
    }
    return false;
}

std::string Stateful::getState(std::string key) {
    auto it = this->states->find(key);
    if (it != this->states->end()) {
        return it->second;
    }
    return nullptr;
}

void Stateful::addState(std::string key, std::string value) {
    this->states->insert({key, value});
}