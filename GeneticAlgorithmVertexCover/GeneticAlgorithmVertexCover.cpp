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
        //return individual.GetNumberOf1s() + LambdaFitness * baseGraph.GetNumberOfNotVerifiedArch(individual) + baseGraph.GetNumberOfArches() + (!baseGraph.IsSolution(individual)*LambdaFitness);
    }

    void RunAlgorithm()
    {
        size_t indexOfIteration{ 0u };
        while (indexOfIteration < NumberOfIterations)
        {
            //ShowPopulation();
            MakeOneIteration();
            indexOfIteration++;
            auto individ = GetBestPersonInGroup(population);
            std::cout << indexOfIteration <<":BEST: " << individ << " SCORE: " << GetFitnessScore(individ) << std::endl;

        }
        std::cout << "FINISHED";
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

        ListOfIndividuals selectedForCrossOver;

        //select those that will participate in crossover
        for (const auto& individ : newPopulation)
        {
            if (Individual::GetChance() < CrossOverParticipationRate)
            {
                selectedForCrossOver.insert(individ);
            }
        }
        //get even number of individuals
        if (selectedForCrossOver.size() % 2 != 0)
        {
            auto it = selectedForCrossOver.begin();
            std::advance(it, selectedForCrossOver.size() - 1);
            selectedForCrossOver.extract(it);
        }

        //make crossover and then try mutation
        for (auto it = selectedForCrossOver.begin(); it!=selectedForCrossOver.end();)
        {
            Individual parent1 = *it;
            std::advance(it, 1);
            Individual parent2 = *it;
            auto [c1, c2] = Crossover(parent1, parent2);
            c1.Mutate();
            c2.Mutate();

            //Deletes all occurences
            /*newPopulation.erase(parent1);
            newPopulation.erase(parent2);*/

            auto itOfParent1 = newPopulation.extract(parent1);
            auto itOfParent2 = newPopulation.extract(parent2);

            newPopulation.insert(c1);
            newPopulation.insert(c2);

            if (it == std::prev(selectedForCrossOver.end())) {
                break;
            }

        }

        population = newPopulation;
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
    }

    Individual GetBestPersonInGroup(const ListOfIndividuals& group) const
    {
        double bestFitnessScore{ INT_MAX };
        Individual winner;
        for (const auto& individual : group)
        {
            if (double score{ GetFitnessScore(individual) }; score < bestFitnessScore)
            {
                winner = individual;
                bestFitnessScore = score;
            }
        }
        return winner;
    }

    std::pair<Individual, Individual> Crossover(const Individual& parent1, const Individual& parent2) const
    {
        size_t point = (size_t)GetRandomIndex(0, parent1.GetNumberOfChromosomes() - 1);
        std::vector<bool> chromosomesForFirstChild(parent1.GetNumberOfChromosomes());
        std::vector<bool> chromosomesForSecondChild(parent1.GetNumberOfChromosomes());

        for (size_t index{ 0u }; index < point; ++index)
            chromosomesForFirstChild[index] = parent1[index];
        for (size_t index{ 0u }; index < point; ++index)
            chromosomesForSecondChild[index] = parent2[index];

        for (size_t index{ point }; index < parent1.GetNumberOfChromosomes(); ++index)
            chromosomesForFirstChild[index] = parent2[index];
        for (size_t index{ point }; index < parent1.GetNumberOfChromosomes(); ++index)
            chromosomesForSecondChild[index] = parent1[index];

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

int main()
{
    Graph a{ NodesNumber };
    try {
        a.AddArch(0, 1);
        a.AddArch(0, 2);
        a.AddArch(1, 3);
        a.AddArch(4, 3);
        a.AddArch(4, 5);
        a.AddArch(4, 6);
        a.AddArch(5, 6);
        a.AddArch(2, 6);
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }
    VertexCoverGeneticAlgorithm algorithm{ a, VertexCoverGeneticAlgorithm::GetPopulation() };
    //TestFor5Nodes(algorithm);
    algorithm.RunAlgorithm();
    /*try {
        std::cout << std::boolalpha << a.IsSolution(std::vector<bool>{true,true,false,false,false});
    }
    catch (std::exception a) {
        std::cout << a.what() << std::endl;
    }*/
    return 0;
}