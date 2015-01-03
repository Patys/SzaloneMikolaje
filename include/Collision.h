#pragma once

#include <SFML/Graphics.hpp>

// //
bool isCollision(float x1, float y1, float w1, float h1,
                 float x2, float y2, float w2, float h2 );

bool isCollision(sf::Vector2f pos, sf::Vector2f size,
                 sf::Vector2f pos2, sf::Vector2f size2 );
// //
