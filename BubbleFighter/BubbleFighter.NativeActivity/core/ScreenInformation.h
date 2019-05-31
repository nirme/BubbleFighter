#pragma once


struct ScreenInformation
{
	int dpi;
	int width, height;
	float ratio;

	void setup(android_app* state)
	{
		dpi = AConfiguration_getDensity(state->config);

		width = ANativeWindow_getWidth(state->window);
		height = ANativeWindow_getHeight(state->window);

		ratio = (float) width / height;
	};
};