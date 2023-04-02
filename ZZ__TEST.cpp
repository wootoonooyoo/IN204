#include <iostream>
#include <random>
#include <map>
#include <string>
#include <string_view>
 
// Write a function that returns a random int between 0 and 6 (inclusive)
// Use std::random_device to seed the random number generator
// Use std::mt19937 as the random number generator
// Use std::uniform_int_distribution to generate the random number
int randomNoGenerator()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 6);
    return distrib(gen);    
}

int main()
{
    for(int z=0; z<10 ; z++)
    {
        std::cout << randomNoGenerator() << std::endl;
    }

}
