#pragma once

enum class FADE_MODE
{
	FADE_NONE,
	FADE_IN,
	FADE_OUT,
	FADE_FINISHED,
	MAX_FADE
};

class Fade
{
private:
	int		m_FadeAlpha = 0;
	float	m_CurrentTime = 0;
	FADE_MODE m_FadeMode = FADE_MODE::FADE_NONE;
public:
	Fade(){}
	~Fade(){}
	bool UpdatingFadeMode(const float& deltatime);
	void Draw();
	void StartFadeIn();
	void StartFadeOut();
	const FADE_MODE& GetFadeMode()const { return m_FadeMode; }
};