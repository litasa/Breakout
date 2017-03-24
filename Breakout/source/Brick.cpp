#include "Brick.h"

Brick::Brick(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, bool solid, bool destroyed)
	: Game_Object(pos,size,sprite,color,velocity), _is_solid(solid), _destroyed(destroyed)
{
}

Brick::Brick()
	: Game_Object(), _is_solid(false), _destroyed(false)
{
}
