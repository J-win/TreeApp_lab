#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <map>
#include <time.h>
#include "AVLTree.h"
#include "SplayTree.h"

void generateIntData(std::mt19937& mersenne, std::vector<int>& dataBase, const size_t sizeDataBase, const std::pair<int, int>& spanData)
{
    for (size_t i = 0; i < sizeDataBase; ++i)
    {
        int randNum = mersenne() % spanData.second + spanData.first;
        dataBase.push_back(randNum);
    }
}

void generateStringData(std::mt19937& mersenne, std::vector<std::string>& dataBase, const size_t sizeDataBase, const std::pair<int, int>& spanData)
{
    const std::string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (size_t i = 0; i < sizeDataBase; ++i)
    {
        int lengthRandString = mersenne() % spanData.second + spanData.first;
        std::string randString;
        for (size_t j = 0; j < lengthRandString; ++j)
        {
            int randChar = mersenne() % 61;
            randString.push_back(charSet[randChar]);
        }
        dataBase.push_back(randString);
    }
}

int main(int argc, char* argv[])
{
    std::random_device rd;
    std::mt19937 mersenne(rd());

    const std::string fileWriterForMap = "..\\..\\script\\timesInsertInMap.txt";
    const std::string fileWriterForAVLTree = "..\\..\\script\\timesInsertInAVLTree.txt";
    const std::string fileWriterForSplayTree = "..\\..\\script\\timesInsertInSplayTree.txt";

    std::vector<int> dataBaseInt;
    std::vector<std::string> dataBaseString;
    std::vector<std::pair<int, int>> timesInsertInMap;
    std::vector<std::pair<int, int>> timesInsertInAVLTree;
    std::vector<std::pair<int, int>> timesInsertInSplayTree;
    AVLTree<std::string, std::string> avlTree;
    std::map<std::string, std::string> rbTree;
    SplayTree<std::string, std::string> splayTree;

    const int step = 20000;
    const int iteration = 200;
    
    std::cout << "Generation data..." << std::endl;
    //generateIntData(mersenne, dataBaseInt, step * iteration, { 1, 1000000 });
    generateStringData(mersenne, dataBaseString, step * iteration, { 1, 100 });

    std::cout << "Run operations..." << std::endl;

    for (size_t k = 0; k < iteration; ++k)
    {
        auto begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            rbTree.insert({ dataBaseString[i], dataBaseString[i] });
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesInsertInMap.push_back({ k * step, elapsed_ms.count() });
    }

    for (size_t k = 0; k < iteration; ++k)
    {
        auto begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            avlTree.insert(dataBaseString[i], dataBaseString[i]);
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesInsertInAVLTree.push_back({ k * step, elapsed_ms.count() });
    }

    for (size_t k = 0; k < iteration; ++k)
    {
        auto begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            splayTree.insert(dataBaseString[i], dataBaseString[i]);
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesInsertInSplayTree.push_back({ k * step, elapsed_ms.count() });
    }

    std::cout << "Writing in file..." << std::endl;

    auto writeInFile = [] (const std::string& fileWrite, const std::vector<std::pair<int, int>>& timesOperation)
    {
        std::ofstream out;
        out.open(fileWrite);
        if (out.is_open())
        {
            for (size_t i = 0; i < timesOperation.size(); ++i)
            {
                out << timesOperation[i].first << " " << timesOperation[i].second << "\n";
            }
        }
        else
        {
            std::cout << "File \"" << fileWrite << "\" is not open" << std::endl;
        }
        out.close();
    };

    writeInFile(fileWriterForMap, timesInsertInMap);
    writeInFile(fileWriterForAVLTree, timesInsertInAVLTree);
    writeInFile(fileWriterForSplayTree, timesInsertInSplayTree);

    return 0;
}