#pragma once
#include <vector>
#include <chrono>
#include "Individual.h"

//typedef  std::unordered_multiset<Individual, Individual::Hash, Individual::IsEqual> ListOfIndividuals;
typedef std::vector<Individual> ListOfIndividuals;
typedef std::vector<Individual*> ListOfPointersToIndividuals;


class VertexCoverGeneticAlgorithm
{
public:

    VertexCoverGeneticAlgorithm(Graph* base, ListOfIndividuals&& population);

    VertexCoverGeneticAlgorithm(Graph* base);

    void SetPopulation(ListOfIndividuals&& population);

    void ShowPopulation() const;

    Individual RunAlgorithm();

    void SavePopulationInFile(const std::string& FilePath = AlgorithmData::FilePathToPopulation) const;
    void ReadPopulationFromFile(const std::string& FilePath = AlgorithmData::FilePathToPopulation) ;

public:

    static ListOfIndividuals GetPopulation(int numberOfPeople = AlgorithmData::PopulationSize);

private:
    void MakeOneIteration();

    void SetGraphToPopulation();

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
    Graph* baseGraph;
    ListOfIndividuals population;
};