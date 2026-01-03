# Ideas
- Chunked archetype ECS: for multithreading efficiency

# Design Document for Chunked Archetype ECS Engine

## Core Components

### 1. **Transform Component**
Defines the position, rotation, and scale of an entity in the game world.
#### Variables:
```cpp
struct Transform {
    float position[3]; // x, y, z
    float rotation[3]; // x, y, z (could use quaternion for optimization)
    float scale[3] = {1.0f, 1.0f, 1.0f};
};
```

### 2. **Mesh Renderer Component**
Handles the rendering of 3D models or meshes associated with an entity.
#### Variables:
```cpp
struct MeshRenderer {
    std::string meshFilePath; // File path to the mesh file
    std::string materialFilePath; // File path to the material/shader
};
```

### 3. **Camera Component**
Defines the entity as a camera.
#### Variables:
```cpp
struct Camera {
    float fieldOfView;
    float aspectRatio;
    float nearClipPlane;
    float farClipPlane;
};
```

### 4. **Light Component**
Defines lighting properties for an entity.
#### Variables:
```cpp
struct Light {
    enum LightType { Directional, Point, Spotlight }; // Type of light
    LightType type;
    float color[3]; // RGB color
    float intensity;
    float range; // Only for point or spotlight
    float spotAngle; // Only for spotlight
};
```

### 5. **Physics/RigidBody Component**
Allows physical simulations for the entity.
#### Variables:
```cpp
struct RigidBody {
    float mass;
    bool isKinematic;
    float velocity[3]; // Linear velocity
    float angularVelocity[3]; // Angular velocity
};
```

### 6. **Collider Component**
Defines the collision geometry for an entity.
#### Variables:
```cpp
struct Collider {
    enum ColliderShape { Box, Sphere, Capsule, Mesh }; // Shape of collider
    ColliderShape shape;
    float size[3]; // Dimensions (width, height, depth)
};
```

### 7. **Audio Source Component**
Provides audio playback capabilities to an entity.
#### Variables:
```cpp
struct AudioSource {
    std::string audioFilePath; // Path to audio file
    bool isLooping;
    float volume;
};
```

### 8. **Input Component**
Handles player input for the entity.
#### Variables:
```cpp
struct Input {
    std::map<std::string, int> keyBindings; // Maps action names to key codes
};
```

### 9. **Script Component**
Allows attaching user scripts to an entity.
#### Variables:
```cpp
struct Script {
    std::string scriptFilePath; // Path to the script file
};
```

### 10. **Particle System Component**
Handles particle systems for effects like fire, smoke, etc.
#### Variables:
```cpp
struct ParticleSystem {
    int maxParticles;
    float emissionRate;
    float lifetime;
    float startSize;
    float startSpeed;
    float startColor[4]; // RGBA
};
```

### 11. **Animation Component**
Handles animation playback for models.
#### Variables:
```cpp
struct Animation {
    std::string animationFilePath; // Path to animation data
    float playbackSpeed;
    bool isLooping;
};
```

### 12. **AI/Behavior Component**
Defines AI behavior for an entity.
#### Variables:
```cpp
struct AIBehavior {
    std::string behaviorTreeFile; // Path to behavior tree or script
    float decisionDelay;
};
```

---

This document will serve as a foundation for implementing the engine and can be iteratively updated as new features are added.
