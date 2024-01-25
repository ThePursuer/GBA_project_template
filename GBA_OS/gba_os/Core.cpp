#include "gba_os/Core.h"
#include "OsErrors.h"
#include "stdio.h"


namespace gba_os{

IWRAM_DATA static video::FRAMERATE framerate_ = video::FRAMERATE60;
IWRAM_DATA static volatile unsigned int frame_count = 0;
IWRAM_DATA static std::vector<Task> tasks_ = {};
IWRAM_DATA static std::unordered_set<uint16_t> task_ids;
IWRAM_DATA static std::vector<Task> vblank_tasks_ = {};
IWRAM_DATA static std::unordered_set<uint16_t> vblank_task_ids;
IWRAM_DATA static chrono::gba_milliseconds frame_duration = chrono::gba_milliseconds(1000 / 60);

static constexpr void sort_tasks(std::vector<Task>& t_){
    std::sort(
        t_.begin(),
        t_.end(),
        [](Task a, Task b){ 
            return (a.priority < b.priority);
        }
    );
}

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

    for(auto task : vblank_tasks_)
        task.f(task);
}

IWRAM_CODE void waitNextFrame(){
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

IWRAM_CODE void run_gba_os(){
    auto clock = chrono::GbaClock::instance();
    chrono::GbaClock::time_point start;
    chrono::gba_milliseconds delta;
    waitNextFrame();
    while(1){
        start = clock.now();
        for(auto task: tasks_){
            task.f(task);
        }
        delta = std::chrono::duration_cast<chrono::gba_milliseconds>(clock.now() - start);
        if (delta > frame_duration)
            gba_os::error::error_state(gba_os::error::FRAME_DURATION_EXCEEDED, static_cast<void*>(&delta));
        waitNextFrame();
    }
}

int register_task(Task t){
    static uint32_t task_id_counter = 0;
    while(task_ids.contains(task_id_counter)){ task_id_counter = (task_id_counter + 1) % UINT16_MAX; }
    t.id = task_id_counter;
    task_ids.insert(task_id_counter);
    task_id_counter++;
    tasks_.push_back(t);
    sort_tasks(tasks_);
    return t.id;
}

void unregister_task(int id){
    task_ids.erase(id);
    std::erase_if(tasks_, [&id](Task t){return t.id == id;});
}

int register_vblank_interrupt_task(Task cb){
    static uint32_t task_id = 0;
    while(task_ids.contains(task_id)){ task_id = (task_id + 1) % UINT16_MAX; }
    cb.id = task_id;
    task_ids.insert(task_id);
    task_id++;
    vblank_tasks_.push_back(cb);
    sort_tasks(vblank_tasks_);
    return cb.id;
}

void unregister_vblank_interrupt_task(int id){
    std::erase_if(vblank_tasks_, [&id](Task cb){return cb.id == id;});
}

}
