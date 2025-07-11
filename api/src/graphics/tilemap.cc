﻿#include "graphics/tilemap.h"

#include <random>
#include "noise/FastNoiseLite.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

static std::mt19937 gen{std::random_device{}()};
static std::uniform_real_distribution dist(0.f, 1.f);

void TileMap::Setup(){
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(1309);
    noise.SetFrequency(0.01f);

    textures_.Load(files_);

    std::array weights = {
        std::pair{Tile::kWater, 3.f},
        std::pair{Tile::kRock, 1.f},
        std::pair{Tile::kTree, 1.f},
        std::pair{Tile::kFood, 1.f},
        std::pair{Tile::kBg, 6.f}
    };

    // Remplir le tableau avec de l'herbe ou de l'eau de manière aléatoire
    for (int tileIndex = 0; tileIndex < tiles_.size(); ++tileIndex) {
        sf::Vector2f pos = ScreenPosition(tileIndex);

        // auto value = 0.5f * (noise.GetNoise(pos.x, pos.y) + 1);
        auto value = dist(gen);

        std::sort(weights.begin(), weights.end(), [](auto &a, auto &b) { return a.second < b.second; });

        float sumWeight = 0;
        for (auto &[tile, weight]: weights) {
            sumWeight += weight;
        }

        float localWeight = 0;
        Tile goodTile = Tile::kBg;

        for (auto &[tile, weight]: weights) {
            localWeight += weight;

            if ((value * sumWeight) < localWeight) {
                goodTile = tile;
                break;
            }
        }

        tiles_[tileIndex] = goodTile;
        if (goodTile != Tile::kWater) {
            walkables_.push_back(pos);
        }
    }

    SetZone(sf::IntRect({0, 0}, sf::Vector2i(kWidth, kHeight)));
}

void TileMap::Draw(sf::RenderWindow &window){
    int tileIndex = 0;

    sf::Sprite sprite(textures_.Get(Tile::kEmpty));

    sprite.setTextureRect(sf::IntRect({0, 0}, {kPixelStep, kPixelStep}));

    for (auto tile: tiles_) {
        sprite.setTexture(textures_.Get(tile));
        sprite.setPosition(ScreenPosition(tileIndex));
        window.draw(sprite);

        tileIndex++;
    }
}

void TileMap::SetTile(int idx, Tile tile) {
	if (idx > 0 && idx < tiles_.size()) {
		tiles_[idx] = tile;
	}
}

const std::vector<sf::Vector2f> &TileMap::GetWalkables() const{ return walkables_; }

std::vector<int> TileMap::GetCollectibles(Tile search_tile) const{

    std::vector<int> collectibles;

    for (int tile_index = 0; tile_index < tiles_.size(); ++tile_index) {
        if (tiles_[tile_index] == search_tile) {
            collectibles.emplace_back(tile_index);
        }
    }

    return collectibles;

}

sf::Vector2f TileMap::ScreenPosition(const int index){
    float x = ceil((index % (kWidth / kPixelStep)) * kPixelStep);
    float y = ceil((index / (kWidth / kPixelStep)) * kPixelStep);

    // std::cout << "Calculated Screen Position : " << x << "." << y << "\n";

    return {x, y};
}

int TileMap::Index(const sf::Vector2f screenPosition){
    return static_cast<int>(ceil(screenPosition.y / kPixelStep * kWidth)) +
           static_cast<int>(ceil(screenPosition.x / kPixelStep));
}

sf::Vector2f TileMap::TilePos(sf::Vector2i pos){
    return {
        static_cast<float>(round(pos.x / kPixelStep) * kPixelStep),
        static_cast<float>(round(pos.y / kPixelStep) * kPixelStep)
    };
}
