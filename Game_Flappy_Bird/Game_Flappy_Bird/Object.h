#ifndef OBJECT_H_
#define OBJECT_H_

#include "CommonFunction.h"

class Object
{
public:
	Object();
	~Object();
	bool LoadImageFile(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void Free();
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; };
	SDL_Rect GetRect() const { return rect_; }
	SDL_Texture* GetTexture() { return texture_; }
protected:
	SDL_Texture* texture_;
	SDL_Rect rect_;
};


#endif

