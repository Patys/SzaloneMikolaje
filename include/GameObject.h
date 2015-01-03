#pragma once

#include <SFML/Graphics.hpp>

class GameObject
{
 public:
 GameObject(): pos(sf::Vector2f(0,0)), vel(sf::Vector2f(0,0)), size(sf::Vector2f(0,0)) {}
 GameObject(sf::Vector2f _pos, sf::Vector2f _vel, sf::Vector2f _size): pos(_pos), vel(_vel), size(_size) {}


  sf::Vector2f pos;
  sf::Vector2f vel;
  sf::Vector2f size;
};
