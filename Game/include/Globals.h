#pragma once

namespace rpg{

const auto gameTitle = "Fantasy RPG 2d"; //For init Engine
const auto videoMode {sf::VideoMode(1600, 900)}; //For init Engine
const auto frameTime = sf::seconds(1.f / 60.f); //used by box2d
constexpr float SCALE = 0.5; //used by box2d
constexpr float GRID = 64.f; //Grid size in tile map
constexpr int LDtkGRID = 16; //Grid size in tile map
constexpr int LDtkSCALE = 4; //Grid size in tile map
constexpr int LDtkSCALEDGRID = LDtkGRID / LDtkSCALE; //Grid size in tile map
}
