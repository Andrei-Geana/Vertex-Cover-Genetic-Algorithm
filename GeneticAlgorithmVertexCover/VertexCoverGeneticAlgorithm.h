#pragma once
#include <vector>
#include "Individual.h"
#include "Graph.h"
#include <chrono>

//typedef  std::unordered_multiset<Individual, Individual::Hash, Individual::IsEqual> ListOfIndividuals;
typedef std::vector<Individual> ListOfIndividuals;
typedef std::vector<Individual*> ListOfPointersToIndividuals;


class VertexCoverGeneticAlgorithm
{
public:

    VertexCoverGeneticAlgorithm(const Graph& base, ListOfIndividuals&& population);

    VertexCoverGeneticAlgorithm(const Graph& base);

    void SetPopulation(ListOfIndividuals&& population);

    void ShowPopulation() const;

    //best fitness = smallest (min)
    /*double GetFitnessScore(const Individual& individual) const
    {
        return individual.GetNumberOf1s() + LambdaFitness * baseGraph.GetNumberOfNotVerifiedArch(individual) + (!baseGraph.IsSolution(individual)) * IsSolutionPoints;
    }*/

    //best fitness = biggest (max)
    /*double GetFitnessScore(const Individual& individual) const
    {
        return 100 / (individual.GetNumberOf1s() + LambdaFitness * baseGraph.GetNumberOfNotVerifiedArch(individual) + (!baseGraph.IsSolution(individual)) * IsSolutionPoints);
    }*/

    //best fitness = biggest (max)
    double GetFitnessScore(const Individual& individual) const;

    Individual RunAlgorithm();

    void SavePopulationInFile(const std::string& FilePath = FilePathToPopulation) const;
    void ReadPopulationFromFile(const std::string& FilePath = FilePathToPopulation) ;

public:

    static ListOfIndividuals GetPopulation(int numberOfPeople = PopulationSize);

private:
    void MakeOneIteration();

    void CalculateAllFitnesses();

    //selection function
    ListOfIndividuals TournamentSelection();

    ListOfIndividuals RouletteWheelSelection();

    ListOfPointersToIndividuals GetIndividualsForCrossOver();

    void CrossOverPopulationForSelectedIndividuals(const ListOfPointersToIndividuals& selectedForCrossOver);

    void MutatePopulation();

    ListOfPointersToIndividuals GetGroup();

    Individual* GetRandomPerson();

    Individual* GetBestPersonInGroup(const ListOfPointersToIndividuals& group) const;

    Individual GetBestPersonInPopulation(const ListOfIndividuals& group) const;

    std::pair<Individual, Individual> OnPointCrossover(const Individual& parent1, const Individual& parent2) const;

    std::pair<Individual, Individual> TwoPointCrossover(const Individual& parent1, const Individual& parent2) const;

    std::pair<Individual, Individual> UniformCrossover(const Individual& parent1, const Individual& parent2) const;

private:
    Graph baseGraph;
    ListOfIndividuals population;
};