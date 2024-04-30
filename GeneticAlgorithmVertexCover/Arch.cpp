#include "Arch.h"

Arch::Arch(int x, int y)
{
    FromToNodes = std::make_pair(x, y);
}

int Arch::From() const
{
    return FromToNodes.first;
}

int Arch::To() const
{
    return FromToNodes.second;
}

bool Arch::operator==(const Arch& newArch)
{
    return (FromToNodes.first == newArch.FromToNodes.first && FromToNodes.second == newArch.FromToNodes.second)
        || (FromToNodes.first == newArch.FromToNodes.second && FromToNodes.second == newArch.FromToNodes.first);
}