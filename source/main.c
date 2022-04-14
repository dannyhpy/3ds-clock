#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <3ds.h>
#include <citro2d.h>

#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240

u64 now;

void drawLineFromCircleCircumference(float angle, float cx, float cy, float r0, float r1, u32 color, float thickness)
{
  float x1 = cx + r0 * sin(angle);
  float y1 = cy + r0 * -cos(angle);

  float x2 = cx + r1 * sin(angle);
  float y2 = cy + r1 * -cos(angle);

  C2D_DrawLine(x1, y1, color, x2, y2, color, thickness, 0);
}

bool draw(C3D_RenderTarget* renderTargetPtr)
{
  gspWaitForVBlank();
  //gfxScreenSwapBuffers(GFX_TOP, false);

  // Common colors I use
  u32 blackColor = C2D_Color32f(0, 0, 0, 1);
  u32 brownColor = C2D_Color32f(0.28f, 0.08f, 0.08f, 1);
  u32 whiteColor = C2D_Color32f(0.8f, 0.8f, 0.8f, 1);
  u32 grayColor = C2D_Color32f(0.436f, 0.436f, 0.436f, 1);
  u32 redColor = C2D_Color32f(1.0f, 0.1f, 0.1f, 1);

  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(renderTargetPtr, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
  C2D_SceneBegin(renderTargetPtr);

  // Circle
  C2D_DrawCircleSolid(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2, 0, 110, brownColor);
  C2D_DrawCircleSolid(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2, 0, 100, blackColor);
  C2D_DrawCircleSolid(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2, 0, 95, whiteColor);

  // Lines
  float cx = TOP_SCREEN_WIDTH / 2;
  float cy = TOP_SCREEN_HEIGHT / 2;
  float r0 = 80;
  float r1 = 95;
  float thickness = 2;

  for (float i = 0; i < 12; i++)
  {
    float angle = (i / 12) * (2 * M_PI);
    drawLineFromCircleCircumference(angle, cx, cy, r0, r1, blackColor, thickness);
  }

  for (float i = 0; i < 60; i++)
  {
    if ((int)i % 5 == 0) continue;
    float angle = (i / 60) * (2 * M_PI);
    drawLineFromCircleCircumference(angle, cx, cy, r0 + 5, r1, blackColor, thickness - 1);
  }

  // Actually more interesting lines to draw
  float r2 = 0;
  float r3 = 70;
  float seconds = now / 1000 % 60;
  float minutes = (now / 1000 / 60 % 60) + (seconds / 60);
  float hours = (now / 1000 / 60 / 60 % 12) + (minutes / 60);

  float secondsAngle = (seconds / 60) * (2 * M_PI);
  float minutesAngle = (minutes / 60) * (2 * M_PI);
  float hoursAngle = (hours / 12) * (2 * M_PI);

  drawLineFromCircleCircumference(secondsAngle, cx, cy, r2, r3, redColor, thickness);
  drawLineFromCircleCircumference(minutesAngle, cx, cy, r2, r3, grayColor, thickness);
  drawLineFromCircleCircumference(hoursAngle, cx, cy, r2, r3, grayColor, thickness + 2);

  // Central point
  C2D_DrawCircleSolid(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2, 0, 5, blackColor);

  C3D_FrameEnd(0);
  return true;
}

int main(int argc, char* argv[])
{
	gfxInitDefault();

  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  C3D_RenderTarget* renderTargetPtr = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	consoleInit(GFX_BOTTOM, NULL);

	printf("START = Quit\n");

  draw(renderTargetPtr);

	// Main loop
	while (aptMainLoop())
	{
    now = osGetTime();

		//gspWaitForVBlank();
		//gfxScreenSwapBuffers(GFX_BOTTOM, false);
		hidScanInput();

		// input managment
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

    if (now % 100 == 0)
    {
      draw(renderTargetPtr);
    }
	}

  C2D_Fini();
  C3D_Fini();
	gfxExit();
	return 0;
}
