#pragma once
#include <iostream>
#include <unordered_set>

#include "Arch.h"
#include "Individual.h"

class Graph
{
public:
    Graph();
    Graph(int n);

    void AddArch(int from, int to);

    bool IsSolution(const Individual& individ) const;

    int GetNumberOfNotVerifiedArch(const Individual& individ) const;

    void PrintAllArches();

    int GetNumberOfArches() const;

private:
    std::unordered_set<int> nodes;
    std::unordered_set<Arch, Arch::Hash, Arch::IsEqual> arches;

};
