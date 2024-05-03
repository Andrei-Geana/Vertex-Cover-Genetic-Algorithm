#include "Individual.h"

Individual::Individual() {/*EMPTY*/ }

Individual::Individual(const std::vector<bool>& chromosomes)
{
    this->genes = chromosomes;
    graph = nullptr;
    score = 0;
}

Individual::Individual(const std::vector<bool>& chromosomes, Graph* graph)
{
    this->genes = chromosomes;
    this->graph = graph;
    UpdateScore();
}

Individual::Individual(Individual* i)
{
    graph = i->graph;
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

void Individual::SetGraph(Graph* graph)
{
    if (!graph)
        throw std::exception{ "not graph provided" };
    this->graph = graph;
    UpdateScore();
}

Graph* Individual::GetGraph() const
{
    return graph;
}

void Individual::UpdateScore()
{
    score = ((int)GetNumberOfChromosomes() - GetNumberOf1s());
    score += (graph->GetNumberOfArches() - graph->GetNumberOfNotVerifiedArch(genes));
    score += graph->IsSolution(genes) * AlgorithmData::IsSolutionPoints;
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
        if (Helper::GetChance() < AlgorithmData::MutationRate)
        {
            genes[index] = !genes[index];
            UpdateScore();
        }
    }
}

Individual Individual::GetPerson(const int& n)
{
    std::vector<bool> genes(n, false);
    for (size_t i{ 0u }; i < AlgorithmData::NodesNumber; ++i)
    {
        if (auto possibility = Helper::ShouldChange(); possibility)
            genes[i] = !genes[i];
    }
    return Individual{ genes };
}

std::ostream& operator<<(std::ostream& out, const Individual& individ)
{
    for (const auto& chromosome : individ.genes)
    {
        out << chromosome << ' ';
    }
    return out;
}
