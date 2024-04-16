#include "Graph.h"
//vertex cover
/*
* -salvez muchiile intr-un unordered_set care va contine perechile de noduri intre care exista muchie (x1,x2)
* -verificare solutie: pt fiecare muchie verific daca cel putin un nod face parte din solutie. daca nu, nu e solutie
* -
*/

//selectia => turneu
/*
* -am o functie care returneaza un grup RANDOM din turneu de dimensiune TourneySize
* -am o functie care selecteaza doar castigatorul unei grupe din turneu
* -selectez PopulationSize indivizi
* -fac incrucisare 1 cu 2, 3 cu 4, etc
* -incerc sa fac mutatie pe copii
* -copiii lui x1 cu x2 ii inlocuiesc pe acestia
*/

static int GetRandomIndex(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

class VertexCoverGeneticAlgorithm
{
public:

    VertexCoverGeneticAlgorithm(Graph base)
    {
        baseGraph = base;
    }

    /*bool IsSolution(const std::vector<bool>& solutionToBeChecked)
    {
        return baseGraph.IsSolution(solutionToBeChecked);
    }*/

    double GetFitnessScore(const Individual& individual)
    {
        return individual.GetNumberOf1s() + LambdaFitness * baseGraph.GetNumberOfNotVerifiedArch(individual);
    }

    void RunAlgorithm(std::vector<std::vector<bool>>& population)
    {
        size_t indexOfIteration{ 0u };
        while (indexOfIteration < NumberOfIterations)
        {
            indexOfIteration++;
            MakeOneIteration(population);
        }
        //return best person
    }

private:
    void MakeOneIteration(std::vector<std::vector<bool>>& population)
    {
        std::vector<std::vector<std::vector<bool>>> groups;
        while (groups.size() != (PopulationSize / TourneySize))
        {
            groups.emplace_back(GetGroup(population));
        }
        std::vector<std::vector<bool>> winners;
        for (const auto& group : groups)
        {
            winners.emplace_back(GetBestPersonInGroup(group));
        }
        std::vector<std::vector<bool>> newPopulation;
        for (size_t index{ 0u }; index < winners.size() - 1; index += 2u)
        {
            ;
        }
    }

    std::vector<std::vector<bool>> GetGroup(std::vector<std::vector<bool>>& population)
    {
        std::vector<std::vector<bool>> group;
        while (group.size() < TourneySize)
        {
            auto person = GetRandomPerson(population);
            //daca nu e deja aici
            group.emplace_back(person);
        }
        return group;
    }
    
    std::vector<bool> GetRandomPerson(std::vector<std::vector<bool>>& population)
    {
        int index{ GetRandomIndex(0, (int)population.size() - 1) };
        return population[index];
    }

    std::vector<bool> GetBestPersonInGroup(const std::vector<std::vector<bool>>& group)
    {
        auto person = group[0];
        for (size_t index{ 1 }; index < group.size(); ++index)
        {
            if (GetFitnessScore(group[index]) > GetFitnessScore(person))
                person = group[index];
        }
        return person;
    }

private:
    Graph baseGraph;

    std::unordered_set<Individual, Individual::Hash, Individual::IsEqual> population;
};

static bool ShouldChange()
{
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 2);
    return distrib(gen) % 2;
}

static std::vector<bool> GetPerson(int n = NodesNumber)
{
    std::vector<bool> guy(n, false);
    for (size_t i{ 0u }; i < NodesNumber; ++i)
    {
        if (ShouldChange())
            guy[i] = !guy[i];
    }
    return guy;
}

static std::vector<std::vector<bool>> GetPopulation(int numberOfPeople = PopulationSize)
{
    std::vector<std::vector<bool>> population;
    while(population.size() < numberOfPeople)
    {
        auto person = GetPerson();
        //Sa elimin indivizii care se repeta?
        population.emplace_back(person);
    }

    return population;
}

int main()
{
    Graph a{ 5 };
    try {
        a.AddArch(1, 2);
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }
    VertexCoverGeneticAlgorithm algorithm{ a };
    auto population = GetPopulation();
    try {
        std::cout << std::boolalpha << a.IsSolution(std::vector<bool>{true,true,false,false,false});
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }
    return 0;
}