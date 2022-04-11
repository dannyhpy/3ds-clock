#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>

#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240

bool draw(C3D_RenderTarget* renderTargetPtr)
{
  gspWaitForVBlank();
  //gfxScreenSwapBuffers(GFX_TOP, false);

  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(renderTargetPtr, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
  C2D_SceneBegin(renderTargetPtr);
  C2D_DrawCircleSolid(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2, 0, 100, C2D_Color32f(0.6f, 0.6f, 0.6f, 1.0f));
  C2D_DrawCircleSolid(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2, 0, 95, C2D_Color32f(0.3f, 0.3f, 0.3f, 1.0f));
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

	printf("START   =   Quit\n");

  draw(renderTargetPtr);

	// Main loop
	while (aptMainLoop())
	{
		//gspWaitForVBlank();
		//gfxScreenSwapBuffers(GFX_BOTTOM, false);
		hidScanInput();

		// input managment
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

    if (kDown & KEY_SELECT)
      draw(renderTargetPtr);

    if (kDown & KEY_A) {
      gspWaitForVBlank();
      consoleClear();
      printf("osGetTime() = %llu\n", osGetTime());
      printf("\n");

      osTimeRef_s timeRef = osGetTimeRef();
      printf("osGetTimeRef() =\n");
      printf("  value_ms: %llu\n", timeRef.value_ms);
      printf("  value_tick: %llu\n", timeRef.value_tick);
      printf("  sysclock_hz: %llu\n", timeRef.sysclock_hz);
      printf("  drift_ms: %llu\n", timeRef.drift_ms);
    }
	}

  C2D_Fini();
  C3D_Fini();
	gfxExit();
	return 0;
}
