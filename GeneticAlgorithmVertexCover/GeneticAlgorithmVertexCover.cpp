#include "Graph.h"
#include <fstream>
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


typedef  std::unordered_multiset<Individual, Individual::Hash, Individual::IsEqual> ListOfIndividuals;

class VertexCoverGeneticAlgorithm
{
public:

    VertexCoverGeneticAlgorithm(Graph& base, ListOfIndividuals&& population)
    {
        baseGraph = base;
        this->population = population;
    }

    //basic GetPopulation
    static ListOfIndividuals GetPopulation(int numberOfPeople = PopulationSize)
    {
        ListOfIndividuals population;
        while (population.size() < numberOfPeople)
        {
            auto person = GetPerson();

            population.insert(person);

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
        double fitnessScore = individual.GetNumberOf1s() + LambdaFitness * baseGraph.GetNumberOfNotVerifiedArch(individual) + (!baseGraph.IsSolution(individual)) * NotSolutionPenalty;
        return fitnessScore;
    }

    void RunAlgorithm()
    {
        std::cout << "---------------------------------------------------------------" << std::endl <<"EPOCHS: "<< std::endl;
       // ShowPopulation();
        auto individual = GetBestPersonInGroup(population);
        std::cout << "\tI: Best Individual: " << individual << " Score: " << GetFitnessScore(individual) << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();

        size_t indexOfIteration{ 0u };
        while (indexOfIteration < NumberOfIterations)
        {
            MakeOneIteration();
            indexOfIteration++;
            individual = GetBestPersonInGroup(population);
            std::cout << "\t";
            if (indexOfIteration == NumberOfIterations)
                std::cout << "E";
            else
                std::cout << indexOfIteration;
            std::cout << ": Best Individual: " << individual << " Score: " << GetFitnessScore(individual) << std::endl;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        std::cout << "Time taken: " << duration << " milliseconds\n";
    }


private:
    void MakeOneIteration()
    {
        std::vector<ListOfIndividuals> groups;
        while (groups.size() != PopulationSize)
        {
            groups.emplace_back(GetGroup());
        }
        ListOfIndividuals newPopulation;
        for (const auto& group : groups)
        {
            newPopulation.insert(GetBestPersonInGroup(group));
        }

        //DEBUGGING
        /*int index = 0;
        for (const auto& winner : newPopulation)
        {
            std::cout << index++ << ": " << GetFitnessScore(winner) << '\n';
        }*/
        
        //ListOfIndividuals => individuals have a bigger chance to crossover with an another individual which has the same score => converges faster
        //vector => standard crossover => bigger diversity
        std::vector<Individual> selectedForCrossOver;

        //select those that will participate in crossover
        for (const auto& individual : newPopulation)
        {
            if (Individual::GetChance() < CrossOverParticipationRate)
            {
                selectedForCrossOver.emplace_back(individual);
            }
        }

        //get even number of individuals
        if (selectedForCrossOver.size() % 2 != 0)
        {
            selectedForCrossOver.pop_back();
        }

        //make crossover and then try mutation
        for (size_t index{ 0u } ; index<selectedForCrossOver.size()-1; index+=2u)
        {
            Individual parent1 = selectedForCrossOver[index];
            Individual parent2 = selectedForCrossOver[index+1];
            auto [c1, c2] = TwoPointCrossover(parent1, parent2);
            //c1.Mutate();
            //c2.Mutate();

            //deletes all occurences of parents
            /*newPopulation.erase(parent1);
            newPopulation.erase(parent2);*/

            //deletes parents
            auto itOfParent1 = newPopulation.extract(parent1);
            auto itOfParent2 = newPopulation.extract(parent2);

            newPopulation.insert(c1);
            newPopulation.insert(c2);
        }

        ListOfIndividuals resultPopulation;
        for (auto individual : newPopulation)
        {
            individual.Mutate();
            resultPopulation.insert(std::move(individual));
        }

        population = resultPopulation;
    }

    ListOfIndividuals GetGroup()
    {
        ListOfIndividuals group;
        while (group.size() < TourneySize)
        {
            auto person = GetRandomPerson();
            group.insert(person);
        }
        return group;
    }
    
    Individual GetRandomPerson()
    {
        int index{ GetRandomIndex(0, (int)population.size() - 1) };
        size_t count{ 0u };
        for (auto it = population.begin(); it != population.end(); ++it) {
            if (count == index) {
                return *it;
            }
            count++;
        }
        throw new std::exception("unable to select person");
    }

    Individual GetBestPersonInGroup(const ListOfIndividuals& group) const
    {
        double bestFitnessScore{ INT_MAX };
        Individual winner;
        double score{ 0 };
        for (const auto& individual : group)
        {
            if (score = GetFitnessScore(individual); score < bestFitnessScore)
            {
                winner = individual;
                bestFitnessScore = score;
            }
        }
        return winner;
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

void TestFor10Nodes(const VertexCoverGeneticAlgorithm& algorithm)
{
    std::cout << "BEST : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, true, false, false, false, false, false, false, false, false} }) << std::endl;
    std::cout << "SECOND BEST : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, true, true, false, false, false, false, false, false, false} }) << std::endl;
    std::cout << "THIRD BEST : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, true, false, false, true, false, false, false, false, false} }) << std::endl;
    std::cout << "NO GOOD : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, false, false, false, false, false, false, false, false, false} }) << std::endl;
}

void TestFor5Nodes(const VertexCoverGeneticAlgorithm& algorithm)
{
    std::cout << "BEST : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, true, false, false, false} }) << std::endl;
    std::cout << "SECOND BEST : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, true, true, false, false} }) << std::endl;
    std::cout << "THIRD BEST : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, true, false, false, true} }) << std::endl;
    std::cout << "NO GOOD : \n";
    std::cout << algorithm.GetFitnessScore(Individual{ std::vector<bool>{false, false, false, false, false} }) << std::endl;
}

Graph ReadGraphFromFile()
{
    std::ifstream fin{ "intrare.txt" };
    fin >> NodesNumber;
    //only for small number of chromosomes
    //PopulationSize = (int)std::pow(2, NodesNumber);
    Graph graph{ NodesNumber };
    int from{}, to{};
    while (fin >> from >> to)
    {
        graph.AddArch(from, to);
    }
    fin.close();
    return graph;

}


int main()
{
    Graph a{ ReadGraphFromFile()};
    auto basePopulation = VertexCoverGeneticAlgorithm::GetPopulation();


    auto pop = basePopulation;
    VertexCoverGeneticAlgorithm algorithm{ a, std::move(pop)};
    algorithm.RunAlgorithm();

    for (size_t _{ 0u }; _ < NumberOfRuns-1; ++_)
    {
        std::cout << std::endl;
        pop = basePopulation;
        VertexCoverGeneticAlgorithm algo{ a, std::move(pop) };
        algo.RunAlgorithm();
    }




    //TestFor5Nodes(algorithm);
    /*try {
        std::cout << std::boolalpha << a.IsSolution(std::vector<bool>{true,true,false,false,false});
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }*/
    return 0;
}