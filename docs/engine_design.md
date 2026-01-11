# Meeting 1 (insert into document properly)
Ideas:
- Entities will not be added to any archetype until they are 'publised'. At this point, the entity will be evaluated by the engine, and based on which components are added, the entity will be added to a certain archetype.
- Regarding the 'publishing' method: the engine has to determine which componenent types are added. How can I do this in the easiest way? Through an abstract factory (don't remember how to do this lol).

Questions and things I don't understand:
- How are components 'linked' to entities? Do the entities just contain some list of references / pointers to the components? And do the entities own the components?
- How are the components, which are stored in chunks, linked to the corresponding entities?

Discussion Sean:
- Specific systems work only for specific components. Therefore, the components offered by the engine will be modified / controlled by the systems in the engine (for example: the speed component is controlled by the physics system)
Also, components added by the user can only be controlled by systems added by the user. These must be dynamically added, so that the correct archetypes can be generated. Because a system for dynamically adding archetypes needs to be added anyway,
the internal systems and components of the engine can also use this dynamic system.
- It is yet tbd how the componenets will be connected to the right archetypes, and that correct chuncks are generated out of this.


# Things we should think about more:
- Behaviour scripts: how do these interface with the engine user?
- Scene system: how will we do this (scenes)?
- Entity system: ??
- Command buffer?