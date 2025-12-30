#pragma once

#include <vector>
#include <functional>

class Component {
public:
    int id{0};
};

class System {
public:
    int id{0};
};

class Entity {
public:
    int id{0};
    void addComponent(Component& aComponent) {mComponents.push_back(aComponent); }
    /**
     * @brief This makes the engine evaluate which components are added, and then adds the entity to the right archetype.
     */
    void publish();
private:
    std::vector<std::reference_wrapper<Component>> mComponents;
};