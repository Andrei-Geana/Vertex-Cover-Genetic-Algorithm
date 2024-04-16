#pragma once
#include <iostream>
class Arch
{
public:
    Arch(int x, int y)
    {
        FromToNodes = std::make_pair(x, y);
    }

    int From() const
    {
        return FromToNodes.first;
    }

    int To() const
    {
        return FromToNodes.second;
    }

    bool operator ==(const Arch& newArch)
    {
        return (FromToNodes.first == newArch.FromToNodes.first && FromToNodes.second == newArch.FromToNodes.second)
            || (FromToNodes.first == newArch.FromToNodes.second && FromToNodes.second == newArch.FromToNodes.first);
    }

    class IsEqual
    {
    public:
        bool operator()(const Arch& a, const Arch& b) const
        {
            return (a.From() == b.From() && a.To() == b.To()) || (a.From() == b.To() && a.To() == b.From());
        }
    };

    class Hash
    {
    public:
        size_t operator()(const Arch& p) const
        {
            auto h1 = std::hash<size_t>()(p.FromToNodes.first);
            auto h2 = std::hash<size_t>()(p.FromToNodes.second);
            return h1 ^ h2;
        }
    };

private:
    std::pair<int, int> FromToNodes;
};
