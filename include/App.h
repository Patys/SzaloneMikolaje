#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GameObject.h>
#include <ResourceMenager.h>

#include <animated_sprite.h>

enum GAME_STATE { MENU, GAME };

class App
{
 public:
  App();
  void run();
 private:
  void update(float delta_time);
  void draw();
  
  sf::RenderWindow window;
  
  void platform_update(float delta_time);
  void snow_update(float delta_time);
  void santas_update(float delta_time);
  void snowballs_update(float delta_time);

  // data
  std::vector<GameObject> snowballs;
  std::vector<GameObject> santas;
  std::vector<GameObject> snow;
  GameObject platform;

  float game_timer;

  ResourceMenager tex_menager;

  int score;

  GAME_STATE state;

  // animation
  Animation snow_explosion;
  AnimatedSprite s_snow_explosion;

  // sf::Music
  sf::Music game_music;

  // sf::SoundBuffer
  sf::SoundBuffer chop_sound_buffer;
  sf::SoundBuffer fire_sound_buffer;

  sf::Sound chop_sound;
  sf::Sound fire_sound;

  // sf::Font
  sf::Font font;

  // sf::Text
  sf::Text t_score;
  sf::Text t_timer;
  sf::Text t_info;
  sf::Text t_info_score;

  // sf::Sprite
  sf::Sprite s_background;
  sf::Sprite s_snow;
  sf::Sprite s_snowball;
  sf::Sprite s_santa;
  sf::Sprite s_platform;
};
