#pragma once

#include <stdint.h>
#include <assert.h>

#define active_component struct
#define passive_component struct
#define entity struct
#define cpu struct


#define enum enum class


template<typename T>
struct optional{
    bool available;
    T m_value;

    optional() { available = false; }
    optional(T v) { available = true; m_value = v; }
};



template<typename T>
class wire
{
public:
    T value() const { return m_v; }
    void push(T v) { m_v = v; }
private:
    T m_v;
};


template <typename T>
class in_buffer
{
public:
    in_buffer(wire<T>& w) : m_w(w) {}

    T value() const { return m_w.value(); }

private:
    wire<T>& m_w;
};



template <typename T>
class out_buffer
{
public:
    out_buffer(wire<T>& w) : m_w(w) {}

    void push(const T& v) { m_w.push(v); }

private:
    wire<T>& m_w;
};




template <typename T>
class inout_buffer
{
public:
    inout_buffer(wire<T>& w) : m_w(w) {}

    void push(const T& v) { m_w.push(v); }

    T value() const { return m_w.value(); }

private:
    wire<T>& m_w;
};



