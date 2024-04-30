#pragma once
#include <iostream>
class Arch
{
public:
    Arch(int x, int y);

    int From() const;

    int To() const;

    bool operator ==(const Arch& newArch);

public:
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
