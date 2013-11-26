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
		bool onground;
		int in_air_vel;
		int jumptime;
		float accel;
		bool jumping;
		int move_vel;
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
};

#endif