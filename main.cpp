#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

#define ITERATIONS 10000
#define PERCENT_TO_DIE_OFF 0.2
#define MUTATION_RATE 0.1

const std::string actual = "1234567890123456";

bool check(std::string guess)
{
    for (int i = 0; i < 16; i ++)
    {
        if (actual[i] != guess[i])
        {
            return false;
        }
    }
    return true;
}

int score(std::string guess, int goal_time)
{
   int avg = 0;
   for (int i=  0; i<10; i++)
   {
       auto now = std::chrono::high_resolution_clock::now();
       check(guess);
       int dt = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - now).count();
       avg += dt;
   }
    avg /= 10;
    return goal_time - avg;
}

void evolveGA(std::vector<std::pair<std::string, int>>& population, int& goal_time)
{

    for (int i = 0; i < ITERATIONS; i++)
    {
        for (int j= 0 ; j < population.size(); j++)
        {
            population[j].second = score(population[j].first, goal_time);
        }

        std::sort(population.begin(), population.end(), [](std::pair<std::string, int> p1, std::pair<std::string, int> p2) {return p1.second < p2.second;});
        int current_size = population.size();
        population = std::vector<std::pair<std::string, int>>(population.begin(), population.end()-(PERCENT_TO_DIE_OFF*current_size));
        int to_populate = current_size - population.size();
        current_size = population.size();
        while (to_populate > 0)
        {
            int split_point = std::min(15, (int)(16*((float)rand()/RAND_MAX)));
            std::string best_first_part = std::string(population[0].first.begin(), population[0].first.begin()+split_point);
            std::string best_second_part = std::string(population[0].first.begin()+split_point, population[0].first.end());

            int random_next = (current_size-1)*((float)rand()/RAND_MAX);

            auto next = population[random_next];

            std::string random_first_part = std::string(next.first.begin(), next.first.begin() + split_point);
            std::string random_second_part = std::string(next.first.begin()+split_point, next.first.end());

            population.push_back(std::make_pair(best_first_part + random_second_part, 100));
            population.push_back(std::make_pair(random_first_part + best_second_part, 100));
            to_populate-= 2;
        }

        // mutate
        int random_selection1, random_selection2; char random_selection3;
        for (int i= 0; i < population.size()*MUTATION_RATE; i++)
        {
            random_selection1 = (population.size()-1)*((float)rand()/RAND_MAX);
            random_selection2 = std::min(15, (int)(16.0*((float)rand()/RAND_MAX)));
            random_selection3 = std::min(9, (int)(10.0*((float)rand()/RAND_MAX))) + '0';
            population[random_selection1].first[random_selection2] = random_selection3;

        }

    }

}

std::vector<std::pair<std::string, int>> make_random_population(int amount)
{
    std::vector<std::pair<std::string, int>> population;
    for (int i= 0 ; i < amount; i++)
    {
        std::string rand_string = "";
        for (int i= 0; i < 16; i++)
        {
            rand_string += '0' + std::min(9, (int)(10.0*((float)rand()/RAND_MAX)));
        }
        population.push_back(std::make_pair(rand_string, 100));
    }
    return population;
}


int main() {

    // get the goal time
    int avg = 0;
    for (int i= 0; i < 5000; i++)
    {
        avg += -score(actual, 0);\
    }
    int goal_time = avg/5000;
    std::cout << "Goal time: " << goal_time << std::endl;


    auto population = make_random_population(100);
    srand((unsigned)time(NULL));
    evolveGA(population, goal_time);

    for (int i = 0; i < population.size(); i++)
    {
        population[i].second = score(population[i].first, goal_time);
    }
    std::sort(population.begin(), population.end(), [&](std::pair<std::string, int> p1, std::pair<std::string, int> p2) {return p1.second < p2.second;});

    std::cout << "Population:\n";
    for (int i= 0; i < population.size(); i++)
    {
        std::cout << population[i].first << " Score: " << population[i].second << std::endl;
    }


    return 0;
}

