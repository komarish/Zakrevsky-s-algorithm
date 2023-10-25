#pragma once

#include <iostream>

using namespace std;

class Heuristics
{
public:
	virtual void checkRules() = 0;
    virtual string getName() = 0;
};

class greedHeuristics : public Heuristics 
{       
public:
    void checkRules() 
    {
        cout << "greedHeuristics\n";
    }

    string getName()
    {
        return "greedHeuristics";
    }
};

class simpleHeuristics : public Heuristics 
{     
public:
    void checkRules() 
    {
        cout << "simpleHeuristics\n";
    }

    string getName()
    {
        return "simpleHeuristics";
    }
};