#include <iostream>
#include <unordered_set>
#include <random>
//vertex cover
/*
* -salvez muchiile intr-un unordered_set care va contine perechile de noduri intre care exista muchie (x1,x2)
* -verificare solutie: pt fiecare muchie verific daca cel putin un nod face parte din solutie. daca nu, nu e solutie
* -
*/

const int NodesNumber = 5;
const int PopulationSize = 100;
const int NumberOfIterations = 50;
const double LambdaFitness = 0.5;
const int TourneySize = 2;

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

class VertexCover
{
public:
    VertexCover(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            nodes.insert(i);
        }
    }

    void AddArch(int from, int to)
    {
        arches.insert(Arch{ from, to });
    }

    bool IsSolution(const std::vector<bool>& solutionToBeChecked)
    {
        if (solutionToBeChecked.size() != nodes.size())
            throw std::exception("vector size not equal to number of nodes in graph");

        for (const Arch& arc : arches)
        {
            if (solutionToBeChecked[arc.From()])
                continue;
            if (solutionToBeChecked[arc.To()])
                continue;
            return false;
        }

        return true;
    }

    void PrintAllArches()
    {
        for (const Arch& elem : arches)
        {
            std::cout << elem.From() << "," << elem.To() << std::endl;
        }
    }

    double GetFitnessScore(const std::vector<bool>& solution)
    {
        return GetNumberOf1s(solution) + LambdaFitness * GetNumberOfNotVerifiedArch(solution);
    }

private:
    static int GetNumberOf1s(const std::vector<bool>& solution)
    {
        int count{ 0 };
        for (const auto& chromosome : solution)
        {
            if (chromosome)
                count++;
        }
        return count;
    }

    int GetNumberOfNotVerifiedArch(const std::vector<bool>& solution)
    {
        int count{ 0 };
        for (const Arch& arc : arches)
        {
            if (solution[arc.From()])
                continue;
            if (solution[arc.To()])
                continue;
            count++;
        }
        return count;
    }

private:
    std::unordered_set<int> nodes;
    std::unordered_set<Arch, Arch::Hash, Arch::IsEqual> arches;
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
    VertexCover a(5);
    a.AddArch(1, 2);
    a.PrintAllArches();
    auto population = GetPopulation();
    try {
        std::cout << std::boolalpha << a.IsSolution(std::vector<bool>{false,true,true,false,false});
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }
    return 0;
}