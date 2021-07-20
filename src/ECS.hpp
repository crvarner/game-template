#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <vector>



class Entity;

class Component {
public:
    Entity* entity;

    virtual void init() {}
    virtual void render(float lag) {}
    virtual ~Component() {}
};






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

class Entity {
private:
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
public:
    void render(float lag) {
        for (auto& c : componentArray) {
            if (c) {
                c->render(lag);
            }
        }
    }

    template <typename T> bool hasComponent() const {
        return componentBitSet[getComponentID<T>()];
    }

    template <typename T, typename... TArgs> T& addComponent(TArgs&&... tArgs) {
        T* component = new T(std::forward<TArgs>(tArgs)...);

        ComponentID componentID = getComponentID<T>();
        componentArray[componentID] = component;
        componentBitSet[componentID] = true;

        component->entity = this;
        component->init();
        return *component;
    }

    template <typename T> T* getComponent() const {
        auto ptr(componentArray[getComponentID<T>()]);
        return static_cast<T*>(ptr);
    }
};






class EntityManager {
public:
    void render(float lag) {
        for (auto& e : entities) e->render(lag);
    }

    void addEntity(Entity *entity) {
        entities.emplace_back(entity);
    }

private:
    std::vector<std::unique_ptr<Entity>> entities;
};