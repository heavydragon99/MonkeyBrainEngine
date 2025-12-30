# Comprehensive Design & Requirements Document

**Project:** Biological Time-Loop Automation Game  
**Game Name:** Splorb

_This document consolidates all design information provided so far and expands it into a full specification for implementation, testing, and iteration._

---

## 1. High-level Concept

A calm, slow-paced automation game set on an alien biological planet. The player is a biological entity that upgrades itself across repeated time-loop cycles. A central symbiotic biological machine powers the world and enforces the loop; repairing it extends future cycle duration and unlocks access to more advanced biological resources. Players can grow worker organisms (bots) which function mainly as carriers or machine copies. The world is procedurally generated and resets each cycle.

---

## 2. Core Game Loop and Time-Loop Mechanics

### Core Loop

- Each **'run'** is a fixed-duration time loop during which the player acts in the world
- The **central biological machine** supplies the player's active energy. When energy runs out the player falls asleep and the run ends
- At the end of a run the world **resets** (full regeneration) and the player returns with persistent upgrades and unlocked recipes
- Player can also apply **upgrades to the central machine** at end-of-run which affect future runs (duration, efficiency, available slots, input speed)
- Major repairs to the machine unlock access to **higher-tier resources** in subsequent runs

---

## 3. World Machine (Central Machine) Specification

**Concept:** A planet-scale biological device that provides bioelectric energy to the player. It accepts resources via an access point (furnace-like interface) and consumes inputs to power the player for the duration of the run.

### Components

Multiple subsystems/components that can be repaired/upgraded individually:

- Receptacle
- Metabolizer
- Regulator
- Distributor
- Storage

### Upgrades at End-of-Run

- **Cycle duration** (base active time)
- **Efficiency** (fewer inputs per unit time required)
- **Number of input slots** (parallel intake)
- **Intake speed** (how fast inputs are consumed/processed)
- **Advanced resource unlocks** (major component repairs unlock new materials)

### Behavior

- During a run the machine's effectiveness will inevitably decline (forcing a loop) unless repaired/upgraded across runs
- Repairing major components is required to access new resource tiers and to make the world blossom more
- The machine is **symbiotic** with the environment—improving the machine increases world lushness over runs

---

## 4. Player Character Design

Player is a single controllable biological entity with modular upgrades. The player is 'alien' (non-human) and evolves across runs.

### Persistent Elements Between Runs

- Upgrade points
- Unlocked recipes
- Learned lore

### Potential Upgrade Categories

- Gather speed
- Crafting speed
- Inventory size
- Movement speed
- Bot node capacity
- Sensor range
- Stamina/energy efficiency

### Upgrade System

- Upgrades are arranged as a **progression tree** (big tree) that you can only invest in at end-of-run
- **Player color in visuals:** Red

---

## 5. Workers & Machines (Bots)

Workers and machines are biological copies/transformations of the player. They serve primarily as carriers and transporters, and as physical machines (e.g., harvesters, processors).

### Visual Color Coding

- **Player:** Red
- **Machines:** Green
- **Workers:** Purple

### Worker Tiers

Workers will be produced at the cost of resources and have **6 tiers:**

1. Small ground
2. Big ground
3. Small flyer
4. Big flyer
5. Small teleporter
6. Big teleporter

### Worker Chain Mechanic

When multiple workers move resources between two points, they form a **chain** so not every worker traverses the full route:

- More workers reduce per-unit movement, increasing throughput
- Must cap chain speed to avoid unbounded optimization

### Machines

- Converted copies of the player (green) that behave as stationary processors or resource collectors
- Worker and machine behavior persists only during a run (they reset with the world)

### Important Worker Design Choices (Left to You)

- How workers are spawned or grown (resource cost, time, infrastructure)
- Worker storage/stacking rules (how many can occupy a tile, carry capacities)
- Exact chain logistics (how handoffs occur, path reservation, congestion rules)

---

## 6. Bot Programming (Optional Advanced Feature)

Although the immediate plan makes bots simple carriers, there is an optional advanced system where bots can be programmed using a **node-based interface**. The player upgrade tree can increase the number of programmable nodes available for each bot.

### List of Programmable Nodes / Actions

#### Movement Nodes

- `MoveTo(tile)`
- `MoveAlongPath(path)`
- `Wait(duration)`

#### Resource Interaction Nodes

- `Gather(resource_type, amount)`
- `Drop(resource_type, amount, target)`
- `Pickup(resource_type, amount)`
- `TransferTo(entity, resource_type, amount)`

#### Machine Interaction Nodes

- `UseMachine(machine_id, action)`
- `StartProcessing(recipe_id)`

#### Conditional / Logic Nodes

- `If(predicate)` → branch
- `Compare(valueA, valueB)`
- `ResourceAvailable(resource_type, amount)`
- `HasInventorySpace(amount)`
- `While(predicate)` → loop

#### Sensing Nodes

- `DetectResourceInRadius(type, radius)`
- `DetectEntityType(type, radius)`
- `GetNearest(target_type)`
- `GetTileProperty(x, y)`

#### Memory / State Nodes

- `Store(value_name, value)`
- `Read(value_name)`
- `Increment(value_name, amount)`
- `Reset(value_name)`

#### Control / Flow Nodes

- **Sequence** (execute children in order)
- **Parallel** (run children concurrently with limits)
- `Delay(seconds)`

#### Utility Nodes

- `RandomChoice(list)`
- `Return(success/failure)`

### Design Notes

- This node system can be as lightweight or deep as you want — it is **optional** and can be gated behind player upgrades or lore discoveries
- Aim for a minimal node set first (move, gather, drop, conditional) and expand later
- Consider performance—evaluating many bot programs each tick can be CPU intensive; consider batching or simplified state machines for lower-tier bots

---

## 7. Resources, Tiers, and Production

### Resource System

- Resources are **biological in nature** (no stone/metal/wood)
- **Examples:** aqueous biomass (water analog), spore-matter, membrane fibers, sapid gel, bioelectric nodes
- World resources are **infinite in total** but have local regeneration limits and global extraction-per-second caps
- At the start there are **two primary resource types** (e.g., Water-like and a basic biomass)
- Higher-tier resources require repaired/advanced machines to extract
- Using machines may require combo resources like water + processed biomass
- Resource flow rates and machine efficiency are affected by player upgrades and machine upgrades

### Production Chains & Machines

- Machines are biological processors that transform inputs into outputs on a timed schedule (the simulation engine processes them on ticks)
- Machines may have internal queues and input/output slots (the central machine itself has such slots)
- Recipes are unlockable via upgrades or lore discoveries
- **Bioelectric pylons** can be used as power infrastructure for advanced machines (conceptual 'electricity' flavored biologically)

---

## 8. World Generation & Persistence

An **infinite procedurally generated map** that resets every loop. The world should be generated with chunking for memory efficiency and streaming (similar to Minecraft's approach).

### Design

- World is conceptually infinite; generate by **deterministic seed** so runs can be reproducible if needed
- **Chunking:** divide the world into manageable chunks, load/unload chunks as the player moves
- At end-of-run every chunk is regenerated for the new run (or re-seeded) unless you decide to persist some features
- Consider storing world metadata in the DB for debugging or replay, but entire terrain need not be stored persistently
- **Chunk save format:** choose a compact representation (binary tiles, RLE, or chunk-level compressed blobs)

### Implementation Hints

- Use procedural noise (Perlin/Simplex) for biome distribution and resource clusters
- Store per-chunk random seeds to allow deterministic regeneration without storing full data
- Decide chunk size to balance memory and IO (e.g., 16×16 or 32×32)
- Decide what is generated per-chunk: base terrain, resource nodes, small flora, special landmarks, machine artifacts

---

## 9. Visualization & UI

You initially considered ASCII but later preferred **pixel art**. Keep UI simple to preserve calm atmosphere.

### Primary Display

- 2D top-down tile map (pixel art preferred)
- **Color coding:** player (red), machines (green), workers (purple)

### UI Overlays

- Time-loop meter
- Current available energy
- Inventory panel
- Selected bot/machine UI
- Upgrade tree access at inter-run screens

### Visual Effects

- **Edge-of-screen 'fatigue' effect:** use vignetting or darkened edges to show low energy before blackout/sleep
- Minimal HUD to keep the scene immersive and calm

### Secondary Visualization

- Optional debug terminal view for developers (show simulation tick info, entity counts, chunk loads)

---

## 10. Simulation Engine & Server Architecture

Separation of concerns is critical to allow **single-player now** and **multiplayer later**. The architecture should support an authoritative simulation engine that can run locally in single-player or on a server for multiplayer.

### Core Components

#### Simulation Engine (C++)

- Authoritative tick loop
- Processes machine recipes, worker movement, growth, resource regen

#### Game Server (C++)

- Exposes action endpoints (move, build, craft, spawn worker)
- Validates actions
- Writes to DB
- Dispatches updates to clients

#### Database

- Stores persistent data (player profiles, upgrade tree progress, unlocked recipes, central machine upgrades)
- Optional logs/replays

### Requirements

- **Single-repo requirement:** make the server and client buildable from the same repository with CMake
- **Tick model:** deterministic update per tick (e.g., every N ms). Consider fixed timestep and physics-simulation-friendly logic
- Simulation must be modular so you can run it headless or attached to a local client UI

---

## 12. Database & Persistence Model

**Minimal persistent data:** player accounts, upgrades, unlocked recipes, machine upgrade state, settings. The world terrain and per-run entities do not need to be persisted long-term unless for debugging/replay.

### DB Choices

- **SQLite** for single-file simplicity (developer-friendly)
- **PostgreSQL** for production server
- Keep DB access abstracted behind a data layer

### Schema Ideas (High Level)

```sql
-- Players table
players(
  id,
  name,
  upgrades_blob,
  unlocked_recipes_blob,
  created_at
)

-- Machine upgrades table
machine_upgrades(
  player_id,
  machine_state_blob,
  last_modified
)

-- Lore fragments table
lore_fragments(
  id,
  seed_location_meta,
  text,
  unlocked
)

-- Runs table
runs(
  run_id,
  player_id,
  seed,
  timestamp,
  stats_blob
)

-- Logs (optional for replay or debugging)
logs(...)
```

### Data Format

- Prefer **JSON blobs** for flexible upgrade trees and unlock sets early, then normalize if needed

---

## 13. Build System, Repo & Developer Productivity

A **single Git repo** that anyone can clone and run. Keep dependencies minimal and optional.

### Build System

- Use **CMake** for cross-platform builds (Windows, macOS, Linux)

### Repository Structure

```
/engine
/server
/client
/tools
/docs
/assets
```

### Dependency Strategy

Keep third-party libraries optional. For example:

- **SDL2 or SFML** for optional graphics front-end (make it optional via CMake flags)
- **spdlog** for logging (optional)
- **sqlite3** for embedded DB
- **nlohmann/json** for JSON serialization
- A small procedural noise library or include your own Perlin/Simplex

### Scripts

- Provide bootstrap scripts for Linux/macOS and a PowerShell script for Windows to install deps and run CMake
- Include a **README** with quickstart, build instructions, and game controls

---

## 14. Art & Assets

You shifted toward **pixel art**. Consider the art pipeline and simple tools.

### Sprite Design

- Keep sprite sizes small (e.g., **16×16** or **32×32**) to maintain a calm aesthetic and make content creation manageable
- Color-coded sprites for player/machines/bots
- Keep animations minimal to preserve calmness (gentle pulses, slow movement)

### Tools

- **Aseprite** (paid)
- **Piskel** (free)
- Simple PNG editors

### Asset Organization

- Store assets in a well-organized `/assets` folder with license notes if using third-party art

---

## 15. UX Flow and Screens

Suggested screens and flows (you decide exact UI):

1. **Title / Main Menu**
2. **Run/resume screen** with seed and loop duration info
3. **In-run HUD** with minimap, inventory, energy meter, selected entity panel
4. **End-of-run summary screen** with upgrade tree access and machine upgrade interface
5. **Lore log / codex** where discovered fragments accumulate
6. **Developer/debug overlay** (optional) for ticks, counts, and profiling

---

## 16. Analytics, Telemetry, Testing

For development and balancing, implement optional telemetry and automated tests.

### Testing

- **Unit tests** for simulation determinism (tick-based unit tests)
- **Integration tests** for client-server action flows (local mode)

### Telemetry

- **Playtest telemetry:** run length, actions per minute, worker counts, machine usage
- Keep off by default for privacy

### Profiling

- Profiling hooks to inspect hot loops in simulation (resource updates, pathfinding)

---

## 17. Milestones & Suggested Roadmap

A suggested incremental roadmap — you decide order and exact features.

### M1 - Boilerplate

- CMake build
- Minimal engine scaffolding
- Basic tile rendering (pixel/placeholder)
- DB integration (SQLite)

### M2 - World Gen

- Chunk-based procedural generation
- Player movement
- Resource nodes and local gathering

### M3 - Central Machine

- Implement access point to feed resources
- Basic cycle timer and end-of-run handling
- Upgrade screen

### M4 - Workers

- Implement worker production
- Chain transport mechanic
- Worker tiers with cap on chain speed

### M5 - Production & Machines

- Simple machines, recipes, and queues
- Bioelectric pylons basic implementation

### M6 - QoL

- Upgrade tree UI
- Lore fragments
- Improved art, audio, and polish

### M7 - Multiplayer Readiness

- Server mode
- Authoritative simulation
- Basic networking for one remote client

### M8 - Stretch

- Node-programmable bots
- Complex machine systems
- Hostile organisms
- Replay system

---

## 18. Design Tradeoffs & Considerations (Reminders)

Things to think about as you design and implement:

- **Determinism vs performance:** deterministic tick order helps debugging and replay but may complicate parallelism
- **Infinite world performance:** chunking reduces memory but increases IO complexity
- **Bots programming complexity vs CPU cost:** heavy scripting for many bots is expensive—consider tiers and simplified behavior for low tiers
- **Upgrade pacing:** ensure loop duration growth feels meaningful but not punishing
- **Resource economy:** infinite resources require extraction caps and diminishing returns to keep progression meaningful
- **Multiplayer architecture:** designing for single-player first will save time; ensure simulation-server APIs are planned early

---

## 19. Example Data Models and API Sketches (Appendix)

### Sample High-Level DB Tables (Conceptual)

```
players:  id, name, upgrades_json, recipes_json, created_at
machine_upgrades: player_id, components_json, efficiency, input_slots, intake_speed
lore_fragments:  id, title, text, found_flag
runs: id, player_id, seed, duration_seconds, timestamp, stats_json
```

### Sample Server Actions/Messages (Conceptual)

#### Actions

- `MoveUnit(unit_id, target_tile)`
- `SpawnWorker(worker_type, target_tile)`
- `FeedMachine(resource_type, amount)`
- `BuildMachine(machine_type, tile)`

#### Queries

- `GetChunk(chunk_x, chunk_y)`

#### Events

- `TickUpdate(world_patch_json)`
- `EndOfRunSummary(summary_json)`

---

## 20. Lore & Narrative Seeds (Appendix)

### Short Lore Fragments

> _"We were grafted to the root when memory was young."_

> _"The machine sings in pulses; its hunger scours the dusk."_

> _"It remembers the first architects, but forgets their language."_

> _"Each repair stitches a new bloom into the planet's skin."_

### Narrative Beats to Consider

- **Loop 0:** Awakening—player learns movement and basic gathering; finds first lore shard
- **Loops 1-3:** Early repairs and discovering the machine's input access point; unlock worker spawning
- **Loops 4-7:** Advanced components repaired; higher-tier resources appear; small ecological changes
- **Final loops:** Player either fully repairs the core to end/reset loops or chooses a different transcendence path

---

## 21. Open Questions (For You to Decide Later)

- Exact number and names of worker tiers (you specified many—finalize names and exact capabilities)
- Precise list of resource types and their interactions
- Detailed upgrade tree design, number of nodes and branching structure
- How much of the world should be procedurally deterministic vs seeded variety
- Whether to allow players to 'bookmark' seeds or replay the same world seed
- How endgame looks — complete machine repair vs alternative transcendence

---

_Document compiled: 2025-12-30_  
_Status: Living document—update as design evolves_

