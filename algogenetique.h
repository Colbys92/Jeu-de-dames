#pragma once
#include "arbre.h"
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <limits>
using namespace std;

class Individu{
    int score;
    float manWeight;
    float kingWeight;
    int depth;
    float nbMoveWeight;
    float advancementForwardWeight;
    float centralWeight;
public:
    Individu(int score1, float manWeight1, float kingWeight1, int depth1, float nbMoveWeight1, float advancementForwardWeight1, float centralWeight1);
    ~Individu() {}
    Individu(const Individu& i):score(i.score),manWeight(i.manWeight),kingWeight(i.kingWeight),depth(i.depth),nbMoveWeight(i.nbMoveWeight),advancementForwardWeight(i.advancementForwardWeight),centralWeight(i.centralWeight){}
    void operator=(const Individu& i);
    float getManWeight(){return manWeight;}
    void setManWeight(float newManWeight){manWeight=newManWeight;}
    float getKingWeight() {return kingWeight;}
    void setKingWeight(float newKingWeight){kingWeight=newKingWeight;}
    int getScore() {return score;}
    void addToScore(int toAdd);
    int getDepth(){return depth;}
    void setDepth(int newDepth){depth=newDepth;}
    float getNbMoveWeight(){return nbMoveWeight;}
    void setNbMoveWeight(float newNbMoveWeight){nbMoveWeight=newNbMoveWeight;}
    float getAdvancementForwardWeight(){return advancementForwardWeight;}
    void setAdvancementForwardWeight(float newAdvancementForwardWeight){advancementForwardWeight=newAdvancementForwardWeight;}
    float getCentralWeight(){return centralWeight;}
    void setCentralWeight(float newCentralWeight){centralWeight=newCentralWeight;}
    bool operator<(Individu i2);
    Move bestMoveAlphaBeta(Board& B,string color,float alpha, float beta );

};


class PowerfulIndividu{
    int score;
    Individu Ibegin;
    Individu Imiddle;
    Individu Iend;
public:
    PowerfulIndividu(Individu i1,Individu i2, Individu i3):Ibegin(i1),Imiddle(i2),Iend(i3){}
    Individu getIbegin(){ return Ibegin; }
    Individu getImiddle(){ return Imiddle; }
    Individu getIend(){ return Iend; }
    int getScore() {return score;}
    Move bestMoveAlphaBeta(Board& B, string color, float alpha, float beta);
    void addToScore(int toAdd);
    void mutationPowerful(int proba);
    bool operator<(PowerfulIndividu i){return score<i.score;}
};

