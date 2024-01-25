#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <gba_base.h>

#include <sstream>
#include <iostream>
#include <string>
#include <memory>

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <cmath>

#include <gba_os/Clock/GbaClock.h>
#include <gba_os/Core.h>
#include <gba_os/Screen.h>
#include <gba_os/Graphics/Graphics.h>
#include <gba_os/Console.h>

#include "cube_obj.h"
#include "cube_mtl.h"
#include "gba_os/Graphics/Scene.h"

#include <libfixmath/fixmath.h>

#define PALETTE_SIZE 256

using namespace gba_os::console;

// Function to create a 16-bit color from red, green, and blue components
u16 createColor(u8 red, u8 green, u8 blue) {
    return (red & 0x1F) | ((green & 0x1F) << 5) | ((blue & 0x1F) << 10);
}

// Function to fill the palette buffer with a spread of colors
void fillPaletteBuffer(u16* paletteBuffer) {
    for (int i = 0; i < PALETTE_SIZE; ++i) {
        u8 red = (i * 7) % 32;   // Cycle through red values
        u8 green = (i * 13) % 32; // Cycle through green values
        u8 blue = (i * 19) % 32;  // Cycle through blue values

        paletteBuffer[i] = createColor(red, green, blue);
    }
}

void update_scene(gba_os::Task& t){
    gba_os::graphics::Scene* scene = (gba_os::graphics::Scene*)t.data;
    scene->update();
}

void buttons_task(gba_os::Task& t){
    std::shared_ptr<gba_os::graphics::Camera>& camera = *(std::shared_ptr<gba_os::graphics::Camera>*)t.data;
    // Get button input
    u16 buttons = keysDown();

    // Change camera rotation by 1 when a button is pressed
    if (buttons & KEY_RIGHT) {
        camera->rotation.z += F16(1);
    }
    if (buttons & KEY_B) {
        camera->rotation.y += F16(1);
    }
    if (buttons & KEY_L) {
        camera->rotation.x += F16(1);
    }
    if (buttons & KEY_LEFT) {
        camera->rotation.z -= F16(1);
    }
}

void run_mode_4_demo(){
    uint16_t * pallet = new uint16_t[256];
    fillPaletteBuffer(pallet);
    // Create a scene object
    gba_os::graphics::Scene scene;

    gba_os::init_gba_os();

    gba_os::Task mode4;
    mode4.f = &gba_os::tasks::update_screen_mode_4_task;
    mode4.data = (void*)scene.get_buffer_ptr();
    mode4.priority = (gba_os::TASK_PRIORITY)0;
    int mode4_task_id = gba_os::register_task(mode4);

    gba_os::graphics::Mesh mesh;
    gba_os::graphics::LoadOBJFile(cube_obj, cube_obj_size, mesh);
    gba_os::graphics::Material material;
    gba_os::graphics::LoadMTLFile256Colors(cube_mtl, cube_mtl_size, material);
    gba_os::graphics::Vector3 position(1, 0, 0);
    int id = 1;
    
    scene.register_entity(
        std::make_shared<gba_os::graphics::Mesh>(mesh), 
        std::make_shared<gba_os::graphics::Material>(material), 
        std::make_shared<gba_os::graphics::Vector3>(position), 
        id
    );

    std::shared_ptr<gba_os::graphics::Camera> camera = std::make_shared<gba_os::graphics::Camera>();
    scene.register_camera(camera);
    camera->pos = {F16(0), F16(0), F16(0)};
    camera->rotation = {F16(0), F16(0), F16(0)};

    gba_os::Task updateSceneTask;
    updateSceneTask.f = &update_scene;
    updateSceneTask.data = &scene;
    updateSceneTask.priority = gba_os::TASK_PRIORITY::MEDIUM;
    gba_os::register_task(updateSceneTask);

    gba_os::Task buttons_task_;
    buttons_task_.f = &buttons_task;
    buttons_task_.data = &camera;
    buttons_task_.priority = gba_os::TASK_PRIORITY::HIGH;
    gba_os::register_task(buttons_task_);

    gba_os::graphics::set_palette_mode_4(pallet);
    gba_os::screen::set_mode4();
    gba_os::set_framerate(gba_os::video::FRAMERATE30);
    
    gba_os::run_gba_os();   
}

// Start the timer
void startTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value

    // Set control to start the timer with a 1:1 prescaler and enable interrupts if needed
    REG_TM0CNT_H = TIMER_START;
}

// Stop the timer and return elapsed time in clock cycles
u16 stopTimer() {
    // Disable the timer
    REG_TM0CNT_H &= ~TIMER_START;
    
    // Return the value of the timer
    return REG_TM0CNT_L;
}

// Reset the timer
void resetTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value
}

EWRAM_CODE void run_console_test(){
    // initialise the console
	// setting NULL & 0 for the font address & size uses the default font
	// The font should be a complete 1bit 8x8 ASCII font
	consoleInit(	0,		// charbase
					4,		// mapbase
					0,		// background number
					NULL,	// font
					0, 		// font size
					15		// 16 color palette
	);

	// set the screen colors, color 0 is the background color
	// the foreground color is index 1 of the selected 16 color palette
	BG_COLORS[0]=RGB8(58,110,165);
	BG_COLORS[241]=RGB5(31,31,31);

    // Set up the video mode and enable sprites
    SetMode(MODE_0 | BG0_ON | OBJ_ENABLE | OBJ_1D_MAP);
}

int main() {
    run_mode_4_demo();

    return 0;
}
