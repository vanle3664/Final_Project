#include "FlatForm.h"

Flatform::~Flatform()
{
}

void Flatform::CheckInScreen() 
{
	Vector2 pos = Get2DPosition();
	Vector2 size = GetSize();

	if (pos.x + size.x * 0.5 < 0 || pos.x - size.x * 0.5 > screenWidth)
	{
		m_isInScreen = false;
	}
	else {
		m_isInScreen = true;
	}
}
void Flatform::Update(GLfloat deltatime)
{
	Vector2 pos = Get2DPosition();
	pos.x = pos.x + xSpeed * deltatime;
	Set2DPosition(pos);
	//Update(deltatime);
}
