inline int NodesNumber;
inline int PopulationSize = 100;
inline const int NumberOfIterations = 10;
inline const double LambdaFitness = 7.5;
inline const int IsSolutionPoints = 40;
inline const int TourneySize = 4;
inline const int PopulationAfterSelection = PopulationSize;
inline const double MutationRate = 0.01;
inline const double CrossOverParticipationRate = 0.5;
inline const size_t NumberOfRuns = 1000;
inline bool ElitismIsEnabled = true;

inline const std::string FilePathToGraph{ "intrare.txt" };
inline const std::string FilePathToResults{ "iesire.txt" };
inline const std::string FilePathToPopulation{ "population.txt" };
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