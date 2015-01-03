
#include <Collision.h>

// // //
bool isCollision(float x1, float y1, float w1, float h1,
                 float x2, float y2, float w2, float h2 )
{
  if( x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2 )
    return( true );
  return( false );
}

bool isCollision(sf::Vector2f pos, sf::Vector2f size,
                 sf::Vector2f pos2, sf::Vector2f size2 )
{
  if( pos.x + size.x >= pos2.x && pos.x <= pos2.x + size2.x &&
      pos.y + size.y >= pos2.y && pos.y <= pos2.y + size2.y )
    return( true );
  return( false );
}
