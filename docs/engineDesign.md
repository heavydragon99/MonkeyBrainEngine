# Ideas
- Chunked archetype ECS: for multithreading efficiency

# Design Document for Chunked Archetype ECS Engine

## Core Components

### 1. **Transform Component**
Defines the position, rotation, and scale of an entity in the game world.

### 2. **Mesh Renderer Component**
Handles the rendering of 3D models or meshes associated with an entity.

### 3. **Camera Component**
Defines the entity as a camera with properties such as field of view and clipping planes.

### 4. **Light Component**
Defines lighting properties for an entity, including type and intensity.

### 5. **Physics/RigidBody Component**
Allows physical simulations for the entity, such as movement and collisions.

### 6. **Collider Component**
Defines the collision geometry for an entity, specifying its shape and size.

### 7. **Audio Source Component**
Provides audio playback capabilities to an entity.

### 8. **Input Component**
Handles player input for the entity, such as key bindings.

### 9. **Script Component**
Allows attaching user scripts to an entity for customizable behavior.

### 10. **Particle System Component**
Handles particle systems to create visual effects like fire and smoke.

### 11. **Animation Component**
Handles animation playback for models, including speed and looping functionality.

### 12. **AI/Behavior Component**
Defines AI behavior for an entity, such as decision-making and actions.

---

This document will serve as a foundation for implementing the engine and can be iteratively updated as new features are added.