# 2d tile based RPG game

![Game_screenshot](https://github.com/larinius/2d_RPG_demo/assets/5310985/6b489a8b-2e41-4a17-b068-c064ac2ee4df)


## Tools used
- SFML - rendering framework - https://www.sfml-dev.org/
- Tgui - game gui - https://tgui.eu/
- LDtk - level editor and loader - https://ldtk.io/
- Box2d - game physics -  https://box2d.org/
- Json load - https://github.com/nlohmann/json

Video of current project state
https://youtu.be/utiu_fjylio

### What is done:
- Load game level from json "Ldtk" format (tiles, collisions, entities)
- Sprite based animations, animation states
- Auidio (music playback, sounds for interactions, only for collisions so far)
- Simple AI for monsters (roaming, idle states)
- A* path finding for Player character
- Contoll Player character with keys and mouse (move to point)
- Collision triggers for Entities
- Simple command queue for Entities
