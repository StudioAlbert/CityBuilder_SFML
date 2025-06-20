﻿#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>

#include "ai/bt_node.h"
#include "assets/asset_manager.h"
#include "graphics/tilemap.h"
#include "motion/motor.h"
#include "motion/path.h"

using namespace core::ai::behaviour_tree;
using namespace api::motion;

namespace api::ai {
    class Npc {
        sf::Texture texture_;

        // Behaviour tree
        std::unique_ptr<Node> bt_tree_;

        // Behaviour Constants
        static constexpr float kHungerRate = 0.1f;
        static constexpr float kMovingSpeed = 50.0f;

        // Motion
        Motor motor_;
        Path path_;

        // Tilemap
        const TileMap* tileMap_;

    public:
        void Setup(std::string_view fileName, const TileMap* tileMap);
        void Update(float dt);

        void Draw(sf::RenderWindow &window);

        // Actions
        Status Move();
        Status Eat();

        void SetupBehaviourTree();

        // Behaviours
        //bool hungry_ = false;
        float hunger_ = 0;
        bool resourceAvailable_ = true;
        bool target_reachable_ = true;
        float target_distance_ = 20;

        // Motion
        void SetPath(const Path& path);

    };
}

#endif //NPC_H
