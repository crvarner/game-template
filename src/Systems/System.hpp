#pragma once

class System {
public:
    virtual void handleEvents() {};
    virtual void process() = 0;
};

class IdleSystem: public System {
public:
    IdleSystem();
    void process();
};