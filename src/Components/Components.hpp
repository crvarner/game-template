#pragma once
#include <string>
#include <map>

#include "ECS.hpp"
#include "Components/Idle.hpp"
#include "Components/Position.hpp"
#include "Components/Sprite.hpp"

enum ComponentType {
    UNKNOWN,
    IDLE,
    POSITION,
    SPRITE,
};

static std::map<std::string, ComponentType> componentMap = {
    {"Idle", IDLE},
    {"Position", POSITION},
    {"Sprite", SPRITE}
};

static ComponentType strToComponentType (std::string key) {
    auto componentType = componentMap.find(key);
    if (componentType == componentMap.end()) {
        return UNKNOWN;
    } else {
        return componentType->second;
    }
}