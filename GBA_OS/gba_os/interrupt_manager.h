#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#include <gba_interrupt.h>

/**
 * @brief A singleton class that ensures that irqInit is called only once for other static objects. 
 * @example 
 * class MyClass {
 * public:
 *   Myclass& instance(){
 *     static MyClass my_class;
 *     return my_class;
 *   }
 * private:
 *   MyClass(){
 *     InterruptManager::instance(); // irqInit is called here and only once
 *     irqEnable(IRQ_VBLANK);
 *     irqSet(IRQ_VBLANK, &vblank_irq_handler);
 *  }
 * };
*/
class InterruptManager {
public:
    static InterruptManager& instance(){
        static InterruptManager manager;
        return manager;
    }
private:
    InterruptManager(){
        irqInit();
    }
};

#endif // INTERRUPT_MANAGER_H