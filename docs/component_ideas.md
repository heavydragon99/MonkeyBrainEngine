# Ideas
- Chunked archetype ECS: for multithreading efficiency

# Design Document for Chunked Archetype ECS Engine

## Unity components
### 1. Core / Essential Components
#### Transform
- Transform (always present)
- Rect Transform (UI-only)

### 2. Rendering Components
#### Renderers
- Mesh Renderer
- Skinned Mesh Renderer
- Sprite Renderer
- Trail Renderer
- Line Renderer
- Particle System Renderer
- Text Mesh
- Text Mesh Pro (package, but common)

#### Visual Effects
- Particle System
- Visual Effect (VFX Graph – package)

#### Lighting
- Light
- Light Probe Group
- Reflection Probe
- Occlusion Area
- Occlusion Portal
- LOD Group

### 3. Physics Components
#### 3D Physics
- Rigidbodies
- Rigidbody
- Constant Force

#### Colliders
- Box Collider
- Sphere Collider
- Capsule Collider
- Mesh Collider
- Wheel Collider
- Terrain Collider

#### Joints
- Fixed Joint
- Hinge Joint
- Spring Joint
- Configurable Joint
- Character Joint

### 2D Physics
#### Rigidbodies
- Rigidbody2D
- Constant Force 2D

#### Colliders
- Box Collider 2D
- Circle Collider 2D
- Capsule Collider 2D
- Polygon Collider 2D
- Edge Collider 2D
- Tilemap Collider 2D
- Composite Collider 2D

#### Joints
- Hinge Joint 2D
- Fixed Joint 2D
- Spring Joint 2D
- Distance Joint 2D
- Slider Joint 2D
- Wheel Joint 2D
- Relative Joint 2D
- Target Joint 2D
- Friction Joint 2D

### 4. Animation Components
- Animator
- Animator Override Controller
- Avatar
- Aim Constraint
- Look At Constraint
- Parent Constraint
- Position Constraint
- Rotation Constraint
- Scale Constraint

### 5. Audio Components
- Audio Source
- Audio Listener
- Audio Reverb Zone
- Audio Low Pass Filter
- Audio High Pass Filter
- Audio Echo Filter
- Audio Distortion Filter
- Audio Chorus Filter

### 6. UI Components (uGUI)
#### Layout
- Canvas
- Canvas Renderer
- Canvas Group
- Rect Mask 2D
- Aspect Ratio Fitter
- Content Size Fitter
- Horizontal Layout Group
- Vertical Layout Group
- Grid Layout Group
-Layout Element

#### UI Elements
- Image
- Raw Image
- Text
- Button
- Toggle
- Slider
- Scrollbar
- Dropdown
- Input Field
- Scroll Rect
- Mask

#### Event System
- Event System
- Standalone Input Module
- Input System UI Input Module (new input system)

#### 7. Navigation & AI
- NavMesh Agent
- NavMesh Obstacle
- NavMesh Surface (package)

#### 8. Cameras & Input
- Camera
- Flare Layer
- Physics Raycaster
- Physics 2D Raycaster

### 9. Effects & Utility
- Projector
- Lens Flare
- Halo
- Wind Zone
- Skybox
- Video Player
- Playable Director
- Sorting Group
- Sprite Shape Renderer (2D package)

### 10. Tilemap (2D)
- Tilemap
- Tilemap Renderer
- Tilemap Collider 2D

### 11. Custom & Script Components
- MonoBehaviour scripts
- StateMachineBehaviour
- Editor-only scripts
