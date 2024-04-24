int NodesNumber;
int PopulationSize = 100;
const int NumberOfIterations = 5;
const double LambdaFitness = 7.5;
const int NotSolutionPenalty = 70;
const int TourneySize = 2;
const int PopulationAfterSelection = PopulationSize;
const double MutationRate = 0.01;
const double CrossOverParticipationRate = 0.5;
const size_t NumberOfRuns = 100;

const std::string FilePathToGraph{ "intrare.txt" };
const std::string FilePathToResults{ "iesire.txt" };
//best data values
/*
	daca modific TourneySize		=> (daca il micsorez) se exploreaza mai multe solutii
									=> (daca il cresc) algoritmul gaseste mai putine solutii, dar converge mai rapid

	daca modific PopulationSize		=> (daca il cresc), creste si diversitatea populatiei => mai multi indivizi unici => creste probabilitatea ca solutia optima sa fie generata
									   (daca il micsorez), scade diversitatea populatiei => converge spre o lista mica de solutii

	daca modific NumberOfIterations => (daca il cresc) se exploreaza mai multe solutii, se ajunge la solutii optime si se ajunge la convergenta
									   (daca il scad) se exploreaza putine solutii, nu se ajunge la solutii optime si nu se ajunge la convergenta in majoritatea cazurilor
*/