#include "entities.h"

Player player;

Player::Player()
{
	move_vel = 380;
	in_air_vel = 220;
	jumptime = 0;
	accel = 5;
	jumping = false;
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