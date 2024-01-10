#include "Core.h"

namespace gba_os{

IWRAM_DATA static video::FRAMERATE framerate_ = video::FRAMERATE60;
IWRAM_DATA static volatile unsigned int frame_count = 0;
IWRAM_DATA static std::vector<task> tasks_ = {};
IWRAM_DATA static std::vector<Callback> vblank_callbacks = {};
IWRAM_DATA static chrono::gba_milliseconds frame_duration = chrono::gba_milliseconds(1000 / 60);

void set_framerate(video::FRAMERATE framerate){
    framerate_ = framerate;

    switch(framerate_){
        case video::FRAMERATE60:
            frame_duration = chrono::gba_milliseconds(1000 / 60);
            break;
        case video::FRAMERATE30:
            frame_duration = chrono::gba_milliseconds(1000 / 30);
            break;
        case video::FRAMERATE15:
            frame_duration = chrono::gba_milliseconds(1000 / 15);
            break;
    }
}

IWRAM_CODE ARM_CODE static void vblank_os_callback(){
    switch(framerate_){
        case video::FRAMERATE60:
            break;
        case video::FRAMERATE30:
            frame_count = (frame_count + 1) % 2;
            break;
        case video::FRAMERATE15:
            frame_count = (frame_count + 1) % 4;
            break;
    }

    for(auto cb : vblank_callbacks)
        cb.f(cb.data);
}

IWRAM_CODE ARM_CODE void waitNextFrame(){
    switch (framerate_)
    {
    case video::FRAMERATE60:
        VBlankIntrWait();
        break;
    default:
        while(frame_count == 0);
        while(frame_count != 0);
        break;
    }
}

void init_gba_os(){
    irqInit();
    irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, &vblank_os_callback);
}

void error_state(chrono::gba_milliseconds s){
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
    
    printf("\033[16;10ERROR!");
    printf("\033[17;10Exceeded frame duration: %i", s.count());
    while(1);
}

IWRAM_CODE ARM_CODE void run_gba_os(){
    auto clock = chrono::GbaClock::instance();
    chrono::GbaClock::time_point start;
    chrono::gba_milliseconds delta;
    waitNextFrame();
    while(1){
        start = clock.now();
        for(auto task: tasks_){
            task.f(task.data);
        }
        delta = std::chrono::duration_cast<chrono::gba_milliseconds>(clock.now() - start);
        if (delta > frame_duration)
            error_state(delta);
        waitNextFrame();
    }
}

int register_task(task t){
    static uint32_t task_id_counter = 0;
    t.id = task_id_counter++;
    tasks_.push_back(t);
    std::sort(tasks_.begin(), tasks_.end(), [](task a, task b){ return (a.priority < b.priority); });
    return t.id;
}

void unregister_task(int id){
    std::erase_if(tasks_, [&id](task t){return t.id == id;});
}

int register_vblank_interrupt_callback(Callback cb){
    static uint32_t interrupt_id = 0;
    cb.id = interrupt_id++;
    vblank_callbacks.push_back(cb);
    return cb.id;
}

void unregister_vblank_interrupt_callback(int id){
    std::erase_if(vblank_callbacks, [&id](Callback cb){return cb.id == id;});
}

}
