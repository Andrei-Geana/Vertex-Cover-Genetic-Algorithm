#include "Individual.h"

Individual::Individual() {/*EMPTY*/ }

Individual::Individual(const std::vector<bool>& chromosomes)
{
    this->genes = chromosomes;
    score = 0;
}

Individual::Individual(Individual* i)
{
    genes = i->genes;
    score = i->score;
}

void Individual::SetScore(double score)
{
    this->score = score;
}

double Individual::GetScore() const
{
    return score;
}

int Individual::GetNumberOf1s() const
{
    int count{ 0 };
    for (const auto& chromosome : genes)
    {
        if (chromosome)
            count++;
    }
    return count;
}

size_t Individual::GetNumberOfChromosomes() const
{
    return genes.size();
}

void Individual::Mutate()
{
    for (size_t index{ 0u }; index < genes.size(); ++index)
    {
        if (GetChance() < MutationRate)
        {
            genes[index] = !genes[index];
        }
    }
}

double Individual::GetChance(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(min, max);
    return distrib(gen);
}

std::ostream& operator<<(std::ostream& out, const Individual& individ)
{
    for (const auto& chromosome : individ.genes)
    {
        out << chromosome << ' ';
    }
    return out;
}
