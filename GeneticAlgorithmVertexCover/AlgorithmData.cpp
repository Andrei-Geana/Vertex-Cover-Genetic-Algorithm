#include "AlgorithmData.h";

int AlgorithmData::NodesNumber;
int AlgorithmData::PopulationSize = 100;
const int AlgorithmData::NumberOfIterations = 15;
const int AlgorithmData::PopulationAfterSelection = AlgorithmData::PopulationSize;
const double AlgorithmData::LambdaFitness = 7.5;
const double AlgorithmData::MutationRate = 0.01;
const double AlgorithmData::CrossOverParticipationRate = 0.5;
const std::string AlgorithmData::FilePathToGraph = "intrare.txt";
const std::string AlgorithmData::FilePathToResults = "iesire.txt";
const std::string AlgorithmData::FilePathToPopulation = "population.txt";
