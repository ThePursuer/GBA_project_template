#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>
#include <vector>
#include <memory>

template<typename... Args>
class Signal;

template<typename... Args>
class SlotBase {
public:
    virtual ~SlotBase() = default;
    virtual void call(Args... args) = 0;
};

template<typename T, typename... Args>
class Slot : public SlotBase<Args...> {
public:
    Slot(T* object, void (T::*function)(Args...))
        : m_object(object), m_function(function) {}

    void call(Args... args) override {
        (m_object->*m_function)(args...);
    }

private:
    T* m_object;
    void (T::*m_function)(Args...);
};

template<typename... Args>
class Connection {
public:
    Connection(std::shared_ptr<SlotBase<Args...>> slot)
        : m_slot(slot) {}

    bool isConnected() const {
        return static_cast<bool>(m_slot);
    }

    void disconnect() {
        m_slot.reset();
    }

    void call(Args... args) const {
        if (m_slot) {
            m_slot->call(args...);
        }
    }

private:
    std::shared_ptr<SlotBase<Args...>> m_slot;
};

template<typename... Args>
class Signal {
public:
    using Slot = std::shared_ptr<SlotBase<Args...>>;

    void connect(Slot slot) {
        m_slots.push_back(slot);
    }

    void disconnect(Slot slot) {
        auto it = std::find(m_slots.begin(), m_slots.end(), slot);
        if (it != m_slots.end()) {
            m_slots.erase(it);
        }
    }

    void operator()(Args... args) const {
        for (const auto& slot : m_slots) 
            slot->call(args...);
    }

private:
    std::vector<Slot> m_slots;
};

template<typename T, typename... Args>
Slot<T, Args...> make_slot(T* object, void (T::*function)(Args...)) {
    return Slot<T, Args...>(object, function);
}

#define CONNECT_SIGNAL(signal, object, function) \
    signal.connect(std::make_shared<decltype(make_slot(object, function))>(make_slot(object, function)))

#define DISCONNECT_SIGNAL(signal, object, function) \
    signal.disconnect(std::make_shared<decltype(make_slot(object, function))>(make_slot(object, function)))

#endif // SIGNAL_H