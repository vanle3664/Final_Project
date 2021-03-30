#pragma once
#include "Sprite2D.h"

class Animation : public Sprite2D
{
private:
	std::string		m_Text;
	void			CaculateWorldMatrix();
protected:
	Vector2			m_Vec2DPos;
	GLint			m_iHeight;
	GLint			m_iWidth;
	int				m_numFrames;
	int				m_currentFrame;
	float			m_currentTime;
	float			m_frameTime;

public:
	Animation();
	Animation(int m_numFrames, float m_frameTime, std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
	Animation(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, Vector4 color);
	~Animation();

	void		Init() override;
	void		Draw() override;
	void		Update(GLfloat deltatime) override;

	void		SetText(std::string text);
	std::string	GetText();

	void		Set2DPosition(GLfloat x, GLfloat y);
	void		Set2DPosition(Vector2 pos);

	Vector2		Get2DPosition();
	void		SetSize(GLint width, GLint height);

};

