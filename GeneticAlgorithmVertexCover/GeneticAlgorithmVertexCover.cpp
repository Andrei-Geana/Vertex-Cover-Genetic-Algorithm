#include "Graph.h"
#include "Individual.h"
#include <fstream>
#include <map>
#include <chrono>
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

static bool ShouldChange()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 10);
    return distrib(gen) % 2;
}

static Individual GetPerson(int n = NodesNumber)
{
    std::vector<bool> genes(n, false);
    for (size_t i{ 0u }; i < NodesNumber; ++i)
    {
        if (auto possibility = ShouldChange(); possibility)
            genes[i] = !genes[i];
    }
    return Individual{ genes };
}


//typedef  std::unordered_multiset<Individual, Individual::Hash, Individual::IsEqual> ListOfIndividuals;
typedef std::vector<Individual> ListOfIndividuals;


class VertexCoverGeneticAlgorithm
{
public:

    VertexCoverGeneticAlgorithm(const Graph& base, ListOfIndividuals&& population)
    {
        baseGraph = base;
        this->population = std::move(population);
    }

    VertexCoverGeneticAlgorithm(const Graph& base)
    {
        baseGraph = base;
    }

    void SetPopulation(ListOfIndividuals&& population)
    {
        this->population = std::move(population);
    }

    //basic GetPopulation
    static ListOfIndividuals GetPopulation(int numberOfPeople = PopulationSize)
    {
        ListOfIndividuals population;
        while (population.size() < numberOfPeople)
        {
            population.emplace_back(GetPerson());

            //Debugging
            
            /*for (size_t index{ 0u }; index < person.GetNumberOfChromosomes(); ++index)
                std::cout << std::boolalpha << person[index] << " ";
            std::cout << population.size() << std::endl;*/
            
        }

        return population;
    }

    void ShowPopulation() const
    {
        for (const auto& individual : population)
                std::cout << individual << " SCORE: " << GetFitnessScore(individual) << std::endl;
    }

    //best fitness = smallest (min)
    double GetFitnessScore(const Individual& individual) const
    {
        return individual.GetNumberOf1s() + LambdaFitness * baseGraph.GetNumberOfNotVerifiedArch(individual) + (!baseGraph.IsSolution(individual)) * NotSolutionPenalty;
    }

    Individual RunAlgorithm()
    {
        std::cout << "---------------------------------------------------------------" << std::endl <<"EPOCHS: "<< std::endl;
        //ShowPopulation();
        auto individual = GetBestPersonInGroup(population);
        std::cout << "\tI: Best Individual: " << individual << " Score: " << GetFitnessScore(individual) << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();

        size_t indexOfIteration{ 0u };
        while (indexOfIteration < NumberOfIterations)
        {
            MakeOneIteration();
            indexOfIteration++;
            //ShowPopulation();
            /*individual = GetBestPersonInGroup(population);
            std::cout << "\t";
            if (indexOfIteration == NumberOfIterations)
                std::cout << "E";
            else
                std::cout << indexOfIteration;
            std::cout << ": Best Individual: " << individual << " Score: " << GetFitnessScore(individual) << std::endl;*/
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        //ShowPopulation();
        individual = GetBestPersonInGroup(population);
        std::cout << "\t";
        std::cout << "E: Best Individual: " << individual << " Score: " << GetFitnessScore(individual) << std::endl;
        std::cout << "Time taken: " << duration << " milliseconds\n";
        return individual;
    }


private:
    void MakeOneIteration()
    {
        std::vector<ListOfIndividuals> groups;

        //Get tournaments
        while (groups.size() != PopulationSize)
        {
            groups.emplace_back(GetGroup());
        }

        //Put in new population winners of tournaments
        ListOfIndividuals newPopulation;
        for (const auto& group : groups)
        {
            newPopulation.emplace_back(GetBestPersonInGroup(group));
        }

        //DEBUGGING
        /*int index = 0;
        for (const auto& winner : newPopulation)
        {
            std::cout << index++ << ": " << GetFitnessScore(winner) << '\n';
        }*/
        

        //ListOfIndividuals => individuals have a bigger chance to crossover with an another individual which has the same score => converges faster
        //vector => standard crossover => bigger diversity
        std::vector<Individual*> selectedForCrossOver;

        //select those that will participate in crossover
        for (auto& individual : newPopulation)
        {
            if (Individual::GetChance() < CrossOverParticipationRate)
            {
                selectedForCrossOver.emplace_back(&individual);
            }
        }

        //get even number of individuals
        if (selectedForCrossOver.size() % 2 != 0)
        {
            selectedForCrossOver.pop_back();
        }

        //make crossover
        for (size_t index{ 0u } ; index<selectedForCrossOver.size()-1; index+=2u)
        {
            auto [c1, c2] = TwoPointCrossover(*selectedForCrossOver[index], *selectedForCrossOver[index + 1]);

            *selectedForCrossOver[index] = c1;
            *selectedForCrossOver[index + 1] = c2;
        }

        //mutation of new population
        for (auto& individual : newPopulation)
        {
            individual.Mutate();
        }

        population = newPopulation;
    }

    ListOfIndividuals GetGroup()
    {
        ListOfIndividuals group;
        while (group.size() < TourneySize)
        {
            group.emplace_back(GetRandomPerson());
        }
        return group;
    }
    
    Individual GetRandomPerson()
    {
        return population[GetRandomIndex(0, (int)population.size() - 1)];
    }

    Individual GetBestPersonInGroup(const ListOfIndividuals& group) const
    {
        return *(std::min_element(group.begin(), group.end(),
            [this](const Individual& ind1, const Individual& ind2) {
                return GetFitnessScore(ind1) < GetFitnessScore(ind2);
            }));
    }

    std::pair<Individual, Individual> OnPointCrossover(const Individual& parent1, const Individual& parent2) const
    {
        size_t point = (size_t)GetRandomIndex(0, (int)parent1.GetNumberOfChromosomes() - 1);
        std::vector<bool> chromosomesForFirstChild((int)parent1.GetNumberOfChromosomes());
        std::vector<bool> chromosomesForSecondChild((int)parent1.GetNumberOfChromosomes());

        size_t index{ 0u };
        for (; index < point; ++index)
        {
            chromosomesForFirstChild[index] = parent1[index];
            chromosomesForSecondChild[index] = parent2[index];
        }

        for (index = point; index < parent1.GetNumberOfChromosomes(); ++index)
        {
            chromosomesForFirstChild[index] = parent2[index];
            chromosomesForSecondChild[index] = parent1[index];
        }

        Individual child1{ chromosomesForFirstChild };
        Individual child2{ chromosomesForSecondChild };
        return std::make_pair(child1, child2);
    }

    std::pair<Individual, Individual> TwoPointCrossover(const Individual& parent1, const Individual& parent2) const
    {
        int numChromosomes{ (int)parent1.GetNumberOfChromosomes() };
        size_t point1{ (size_t)GetRandomIndex(0, numChromosomes - 1) };
        size_t point2{ (size_t)GetRandomIndex(0, numChromosomes - 1) };

        if (point1 > point2) 
        {
            std::swap(point1, point2);
        }

        std::vector<bool> chromosomesForFirstChild(numChromosomes);
        std::vector<bool> chromosomesForSecondChild(numChromosomes);

        for (size_t index{ 0u }; index < numChromosomes; ++index)
        {
            if (index >= point1 && index <= point2) 
            {
                chromosomesForFirstChild[index] = parent1[index];
                chromosomesForSecondChild[index] = parent2[index];
            }
            else 
            {
                chromosomesForFirstChild[index] = parent2[index];
                chromosomesForSecondChild[index] = parent1[index];
            }
        }

        Individual child1{ chromosomesForFirstChild };
        Individual child2{ chromosomesForSecondChild };
        return std::make_pair(child1, child2);
    }

    std::pair<Individual, Individual> UniformCrossover(const Individual& parent1, const Individual& parent2) const
    {
        std::vector<bool> chromosomesForFirstChild((int)parent1.GetNumberOfChromosomes());
        std::vector<bool> chromosomesForSecondChild((int)parent1.GetNumberOfChromosomes());

        for (size_t i = 0; i < parent1.GetNumberOfChromosomes(); ++i) 
        {
            if (Individual::GetChance() < 0.5) {
                chromosomesForFirstChild[i] = parent1[i];
                chromosomesForSecondChild[i] = parent2[i];
            }
            else {
                chromosomesForFirstChild[i] = parent2[i];
                chromosomesForSecondChild[i] = parent1[i];
            }
        }

        Individual child1{ chromosomesForFirstChild };
        Individual child2{ chromosomesForSecondChild };
        return std::make_pair(child1, child2);
    }

private:
    Graph baseGraph;
    ListOfIndividuals population;
};

Graph ReadGraphFromFile()
{
    std::ifstream fin{ FilePathToGraph };
    fin >> NodesNumber;
    Graph graph{ NodesNumber };
    int from{}, to{};
    while (fin >> from >> to)
    {
        try 
        {
            graph.AddArch(from, to);
        }
        catch (std::exception ex)
        {
            std::cout << ex.what() << " for arch: (" << from << ", " << to << ")" << std::endl;
        }
    }
    fin.close();
    return graph;

}

//score => numberOfIndividualsWithScore
void WriteResultsToFile(const std::map<double,int>& percentages, const std::map<double, std::unordered_set<Individual, Individual::Hash, Individual::IsEqual>>& individuals)
{
    std::ofstream out(FilePathToResults);
    if (!out)
        throw std::exception("Unable to open file.");

    int totalNumberOfIndividuals{ 0 };
    for (const auto& element : percentages)
    {
        totalNumberOfIndividuals += element.second;
    }

    for (const auto& element : percentages)
    {
        double percentage{ static_cast<double>(element.second) / totalNumberOfIndividuals * 100.0 };

        out << "SCORE: " << element.first << " =>" << "\tNUMBER OF INDIVIDUALS: " << element.second << "\tPERCENTAGE: " << percentage << "%" << std::endl;

        for (const auto& individual: individuals.at(element.first))
        {
            out << individual << std::endl;
        }
        out << std::endl;
    }

    out.close();

}

int main()
{
    Graph a{ ReadGraphFromFile()};
    VertexCoverGeneticAlgorithm algo{ a };

    ListOfIndividuals basePopulation{ VertexCoverGeneticAlgorithm::GetPopulation() };

    ListOfIndividuals pop;

    std::map<double, int> results;
    std::map<double, std::unordered_set<Individual, Individual::Hash, Individual::IsEqual>> individuals;
    Individual individual;
    for (size_t _{ 0u }; _ < NumberOfRuns; ++_)
    {
        std::cout << std::endl;
        pop = basePopulation;
        algo.SetPopulation(std::move(pop));
        individual = algo.RunAlgorithm();
        auto score = algo.GetFitnessScore(individual);
        results[score]++;
        individuals[score].insert(std::move(individual));
    }
    WriteResultsToFile(results, individuals);



    //TestFor5Nodes(algorithm);
    /*try {
        std::cout << std::boolalpha << a.IsSolution(std::vector<bool>{true,true,false,false,false});
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }*/
    return 0;
}