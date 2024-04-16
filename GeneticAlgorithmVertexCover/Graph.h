#pragma once
#include <iostream>
#include <unordered_set>

#include "Arch.h"
#include "Individual.h"

class Graph
{
public:
    Graph() {/*EMPTY*/ }
    Graph(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            nodes.insert(i);
        }
    }

    void AddArch(int from, int to)
    {
        if (!nodes.contains(from))
            throw new std::exception("start node not in graph");
        if (!nodes.contains(to))
            throw new std::exception("destination node not in graph");

        arches.insert(Arch{ from, to });
    }

    bool IsSolution(const Individual& individ) const
    {
        if (individ.GetNumberOfChromosomes() != nodes.size())
            throw std::exception("vector size not equal to number of nodes in graph");

        for (const Arch& arc : arches)
        {
            if (individ[arc.From()])
                continue;
            if (individ[arc.To()])
                continue;
            return false;
        }

        return true;
    }

    int GetNumberOfNotVerifiedArch(const Individual& individ) const
    {
        int count{ 0 };
        for (const Arch& arc : arches)
        {
            if (individ[arc.From()])
                continue;
            if (individ[arc.To()])
                continue;
            count++;
        }
        return count;
    }

    void PrintAllArches()
    {
        for (const Arch& elem : arches)
        {
            std::cout << elem.From() << "," << elem.To() << std::endl;
        }
    }

    int GetNumberOfArches() const
    {
        return (int)arches.size();
    }

private:
    std::unordered_set<int> nodes;
    std::unordered_set<Arch, Arch::Hash, Arch::IsEqual> arches;

};
