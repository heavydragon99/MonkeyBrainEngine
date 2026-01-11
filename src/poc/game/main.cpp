#include <stdio.h>
#include "Engine.h"

class ComponentPosition : public Component {
public:
    int x{0}, y{0}, yaw{0};
};

class ComponentVelocity : public Component {
public:
    int vx{0}, vy{0};
};

class ComponentFashion : public Component {
public:
    int style{0}, slay{0};
};

int main() {
    Entity my_entity;
    ComponentPosition componentA;
    ComponentVelocity componentB;

    my_entity.addComponent(componentA);
    my_entity.addComponent(componentB);

    my_entity.publish();

    return 0;
}