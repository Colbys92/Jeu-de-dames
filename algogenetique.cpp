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
    float getManWeight(){return manWeight;}
    void setManWeight(float newManWeight){manWeight=newManWeight;}
    float getKingWeight() {return kingWeight;}
    void setKingWeight(float newKingWeight){kingWeight=newKingWeight;}
    int getScore() {return score;}
    int getDepth(){return depth;}
    int addToScore(int toAdd);
    float getNbMoveWeight(){return nbMoveWeight;}
    float getAdvancementForwardWeight(){return advancementForwardWeight;}
    float getCentralWeight(){return centralWeight;}
    operator<(Individu i2);
};

Individu::operator<(Individu i2){
    return(score<i2.getScore());
}

Individu::addToScore(int toAdd){
    score+=toAdd;
}

Individu::Individu(int score1, float manWeight1, float kingWeight1, int depth1, float nbMoveWeight1, float advancementForwardWeight1, float centralWeight1){
    score = score1;
    manWeight=manWeight1;
    kingWeight=kingWeight1;
    depth=depth1;
    nbMoveWeight=nbMoveWeight1;
    advancementForwardWeight=advancementForwardWeight1;
    centralWeight=centralWeight1;
}




//==================================Méthode pour l'évaluation========================

int match(Individu i1, Individu i2){
    // Renvoie 1 si le premier individu gagne; 2 si le second; 0 si personne ne gagne.
    Board B;
    Move m1,m2;
    float val;
    int compteur =0;
    while(compteur<200){


        if(B.playableMoves("white").size()>0){
//            m1=B.bestMove("white",i1.getManWeight(),i1.getKingWeight(),true,i1.getDepth()).second;
            //m1=B.bestMoveAlphaBeta("white",i1.getDepth(),i1.getManWeight(),i1.getKingWeight(),i1.getNbMoveWeight(),i1.getAdvancementForwardWeight(),i1.getCentralWeight());
//            if(val!=B.bestMoveAlphaBeta("white",i1.getDepth(),i1.getManWeight(),i1.getKingWeight(),true,-10000,10000).first){
//                cerr << "WTF BORDEL DE COUILLES" << endl;
//            }
            m1=B.bestMoveAlphaBeta("white",i1.getDepth(),i1.getManWeight(),i1.getKingWeight(),true,-10000,10000).second;
            B.playMove(m1);
            //cout << m1.getStart() << " " << m1.getArrival() << endl;

        }
        else{

            return 2;
        }
        if(B.playableMoves("black").size()>0){

            //m2=B.bestMoveAlphaBeta("black", i2.getDepth(),i2.getManWeight(),i2.getKingWeight(),i2.getNbMoveWeight(),i2.getAdvancementForwardWeight(),i2.getCentralWeight());
            m2=B.bestMoveAlphaBeta("black", i2.getDepth(), i2.getManWeight(),i2.getKingWeight(),true, -10000,10000).second;
            B.playMove(m2);
            //cout << m2.getStart() << " " << m2.getArrival() << "kill" << m2.getKills() << endl;
        }
        else{
            return 1;
        }
        compteur+=1;
    }
    return 0;
}


void resultsMatch(Individu& i1, Individu& i2){
    // Si un individu gagne, on ajoute 2 points à son score, si il y a match nul on ajoute 1, 0 si il perd.
    int results = match(i1,i2);
    if (results==1){
        i1.addToScore(2);
    }
    if(results==2){
        i2.addToScore(2);
    }
    if(results==0){
        i1.addToScore(1);
        i2.addToScore(1);
    }
}
int evaluation(vector<Individu>& individus){
    for(int i=0; i< individus.size(); i++){
        for(int j=i+1; j<individus.size(); j++){
            cout<<"("<<i<<","<<j<<")"<<endl;
            resultsMatch(individus[i],individus[j]);
            resultsMatch(individus[j],individus[i]);
        }
    }
}

//=============================Selection des meilleurs individus ===============================
vector<Individu> selection(vector<Individu>& individus){
    std::sort(individus.begin(), individus.end());
    vector<Individu> chosenOnes;
    for(int i=individus.size()-1; i>individus.size()-11; i--){
        chosenOnes.push_back(individus[i]);
    }
    return chosenOnes;
}

//=============================Hérédité et mutations ==========================
void mutation(Individu individus){
    if(std::rand()%100<1){
        individus.setKingWeight(rand()%1000/1000.);
    }
    if(std::rand()%100<1){
        individus.setManWeight(rand()%1000/1000.);
    }
}

void heredity(vector<Individu>& individus, vector<Individu> chosenOnes){
    individus.clear();
    float maxScore;
    float dices;
    for(int i=0; i<chosenOnes.size();i++){
        maxScore += float(chosenOnes[i].getScore());
        }
    vector<float> proba;
    proba.push_back(chosenOnes[0].getScore()/maxScore);
    for(int i=1;i<chosenOnes.size();i++){
        proba.push_back(chosenOnes[i].getScore()+proba[i-1]);
    }

    for(int k=0; k<100; k++){
        dices = float(std::rand()%1000)/1000;
        int compteur = 0;
        bool trouve = false;
        if(proba[0]>=dices){
            trouve=true;
        }
        while(compteur<chosenOnes.size() && !trouve){
            compteur+=1;
            if(proba[compteur]>=dices){
                trouve=true;
            }
        }
        individus.push_back(chosenOnes[compteur]);
        mutation(individus[k]);
    }
}





//===============================Execution===============================
int main(){


    vector<Individu> individus;
    vector<Individu> chosenOnes;
    for(int i=0; i<50; i++){
        individus.push_back(Individu(0,std::rand()%1000/1000.,std::rand()%1000/1000.,2,0,0,0));
    }
    for(int i=0; i<1;i++){
        cout << i << endl;
        evaluation(individus);
        chosenOnes = selection(individus);
        heredity(individus,chosenOnes);
        chosenOnes.clear();
    }
    std::sort(individus.begin(),individus.end());
    cout << "KingWeight : " <<individus.begin()->getKingWeight() << "ManWeight : "<<individus.begin()->getManWeight() << endl;


}
