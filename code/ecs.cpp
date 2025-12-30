class component {};

class velocity : public component {
  int x, y, z;
};

class position : public component {
  int x, y, z;
};

class health : public component {
  int hp;
};

class entity {
  int id;
};

class archetype {
  // stores component types
  virtual int maxEntities;
  virtual int id;
  virtual uint32_t componentMask;
  virtual std::list<chunk*> chunks;
};

class velPosArchetype : public archetype {
  int maxEntities = (16 * 1024) - 8 - 16 / (sizeof(velocity) + sizeof(position));
  int id = 1;
  uint16_t componentMask = 0b1100000000000000; // velocity + position
};

class velHealthArchetype : public archetype {
  int maxEntities = (16 * 1024) - 8 - 16 / (sizeof(velocity) + sizeof(health));
  int id = 2;
  uint16_t componentMask = 0b1010000000000000; // velocity + health
};

class chunk {
  // stores entities and their components
  virtual archetype* archetypeType;
  virtual int entityCount;
  virtual std::array<entity, 1> entities;
};

VelPosArchetype* sharedVelPosArchetype = new VelPosArchetype();

class velPosChunk : public chunk {
  archetype* archetypeType = sharedVelPosArchetype;
  int entityCount = 0;
  std::array<entity, sharedVelPosArchetype->maxEntities> entities;
  std::array<velocity, sharedVelPosArchetype->maxEntities> velocities;
  std::array<position, sharedVelPosArchetype->maxEntities> positions;

  
  uint32_t versionVelocity = 0;
  uint32_t versionPosition = 0;
};


