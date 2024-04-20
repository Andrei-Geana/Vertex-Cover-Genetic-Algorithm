int NodesNumber;
int PopulationSize = 200;
const int NumberOfIterations = 10;
const double LambdaFitness = 5;
const int NotSolutionPenalty = 10;
const int TourneySize = 2;
const int PopulationAfterSelection = PopulationSize;
const double MutationRate = 0.01;
const double CrossOverParticipationRate = 0.5;

//best results:
/*
int PopulationSize = 200; //principal factor which influences whether the best solution is "created"
const int NumberOfIterations = 10;
const double LambdaFitness = 5;
const int NotSolutionPenalty = 10;
const int TourneySize = 6; //principal factor which influences whether the best solution is "created"
const int PopulationAfterSelection = PopulationSize;
const double MutationRate = 0.01;
const double CrossOverParticipationRate = 0.5; //principal factor which influences whether the best solution is "created"
*/


const size_t NumberOfRuns = 10;