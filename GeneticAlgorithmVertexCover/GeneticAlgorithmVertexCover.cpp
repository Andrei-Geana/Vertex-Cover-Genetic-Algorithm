#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
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

const int NodesNumber = 5;
const int PopulationSize = 100;
const int NumberOfIterations = 50;
const double LambdaFitness = 0.1;
const int TourneySize = 2;
const int PopulationAfterSelection = PopulationSize;
const double MutationRate = 0.001;


static int GetRandomIndex(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

static double GetMutationChance(double min = 0.0005, double max = 0.1)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(min, max);
    return distrib(gen);
}


class Individual
{
public:
    Individual(const std::vector<bool>& chromosomes)
    {
        this->genes = chromosomes;
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
        for (size_t index{ 0u }; index<genes.size(); ++index)
        {
            if (GetMutationChance() < MutationRate)
                genes[index] = !genes[index];
        }
    }

private:
    std::vector<bool> genes;
};


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

    bool IsSolution(const Individual& individ)
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

    int GetNumberOfNotVerifiedArch(const Individual& individ)
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

private:
    std::unordered_set<int> nodes;
    std::unordered_set<Arch, Arch::Hash, Arch::IsEqual> arches;

};

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
    Graph a(5);
    try {
        a.AddArch(1, 2);
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }
    VertexCoverGeneticAlgorithm solution(a);
    auto population = GetPopulation();
    try {
        std::cout << std::boolalpha << a.IsSolution(std::vector<bool>{true,true,false,false,false});
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }
    return 0;
}