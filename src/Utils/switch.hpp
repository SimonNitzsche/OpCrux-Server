#include <unordered_map>
#include <functional>


template<typename Key>

class Switcher {
public:
    typedef std::function<void()> Func;
    Switcher(Key key) : m_impl(), m_default(), m_key(key) {}
    Switcher& Case(Key key, Func func) {
        m_impl.insert(std::make_pair(key, func));
        return *this;
    }

    Switcher& Default(Func func) {
        m_default = func;
        return *this;
    }

    ~Switcher() {
        auto iFunc = m_impl.find(m_key);
        if (iFunc != m_impl.end())
            iFunc->second();
        else
            m_default();
    }

private:
    std::unordered_map<Key, Func> m_impl;
    Func m_default;
    Key m_key;
};

template<typename Key>

Switcher<Key> Switch(Key key)
{
    return Switcher<Key>(key);
}