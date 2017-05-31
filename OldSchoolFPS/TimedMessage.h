#pragma once

#include "Common.h"

class TimedMessage
{
public:

	bool m_markForDeletion = false;

	TimedMessage(char* message, void* font, int x, int y, float time)
	{
		m_initTime = glutGet(GLUT_ELAPSED_TIME);
		m_x = x;
		m_y = y;
		m_font = GLUT_BITMAP_8_BY_13;
		m_str = message;
		m_timeToEnd = time;

		char *c;
		for (c = message; *c != '\0'; c++) {
			m_messageSize++;
		}
	}

	~TimedMessage() {}

	void DrawMessage()
	{
		renderBitmapString((m_x/2) - ((m_messageSize/2) * 8), m_y / 2, 0, m_font, (char*)m_str);

		float time = glutGet(GLUT_ELAPSED_TIME);
		if (time - timebase > 1000) {
			timebase = time;
		}
		m_elapsedTime = time - m_initTime;

		if (m_elapsedTime > m_timeToEnd * 1000)
		{
			m_markForDeletion = true;
		}
	}

private:
	float m_initTime, m_timeToEnd, m_elapsedTime = 0, timebase = 0;
	char* m_str;
	void* m_font;
	int m_x, m_y;
	int m_messageSize = 0;

	void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

		char *c;
		glRasterPos3f(x, y, z);
		for (c = string; *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
		}
	}
};
