#pragma once
#include <random>

class Helper
{
    public:
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

        static double GetChance(double min=0, double max=1)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distrib(min, max);
            return distrib(gen);
        }
};