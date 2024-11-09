#include "fade.h"
#include "Main/main.h"
#include "Manager/textureManager.h"

constexpr int FADE_MAX_ALPHA = 255;
constexpr int FADE_MIN_ALPHA = 0;
constexpr float FADE_DURATION = 1;

bool Fade::UpdatingFadeMode(const float& deltatime)
{
	if (m_FadeMode != FADE_MODE::FADE_NONE)
	{
		m_CurrentTime += deltatime;
	}
	
	if (m_FadeMode == FADE_MODE::FADE_OUT)
	{
		m_FadeAlpha = static_cast<int>((FADE_MAX_ALPHA * m_CurrentTime) / FADE_DURATION);
		if (m_FadeAlpha > 255)
		{
			m_FadeAlpha = 255; 
			m_CurrentTime = 0;
			m_FadeMode = FADE_MODE::FADE_FINISHED;
		}
	}
	else if (m_FadeMode == FADE_MODE::FADE_IN)
	{
		m_FadeAlpha = static_cast<int>(FADE_MAX_ALPHA - (FADE_MAX_ALPHA * m_CurrentTime) / FADE_DURATION);
		if (m_FadeAlpha < 0)
		{
			m_FadeAlpha = 0;
			m_CurrentTime = 0;
			m_FadeMode = FADE_MODE::FADE_NONE;
		}
	}
	
	return m_FadeMode != FADE_MODE::FADE_NONE;
}

void Fade::Draw()
{
	if (m_FadeMode != FADE_MODE::FADE_NONE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FadeAlpha);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, m_FadeAlpha);
	}

}

void Fade::StartFadeIn()
{	
	m_FadeMode = FADE_MODE::FADE_IN;
	m_FadeAlpha = 255;
}

void Fade::StartFadeOut()
{
	m_FadeMode = FADE_MODE::FADE_OUT;
	m_FadeAlpha = 0;
}
