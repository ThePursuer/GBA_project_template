#include "gba_os/Core.h"

#include "OsErrors.h"
#include "gba_os/Clock.h"
#include "interrupt_manager.h"

namespace Gba_os{

using namespace Clock;

IWRAM_DATA static video::FRAMERATE framerate_ = video::FRAMERATE60;
IWRAM_DATA static volatile unsigned int frame_count = 0;

IWRAM_DATA static std::array<Task, 40> tasks_ = {};
IWRAM_DATA static volatile int task_count = 0;
IWRAM_DATA static std::unordered_set<uint16_t> task_ids;

IWRAM_DATA static std::array<Task, 40> vblank_tasks_ = {};
IWRAM_DATA static volatile int vblank_task_count = 0;
IWRAM_DATA static std::unordered_set<uint16_t> vblank_task_ids;

constexpr unsigned int cycles_per_v_blank = 279238;
// nanosecond precision helps us avoid conversions between units later
EWRAM_DATA static const std::chrono::microseconds vblank_interval = std::chrono::microseconds(16743);
EWRAM_DATA static std::chrono::microseconds frame_duration = vblank_interval;
IWRAM_DATA static GbaClock& osclock = GbaClock::instance();

IWRAM_DATA static bool alert_called = false;

void set_framerate(video::FRAMERATE framerate){
    framerate_ = framerate;

    switch(framerate_){
        case video::FRAMERATE60:
            frame_duration = vblank_interval;
            break;
        case video::FRAMERATE30:
            frame_duration = vblank_interval * 2;
            break;
        case video::FRAMERATE15:
            frame_duration = vblank_interval * 4;
            break;
    }
}

IWRAM_CODE static void vblank_os_callback(){
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

    for(int i = 0; i < vblank_task_count; i++){
        vblank_tasks_[i].f(vblank_tasks_[i]);
    }
}

IWRAM_CODE void waitNextFrame(){
    switch (framerate_)
    {
    case video::FRAMERATE60:
        VBlankIntrWait();
        break;
    default:
        while(frame_count == 0){};
        while(frame_count != 0){};
        break;
    }
}

void init_gba_os(){
    InterruptManager::instance();
    irqEnable(IRQ_VBLANK);
    irqEnable(IRQ_KEYPAD);
	irqSet(IRQ_VBLANK, &vblank_os_callback);
}

static std::string errmsg = "Frame counter not 0 at start of frame!";
IWRAM_CODE ARM_CODE void tick(){
    auto start = osclock.now();

    if(frame_count != 0)
        Gba_os::error::error_state(Gba_os::error::SFOTWARE_RUNTIME_ERROR, static_cast<void*>(&errmsg));

    for(int i = 0; i < task_count; i++)
        tasks_[i].f(tasks_[i]);

    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(osclock.now() - start);
    if (delta > frame_duration && !alert_called)
        Gba_os::error::error_state(Gba_os::error::FRAME_DURATION_EXCEEDED, static_cast<void*>(&delta));
    waitNextFrame();
}

int register_task(Task t){
    static uint32_t task_id_counter = 0;
    while(task_ids.contains(task_id_counter)){ task_id_counter = (task_id_counter + 1) % UINT16_MAX; }
    t.id = task_id_counter;
    task_ids.insert(task_id_counter);
    task_id_counter++;
    tasks_[task_count] = t;
    task_count++;
    std::sort(
        tasks_.begin(),
        tasks_.begin() + task_count,
        [](Task a, Task b){
            return (a.priority < b.priority);
        }
    );
    return t.id;
}

void unregister_task(int id){
    task_ids.erase(id);
    auto it = std::find_if(tasks_.begin(), tasks_.begin() + task_count, [&id](Task t){return t.id == id;});
    if(it != tasks_.begin() + task_count){
        std::rotate(it, it + 1, tasks_.begin() + task_count);
        task_count--;
    }
}

int register_vblank_interrupt_task(Task cb){
    static uint32_t task_id_counter = 0;
    while(task_ids.contains(task_id_counter)){ task_id_counter = (task_id_counter + 1) % UINT16_MAX; }
    cb.id = task_id_counter;
    vblank_task_ids.insert(task_id_counter);
    task_id_counter++;
    vblank_tasks_[vblank_task_count] = cb;
    vblank_task_count++;
    std::sort(
        vblank_tasks_.begin(),
        vblank_tasks_.begin() + vblank_task_count,
        [](Task a, Task b){
            return (a.priority < b.priority);
        }
    );
    return cb.id;
}

void unregister_vblank_interrupt_task(int id){
    vblank_task_ids.erase(id);
    auto it = std::find_if(vblank_tasks_.begin(), vblank_tasks_.begin() + vblank_task_count, [&id](Task t){return t.id == id;});
    if(it != vblank_tasks_.begin() + vblank_task_count){
        std::rotate(it, it + 1, vblank_tasks_.begin() + vblank_task_count);
        vblank_task_count--;
    }
}

void alert(std::string alert_message){
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
	Gba_os::console::clearConsole();

	printf(alert_message.c_str());
    while(
        !(keysDown() & KEY_A)
    ){
		scanKeys();
    };
    alert_called = true;
    Gba_os::screen::restore_user_mode();
}

}
