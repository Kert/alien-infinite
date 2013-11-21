#ifndef _entities_h_
#define _entities_h_ 

#include <SDL.h>

class Entity
{
	public:
		float x, y;
		int width;
		int height;
		SDL_Rect rect;
};

class DynamicEntity : public Entity
{
	public:
		float xVel, yVel;
		bool direction;
		int move_vel;
		bool onground;
};

class Player : public DynamicEntity
{

    public:
    //Initializes the variables
    Player();

	float get_xvelocity();
	float get_yvelocity();
	bool get_direction();
	void set_direction(bool direction);
	void get_pos(int &x, int &y);
	void set_pos(float x, float y);
	void get_pos_rect(int &x, int &y);
} player;

Player::Player()
{
	move_vel = 380;
	//player_height = 26;
	//player_width = 30;
	height = 32*2;
	width = 24*2;

    //Initialize the offsets
    x = 0;
    y = 0;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;

	rect.h = height; rect.w = width;

	onground = false;
}

float Player::get_xvelocity()
{
	return xVel;
}

float Player::get_yvelocity()
{
	return yVel;
}

bool Player::get_direction()
{
	return direction;
}

void Player::set_direction(bool direction)
{
	this->direction = direction;
}

void Player::get_pos(int &x, int &y)
{
	x = (int)this->x;
	y = (int)this->y;
}

void Player::set_pos(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Player::get_pos_rect(int &x, int &y)
{
	x = rect.x;
	y = rect.y;
}

#endif