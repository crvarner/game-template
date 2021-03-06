#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <vector>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID getComponentID() noexcept {
    static ComponentID typeID = getComponentID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual ~Component() {}
};

class Entity {
private:
    std::vector<std::unique_ptr<Component>> components;
public:
    void update() {
        for (auto& c : components) c->update();
        for (auto& c : components) c->draw();
    }
    void draw() {}

    template <typename T> bool hasComponent() const {
        return componentBitSet[getComponentID<T>()]
    }

    template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;
        c-init();
        return *c;
    }

    template <typename T> T& getComponent() const {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return static_cast<T*>(ptr);
    }
};

class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
public:
    void update() {
        for (auto& e : entities) {
            e->update();
        }
    }
    void draw() {
        for (auto& e : entities) {
            e->draw();
        }
    }
};