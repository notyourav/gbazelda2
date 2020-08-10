//---------------------------------------------------------------------------------
// GBA ZELDA 2 MAIN
//---------------------------------------------------------------------------------
#include "global.h"
#include "main.h"
#include "player.h"
#include "oam.h"

u16 keysDownRaw;
Input inputDown;
u16 keysHeldRaw;
Input inputHeld;
u32 frame;
Pos camera;

//---------------------------------------------------------------------------------
// Interrupts and timing
//---------------------------------------------------------------------------------
void VblankInterrupt()
{
	frame += 1;
	scanKeys();
	keysDownRaw = keysDown();
	inputDown = *(Input *)&keysDownRaw;
	keysHeldRaw = keysHeld();
	inputHeld = *(Input *)&keysHeldRaw;
}

inline void vsync()
{
    while (REG_VCOUNT >= 160);
    while (REG_VCOUNT < 160);
}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
{
	// Set up the interrupt handlers
	irqInit();
	irqSet(IRQ_VBLANK, VblankInterrupt);
	irqEnable(IRQ_VBLANK);

	// Allow Interrupts
	REG_IME = 1;

	SetMode( MODE_1 | OBJ_ENABLE | OBJ_1D_MAP );
	SetupPlayer();
	camera.x = 0;
	camera.y = 0;
	while (1)
	{
		VBlankIntrWait();
		// if (keysDown() & 1) doVSync = !doVSync;
		// if (doVSync) vsync();

		PlayerLoop();
		CopyOAMBuffer();
	}
}