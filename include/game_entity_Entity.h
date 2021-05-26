#pragma once

namespace sym::game::entity
{

class Entity
{
public:
    virtual void FreeGraphicResources() = 0;
    virtual void AllocateGraphicResources() = 0;
};

} // namespace sym::game::entity
