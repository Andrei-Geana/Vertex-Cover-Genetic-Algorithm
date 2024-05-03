#ifndef ALGORITHMDATA_H
#define ALGORITHMDATA_H
#pragma once
#include <string>

class AlgorithmData
{
public:
	static int NodesNumber;
	static int PopulationSize;
	static const int NumberOfIterations;
	static const double LambdaFitness;
	static const int IsSolutionPoints = 20;
	static const int TourneySize = 4;
	static const int PopulationAfterSelection;
	static const double MutationRate;
	static const double CrossOverParticipationRate;
	static const size_t NumberOfRuns = 1000;
	static const bool ElitismIsEnabled = true;

	static const std::string FilePathToGraph;
	static const std::string FilePathToResults;
	static const std::string FilePathToPopulation;
};
#endif

//best data values
/*
	daca modific TourneySize		=> (daca il micsorez) se exploreaza mai multe solutii
									=> (daca il cresc) algoritmul gaseste mai putine solutii, dar converge mai rapid

	daca modific PopulationSize		=> (daca il cresc), creste si diversitatea populatiei => mai multi indivizi unici => creste probabilitatea ca solutia optima sa fie generata
									   (daca il micsorez), scade diversitatea populatiei => converge spre o lista mica de solutii

	daca modific NumberOfIterations => (daca il cresc) se exploreaza mai multe solutii, se ajunge la solutii optime si se ajunge la convergenta
									   (daca il scad) se exploreaza putine solutii, nu se ajunge la solutii optime si nu se ajunge la convergenta in majoritatea cazurilor
*/

/*
* calculez scorul la fiecare iteratie : performanta -60ms
* elitism (pastrez cel mai bun individ) : rezultate mai bune
*/