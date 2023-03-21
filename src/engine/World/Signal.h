#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>
#include <vector>

// Define a template class for a signal that can be emitted with any number of arguments
template<typename... Args>
class Signal
{
public:
    // Define a type alias for the signal handler
    using Slot = std::function<void(Args...)>;

    // Connect a slot to the signal
    void connect(Slot slot)
    {
        m_slots.push_back(slot);
    }

    // Emit the signal with the given arguments
    void emit(Args... args)
    {
        for (auto& slot : m_slots)
        {
            slot(args...);
        }
    }

private:
    std::vector<Slot> m_slots;
};

// Define a template class for a slot that can receive any number of arguments
template<typename... Args>
class Slot
{
public:
    // Define a type alias for the signal that this slot is connected to
    using SignalType = Signal<Args...>;

    // Connect this slot to the given signal
    void connect(SignalType& signal)
    {
        m_connection = signal.connect([this](Args... args) {
            this->operator()(args...);
        });
    }

    // Disconnect this slot from the signal
    void disconnect()
    {
        m_connection.disconnect();
    }

    // Override the function call operator to execute the slot's code when it is invoked
    virtual void operator()(Args... args) = 0;

private:
    typename SignalType::Connection m_connection;
};

// Define a macro to make it easier to connect slots to signals
#define CONNECT_SIGNAL(signal, slot) (slot).connect(signal)

// Define a macro to make it easier to disconnect slots from signals
#define DISCONNECT_SIGNAL(slot) (slot).disconnect()

#endif // SIGNAL_H