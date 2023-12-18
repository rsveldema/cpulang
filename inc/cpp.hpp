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
    T value() const { return w.value(); }

private:
    wire<T>& w;
};



template <typename T>
class out_buffer
{
public:
    void push(const T& v) { w.push(v); }

private:
    wire<T>& w;
};




template <typename T>
class inout_buffer
{
public:
    void push(const T& v) {w.push(v); }

    T value() const { return w.value(); }

private:
    wire<T>& w;
};



