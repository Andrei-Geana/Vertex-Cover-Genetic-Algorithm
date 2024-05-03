#pragma once
#include <iostream>
#include <vector>
#include <random>

#include "Graph.h"
#include "Helper.h"

class Individual
{
public:
    Individual();

    Individual(const std::vector<bool>& chromosomes);

    Individual(const std::vector<bool>& chromosomes, Graph* graph);

    Individual(Individual* i);

    void SetScore(double score);

    double GetScore() const;

    void SetGraph(Graph* graph);

    Graph* GetGraph() const;

    void UpdateScore();

    int GetNumberOf1s() const;

    size_t GetNumberOfChromosomes() const;

    std::vector<bool>::reference operator[](size_t index)
    {
        if (index >= genes.size())
        {
            throw std::out_of_range("Index out of range");
        }
        return genes[index];
    }

    std::vector<bool>::const_reference operator[](size_t index) const
    {
        if (index >= genes.size())
        {
            throw std::out_of_range("Index out of range");
        }
        return genes[index];
    }

    void Mutate();

public:

    friend std::ostream& operator<<(std::ostream& out, const Individual& individ);

    static Individual GetPerson(const int& n = AlgorithmData::NodesNumber);

public:

    class IsEqual
    {
    public:
        bool operator()(const Individual& a, const Individual& b) const
        {
            return a.genes == b.genes;
        }
    };

    class Hash
    {
    public:
        size_t operator()(const Individual& p) const
        {
            std::hash<bool> hasher;
            size_t answer = 0;
            for (const bool& i : p.genes)
            {
                answer ^= hasher(i) + 0x9e3779b9 +
                    (answer << 6) + (answer >> 2);
            }
            return answer;
        }
    };

private:
    std::vector<bool> genes;
    double score;
    Graph* graph;
};

