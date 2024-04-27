#pragma once
#include <iostream>
#include <vector>
#include <random>

#include "AlgorithmData.h"

class Individual
{
public:
    Individual() {/*EMPTY*/ }

    Individual(const std::vector<bool>& chromosomes)
    {
        this->genes = chromosomes;
        score = 0;
    }

    Individual(Individual* i)
    {
        genes = i->genes;
        score = i->score;
    }

    void SetScore(double score)
    {
        this->score = score;
    }

    double GetScore() const
    {
        return score;
    }

    int GetNumberOf1s() const
    {
        int count{ 0 };
        for (const auto& chromosome : genes)
        {
            if (chromosome)
                count++;
        }
        return count;
    }

    size_t GetNumberOfChromosomes() const
    {
        return genes.size();
    }

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

    void Mutate()
    {
        for (size_t index{ 0u }; index < genes.size(); ++index)
        {
            if (GetChance() < MutationRate)
            {
                genes[index] = !genes[index];
            }
        }
    }

public:

    friend std::ostream& operator<<(std::ostream& out, const Individual& individ) 
    {
        for (const auto& chromosome : individ.genes)
        {
            out << chromosome << ' ';
        }
        return out;
    }

    static double GetChance(double min = 0, double max = 1)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distrib(min, max);
        return distrib(gen);
    }

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
};

