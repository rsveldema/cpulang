#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <vector>

struct ActiveComponent
{
  ActiveComponent() { s_components.push_back(this); }

  virtual void tick() = 0;

  static std::vector<ActiveComponent *> s_components;
};

/** call the tick methods for all active components
 */
void tick_all_active_components();

#define ACTIVE_COMPONENT(NAME) struct NAME : public ActiveComponent
#define PASSIVE_COMPONENT(NAME) struct NAME

#define enum enum class

template <typename T> struct optional
{
  bool available;
  T    m_value;

  optional() { available = false; }
  optional(T v)
  {
    available = true;
    m_value   = v;
  }
};

class AbstractWire
{
public:
  virtual void tick() = 0;

  AbstractWire()
  {
    s_wires.push_back(this);
  }

static std::vector<AbstractWire *> s_wires;
};

template <typename T> class Wire : AbstractWire
{
public:
  Wire() {}

  T value() { return m_current_value; }

  void push(T v) { m_new_value = v; }

  void tick() override
  {
    if (m_counter == 0)
      {
        m_current_value = m_new_value;
        m_counter       = m_delay;
      }
    else
      {
        m_counter--;
      }
  }

  void test_set_delay(uint32_t delay) { m_delay = m_counter = delay; }

private:
  uint32_t m_delay   = 0;
  uint32_t m_counter = 0;

  T m_new_value;
  T m_current_value;
};

template <typename T> class in_buffer
{
public:
  in_buffer(Wire<T> &w) : m_w(w) {}

  T value() const { return m_w.value(); }

private:
  Wire<T> &m_w;
};

template <typename T> class out_buffer
{
public:
  out_buffer(Wire<T> &w) : m_w(w) {}

  void push(const T &v) { m_w.push(v); }

private:
  Wire<T> &m_w;
};

template <typename T> class inout_buffer
{
public:
  inout_buffer(Wire<T> &w) : m_w(w) {}

  void push(const T &v) { m_w.push(v); }

  T value() const { return m_w.value(); }

private:
  Wire<T> &m_w;
};
