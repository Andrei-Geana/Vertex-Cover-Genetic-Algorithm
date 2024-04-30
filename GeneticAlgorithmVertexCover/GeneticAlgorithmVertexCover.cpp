#include "VertexCoverGeneticAlgorithm.h"
#include <fstream>
#include <map>
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
    Graph a{ Graph::ReadGraphFromFile()};
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
        auto score = individual.GetScore();
        results[score]++;
        individuals[score].insert(std::move(individual));
        WriteResultsToFile(results, individuals);
    }
    WriteResultsToFile(results, individuals);

    return 0;
}