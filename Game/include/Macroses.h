#pragma once
//#include "Engine.h"

#define MGR dynamic_cast<ECSystem *>(rpg::Engine::get("ECSManager"))
#define GUI (dynamic_cast<Gui *>(rpg::Engine::get("Gui")))->get()
#define PHYS dynamic_cast<Physics *>(rpg::Engine::get("Physics"))
#define NAVSYS dynamic_cast<NavSystem *>(rpg::Engine::get("Navigation"))
#define AUDIO dynamic_cast<Audio *>(rpg::Engine::get("Audio"))
#define COLLSYS dynamic_cast<CollisionSystem *>(rpg::Engine::get("Collisions"))
#define AISYS dynamic_cast<AI *>(rpg::Engine::get("AI"))
#define COMMAND dynamic_cast<CommandQueue *>(rpg::Engine::get("CommandQueue"))

#define RWIND rpg::Engine::window()
#define ID rpg::helper::Id::getId()
