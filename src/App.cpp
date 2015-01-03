#include <App.h>
#include <Collision.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

App::App():
  window(sf::VideoMode(800, 600), L"Szalone Mikołaje by P@tys"),
  platform(sf::Vector2f(266,0),sf::Vector2f(0,0),sf::Vector2f(64,32)),
  s_snow_explosion(sf::seconds(0.09), true, false)
{
  srand(time(0));
  window.setFramerateLimit(60);
  s_platform.setTexture(tex_menager.getTexture("data/platform-snow.png"));
  s_snow.setTexture(tex_menager.getTexture("data/snow.png"));
  s_snowball.setTexture(tex_menager.getTexture("data/snowball.png"));
  s_santa.setTexture(tex_menager.getTexture("data/santaandsnowman.png"));
  s_background.setTexture(tex_menager.getTexture("data/snow-background.png"));

  font.loadFromFile("data/Raleway-Bold.otf");
  t_score.setFont(font);
  t_timer.setFont(font);
  t_info.setFont(font);
  t_info.setString(L"Sterowanie:\nA/D lub <-/->\nSPACE-Strzał\nENTER-Start\n\nMasz 30s, aby uzyskać\njak najlepszy wynik.");
  sf::FloatRect textRect1 = t_info.getLocalBounds();
  t_info.setOrigin(textRect1.left + textRect1.width/2.0f,
		    textRect1.top  + textRect1.height/2.0f);
  t_info.setPosition(400,250);

  snow_explosion.setSpriteSheet(tex_menager.getTexture("data/explosion_snow.png"));
  snow_explosion.addFrame(sf::IntRect(0,0,100,100)); 
  snow_explosion.addFrame(sf::IntRect(100,0,100,100)); 
  snow_explosion.addFrame(sf::IntRect(200,0,100,100)); 
  snow_explosion.addFrame(sf::IntRect(300,0,100,100)); 
  snow_explosion.addFrame(sf::IntRect(400,0,100,100)); 
  
  game_music.openFromFile("data/carnivalrides.ogg");
  game_music.setLoop(true);
  game_music.setVolume(80);
  game_music.play();

  chop_sound_buffer.loadFromFile("data/chop.ogg");
  fire_sound_buffer.loadFromFile("data/fire.ogg");

  chop_sound.setBuffer(chop_sound_buffer);
  fire_sound.setBuffer(fire_sound_buffer);

  score = 0;
  game_timer = 0;
  state = MENU;

  for(std::size_t i = 0; i < 50; i++)
    {

      snow.push_back(GameObject(sf::Vector2f(rand()%800-32,-32), //pos
				sf::Vector2f(0,rand()%400+100), //vel
				sf::Vector2f(32,32))); //size
    }

}

void App::run()
{
  sf::Clock game_clock;
  sf::Time frame_time;
  while (window.isOpen())
    {
      // Process events
      sf::Event event;
      while (window.pollEvent(event))
	{
	  // Close window: exit
	  if (event.type == sf::Event::Closed)
	    window.close();
	}
      frame_time = game_clock.restart();

      update(frame_time.asSeconds());
      s_snow_explosion.update(frame_time);
      draw();
    }
}

void App::update(float delta_time)
{
  snow_update(delta_time);
  santas_update(delta_time);
  platform_update(delta_time);
  snowballs_update(delta_time);

  if(state == GAME)
    game_timer -= delta_time;

  for(std::size_t i = 0; i < snowballs.size(); i++)
    {
      for(std::size_t n = 0; n < santas.size(); n++)
	{
	  bool is_collision =  isCollision(snowballs[i].pos, snowballs[i].size,
					   santas[n].pos, santas[n].size);
	  if(is_collision)
	    {
	      s_snow_explosion.setPosition(sf::Vector2f(snowballs[i].pos.x-50, snowballs[i].pos.y-50));
	      s_snow_explosion.play(snow_explosion);
	      chop_sound.play();
	      snowballs.erase(snowballs.begin()+i);
	      santas.erase(santas.begin()+n);
	      if(state == GAME)
		score += 5;
	    }
	}
    }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
      state = GAME;
      score = 0;
      game_timer = 30;
    }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
      state = MENU;
    }

  if(game_timer < 0)
    state = MENU;

  std::ostringstream _score_string;
  _score_string << score;
	  
  t_score.setString("Punkty: " +  _score_string.str());
  sf::FloatRect textRect = t_score.getLocalBounds();
  t_score.setOrigin(textRect.left + textRect.width/2.0f,
		    textRect.top  + textRect.height/2.0f);
  t_score.setPosition(sf::Vector2f(400,textRect.top  + textRect.height/2.0f + 35));

  std::ostringstream _timer_string;
  _timer_string << (int)game_timer;
	  
  t_timer.setString( _timer_string.str() + "s");
  sf::FloatRect textRect1 = t_timer.getLocalBounds();
  t_timer.setOrigin(textRect1.left + textRect1.width/2.0f,
		    textRect1.top  + textRect1.height/2.0f);
  t_timer.setPosition(sf::Vector2f(400,textRect1.top  + textRect1.height/2.0f + 65));

}

void App::platform_update(float delta_time)
{
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || 
     sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
      platform.vel.x = -400;
    }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || 
     sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
      platform.vel.x = 400;
    }

  sf::Vector2f next_pos = platform.pos + platform.vel * delta_time;

  if(next_pos.x < 0 || 
     next_pos.x > 800-64)
    platform.vel = sf::Vector2f(0,0);

  platform.pos += platform.vel * delta_time;

  platform.vel = sf::Vector2f(0,0);
}

void App::santas_update(float delta_time)
{
  static float timer = 0;
  timer += delta_time;
  if(timer > 0.81)
    {
      santas.push_back(GameObject(sf::Vector2f(800, rand()%436+64),
				  sf::Vector2f(-(rand()%400+100),0),
				  sf::Vector2f(100,100)));
      timer = 0;
    }
  for(std::size_t i = 0; i < santas.size(); i++)
    {
      sf::Vector2f next_pos = santas[i].pos + santas[i].vel * delta_time;

      santas[i].pos += santas[i].vel * delta_time;

      if(next_pos.x + santas[i].size.x < 0)
	santas.erase(santas.begin() + i);
    }
}

void App::snowballs_update(float delta_time)
{
  static float timer = 0;
  timer += delta_time;
  if(timer > 0.2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
      snowballs.push_back(GameObject(sf::Vector2f(platform.pos.x + platform.size.x / 2, platform.pos.y),
				     sf::Vector2f(0,500),
				     sf::Vector2f(32,32)));
      fire_sound.play();
      if(state == GAME)
	score -= 1;
      timer = 0;
    }

  for(std::size_t i = 0; i < snowballs.size(); i++)
    {
      sf::Vector2f next_pos = snowballs[i].pos + snowballs[i].vel * delta_time;

      snowballs[i].pos = next_pos;

      if(next_pos.y + snowballs[i].size.y > 600)
	{
	  snowballs.erase(snowballs.begin()+i);
	}
    }

}

void App::snow_update(float delta_time)
{
  for(std::size_t i = 0; i < snow.size(); i++)
    {
      sf::Vector2f next_pos = snow[i].pos + snow[i].vel * delta_time;
      if(next_pos.y > 600)
	{
	  snow[i].pos.y = -32;
	  snow[i].pos.x = rand()%800-32;
	}
      snow[i].pos += snow[i].vel * delta_time;
    }
}
void App::draw()
{
  window.clear();
  window.draw(s_background);
  s_platform.setPosition(platform.pos);
  window.draw(s_platform);
  
  for(auto x : santas)
    {
      s_santa.setPosition(x.pos);
      window.draw(s_santa);
    }

  for(auto x : snowballs)
    {
      s_snowball.setPosition(x.pos);
      window.draw(s_snowball);
    }
  if(s_snow_explosion.isPlaying())
    window.draw(s_snow_explosion);

  for(auto x : snow)
    {
      s_snow.setPosition(x.pos);
      window.draw(s_snow);
    }
  if(state == MENU)
    window.draw(t_info);
  if(state == GAME)
    window.draw(t_timer);
  window.draw(t_score);
  window.display();
}
