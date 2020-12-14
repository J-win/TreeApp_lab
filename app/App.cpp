#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <time.h>
#include "AVLTree.h"
#include "SplayTree.h"
#include "TreapTree.h"

void generateIntData(std::mt19937& mersenne, std::vector<int>& dataBase, const size_t sizeDataBase, const std::pair<int, int>& spanData)
{
    int randNum = 0;
    std::vector<bool> repeat(spanData.second, false);

    for (size_t i = 0; i < sizeDataBase; ++i)
    {
        randNum = mersenne() % (spanData.second - spanData.first) + spanData.first;
        while (repeat[randNum])
        {
            randNum = mersenne() % (spanData.second - spanData.first) + spanData.first;
        }
        dataBase.push_back(randNum);
        repeat[randNum] = true;
    }
}

void generateStringData(std::mt19937& mersenne, std::vector<std::string>& dataBase, const size_t sizeDataBase, const std::pair<int, int>& spanData)
{
    const std::string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (size_t i = 0; i < sizeDataBase; ++i)
    {
        std::string randString;
        int lengthRandString = mersenne() % (spanData.second - spanData.first) + spanData.first;
        for (size_t j = 0; j < lengthRandString; ++j)
        {
            int randChar = mersenne() % 61;
            randString.push_back(charSet[randChar]);
        }
        dataBase.push_back(randString);
    }
}

void writeInFile(const std::string& fileWrite, const std::vector<std::pair<int, int>>& timesOperation)
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
}

template <typename TypeTree, typename TypeDataBase>
void measureTimeInsertAndFind(TypeTree& tree, const TypeDataBase& dataBase,
    const size_t iteration, const size_t step,
    const std::string& fileWriterInsert, const std::string& fileWriterFind)
{
    std::vector<std::pair<int, int>> timesInsert;
    std::vector<std::pair<int, int>> timesFind;

    for (size_t k = 0; k < iteration; ++k)
    {
        auto begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            tree.insert({ dataBase[i], dataBase[i] });
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesInsert.push_back({ k * step, elapsed.count() });

        begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            tree.find(dataBase[i]);
        }
        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesFind.push_back({ k * step, elapsed.count() });
    }

    writeInFile(fileWriterInsert, timesInsert);
    writeInFile(fileWriterFind, timesFind);
}

template <typename TypeTree, typename TypeDataBase>
void measureTimeInsertAndFind(TypeTree& tree, const TypeDataBase& dataBase,
    const size_t iteration, const size_t step, const std::vector<int> priority,
    const std::string& fileWriterInsert, const std::string& fileWriterFind)
{
    std::vector<std::pair<int, int>> timesInsert;
    std::vector<std::pair<int, int>> timesFind;

    for (size_t k = 0; k < iteration; ++k)
    {
        auto begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            tree.insert(dataBase[i], priority[i], dataBase[i]);
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesInsert.push_back({ k * step, elapsed.count() });

        begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            tree.find(dataBase[i]);
        }
        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesFind.push_back({ k * step, elapsed.count() });
    }

    writeInFile(fileWriterInsert, timesInsert);
    writeInFile(fileWriterFind, timesFind);
}

template <typename TypeTree, typename TypeDataBase>
void measureTimeRemove(TypeTree& tree, const TypeDataBase& dataBase,
    const size_t iteration, const size_t step, const std::string fileWriterRemove)
{
    std::vector<std::pair<int, int>> timesRemove;

    for (size_t k = 0; k < iteration; ++k)
    {
        auto begin = std::chrono::steady_clock::now();
        for (size_t i = k * step; i < (k + 1) * step; ++i)
        {
            tree.erase(dataBase[i]);
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        timesRemove.push_back({ dataBase.size() - k * step, elapsed.count() });
    }

    writeInFile(fileWriterRemove, timesRemove);
}

int main(int argc, char* argv[])
{
    std::random_device rd;
    std::mt19937 mersenne(rd());

    const std::string fileWriterInsertForMap = "..\\..\\..\\script\\timesInsertInMap.txt";
    const std::string fileWriterFindForMap = "..\\..\\..\\script\\timesFindInMap.txt";
    const std::string fileWriterRemoveForMap = "..\\..\\..\\script\\timesRemoveInMap.txt";

    const std::string fileWriterInsertForAVLTree = "..\\..\\..\\script\\timesInsertInAVLTree.txt";
    const std::string fileWriterFindForAVLTree = "..\\..\\..\\script\\timesFindInAVLTree.txt";
    const std::string fileWriterRemoveForAVLTree = "..\\..\\..\\script\\timesRemoveInAVLTree.txt";

    const std::string fileWriterInsertForSplayTree = "..\\..\\..\\script\\timesInsertInSplayTree.txt";
    const std::string fileWriterFindForSplayTree = "..\\..\\..\\script\\timesFindInSplayTree.txt";
    const std::string fileWriterRemoveForSplayTree = "..\\..\\..\\script\\timesRemoveInSplayTree.txt";

    const std::string fileWriterInsertForTreapTree = "..\\..\\..\\script\\timesInsertInTreapTree.txt";
    const std::string fileWriterFindForTreapTree = "..\\..\\..\\script\\timesFindInTreapTree.txt";
    const std::string fileWriterRemoveForTreapTree = "..\\..\\..\\script\\timesRemoveInTreapTree.txt";

    const int step = 10000;
    const int iteration = 200;
    const bool flag = true;

    if (flag)
    {
        std::vector<int> dataBaseInt;
        std::vector<int> priority;
        AVLTree<int, int> avlTree;
        std::map<int, int> rbTree;
        SplayTree<int, int> splayTree;
        TreapTree<int, int, int> treapTree;

        std::cout << "Generation data..." << std::endl;
        generateIntData(mersenne, dataBaseInt, step * iteration, { 1, 2 * step * iteration });
        generateIntData(mersenne, priority, step * iteration, { 1, 2 * step * iteration });

        std::cout << "Run operations..." << std::endl;

        measureTimeInsertAndFind(rbTree, dataBaseInt, iteration, step, fileWriterInsertForMap, fileWriterFindForMap);
        measureTimeRemove(rbTree, dataBaseInt, iteration, step, fileWriterRemoveForMap);

        measureTimeInsertAndFind(avlTree, dataBaseInt, iteration, step, fileWriterInsertForAVLTree, fileWriterFindForAVLTree);
        measureTimeRemove(avlTree, dataBaseInt, iteration, step, fileWriterRemoveForAVLTree);

        measureTimeInsertAndFind(splayTree, dataBaseInt, iteration, step, fileWriterInsertForSplayTree, fileWriterFindForSplayTree);
        measureTimeRemove(splayTree, dataBaseInt, iteration, step, fileWriterRemoveForSplayTree);

        measureTimeInsertAndFind(treapTree, dataBaseInt, iteration, step, priority, fileWriterInsertForTreapTree, fileWriterFindForTreapTree);
        measureTimeRemove(treapTree, dataBaseInt, iteration, step, fileWriterRemoveForTreapTree);
    }
    else
    {
        std::vector<std::string> dataBaseString;
        std::vector<int> priority;
        AVLTree<std::string, std::string> avlTree;
        std::map<std::string, std::string> rbTree;
        SplayTree<std::string, std::string> splayTree;
        TreapTree<std::string, int, std::string> treapTree;

        std::cout << "Generation data..." << std::endl;
        generateStringData(mersenne, dataBaseString, step * iteration, { 1, 100 });
        generateIntData(mersenne, priority, step * iteration, { 1, 2 * step * iteration });

        std::cout << "Run operations..." << std::endl;

        measureTimeInsertAndFind(rbTree, dataBaseString, iteration, step, fileWriterInsertForMap, fileWriterFindForMap);
        measureTimeRemove(rbTree, dataBaseString, iteration, step, fileWriterRemoveForMap);

        measureTimeInsertAndFind(avlTree, dataBaseString, iteration, step, fileWriterInsertForAVLTree, fileWriterFindForAVLTree);
        measureTimeRemove(avlTree, dataBaseString, iteration, step, fileWriterRemoveForAVLTree);

        measureTimeInsertAndFind(splayTree, dataBaseString, iteration, step, fileWriterInsertForSplayTree, fileWriterFindForSplayTree);
        measureTimeRemove(splayTree, dataBaseString, iteration, step, fileWriterRemoveForSplayTree);

        measureTimeInsertAndFind(treapTree, dataBaseString, iteration, step, priority, fileWriterInsertForTreapTree, fileWriterFindForTreapTree);
        measureTimeRemove(treapTree, dataBaseString, iteration, step, fileWriterRemoveForTreapTree);
    }
    return 0;
}