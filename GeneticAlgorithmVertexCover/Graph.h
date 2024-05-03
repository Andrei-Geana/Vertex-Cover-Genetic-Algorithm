#pragma once
#include <iostream>
#include <unordered_set>
#include <fstream>

#include "AlgorithmData.h"
#include "Arch.h"

class Graph
{
public:
    Graph();
    Graph(int n);

    void AddArch(int from, int to);

    bool IsSolution(const std::vector<bool>& individ) const;

    int GetNumberOfNotVerifiedArch(const std::vector<bool>& individ) const;

    void PrintAllArches();

    int GetNumberOfArches() const;

public:
    static Graph* ReadGraphFromFile(const std::string& FilePath = AlgorithmData::FilePathToGraph);

private:
    std::unordered_set<int> nodes;
    std::unordered_set<Arch, Arch::Hash, Arch::IsEqual> arches;

};
