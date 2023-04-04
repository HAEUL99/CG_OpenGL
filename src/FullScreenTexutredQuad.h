#ifndef __fullscText__
#define __fullscText__


#include "context.h"

class FullScreenTexturedQuad {
public:
	void Settings(int windowWidth, int windowHeight);

	void LoadTexture(std::vector<Vec4f>& image);

	void Draw();
    unsigned int vao = 0, textureId = 0;
};

#endif
