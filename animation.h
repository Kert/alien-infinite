#ifndef _animation_h_
    #define _animation_h_

#include <SDL.h>

class Animation {
    private:
        int    CurrentFrame;
        int     FrameInc;
 
    private:
        int     FrameRate; //Milliseconds
        long    OldTime;
 
	public:
        int    MaxFrames;
        bool    Oscillate;
 
    public:
        Animation();
        void Animate();
 
    public:
        void SetFrameRate(int Rate);
        void SetCurrentFrame(int Frame);
        int GetCurrentFrame();
};

#endif