//
// Created by hyperledger on 5/23/21.
//

#ifndef IMAGE_PROCESSING_STATEFUL_H
#define IMAGE_PROCESSING_STATEFUL_H


#include <memory>
#include <map>

class Stateful {
public:
    static Stateful &Get() {
        static Stateful instance;
        return instance;
    }

    ~Stateful() {};

    bool isState(std::string key);

    void addState(std::string key, std::string value);

    std::string getState(std::string key);

private:
    Stateful() {
        states = std::make_shared<std::map<std::string, std::string>>();
    };

    std::shared_ptr<std::map<std::string, std::string>> states;

};


#endif //IMAGE_PROCESSING_STATEFUL_H
