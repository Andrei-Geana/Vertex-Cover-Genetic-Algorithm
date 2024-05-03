#include "Graph.h"

Graph::Graph() {/*EMPTY*/ }

Graph::Graph(int n)
{
    for (int i = 0; i < n; ++i)
    {
        nodes.insert(i);
    }
}

void Graph::AddArch(int from, int to)
{
    if (!nodes.contains(from))
        throw std::exception("start node not in graph");
    if (!nodes.contains(to))
        throw std::exception("destination node not in graph");

    arches.insert(Arch{ from, to });
}

bool Graph::IsSolution(const std::vector<bool>& individ) const
{
    if (individ.size() != nodes.size())
        throw std::exception("vector size not equal to number of nodes in graph");

    for (const Arch& arc : arches)
    {
        if (individ[arc.From()])
            continue;
        if (individ[arc.To()])
            continue;
        return false;
    }

    return true;
}

int Graph::GetNumberOfNotVerifiedArch(const std::vector<bool>& individ) const
{
    int count{ 0 };
    for (const Arch& arc : arches)
    {
        if (individ[arc.From()])
            continue;
        if (individ[arc.To()])
            continue;
        count++;
    }
    return count;
}

void Graph::PrintAllArches()
{
    for (const Arch& elem : arches)
    {
        std::cout << elem.From() << "," << elem.To() << std::endl;
    }
}

int Graph::GetNumberOfArches() const
{
    return (int)arches.size();
}

Graph* Graph::ReadGraphFromFile(const std::string& FilePath)
{
    std::ifstream fin{ AlgorithmData::FilePathToGraph };
    fin >> AlgorithmData::NodesNumber;
    Graph* graph = new Graph{ AlgorithmData::NodesNumber };
    int from{}, to{};
    while (fin >> from >> to)
    {
        try
        {
            graph->AddArch(from, to);
        }
        catch (std::exception ex)
        {
            std::cout << ex.what() << " for arch: (" << from << ", " << to << ")" << std::endl;
        }
    }
    fin.close();
    return graph;

}
