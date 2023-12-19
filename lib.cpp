#include "examples/cpu/cpu_container.cpu"

std::vector<ActiveComponent *> ActiveComponent::s_components;
std::vector<AbstractWire *> AbstractWire::s_wires;

void tick_all_active_components()
{
    for (auto *c : ActiveComponent::s_components)
    {
        c->tick();
    }

    for (auto *c : AbstractWire::s_wires)
    {
        c->tick();
    }
}