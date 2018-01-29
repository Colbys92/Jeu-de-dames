#include "arbre.h"
#include <queue>
#include <algorithm>

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
    float getKingWeight() {return kingWeight;}
    int getDepth(){return depth;}
    float getNbMoveWeight(){return nbMoveWeight;}
    float getAdvancementForwardWeight(){return advancementForwardWeight;}
    float getCentralWeight(){return centralWeight;}
};

Individu::Individu(int score1, float manWeight1, float kingWeight1, int depth1, float nbMoveWeight1, float advancementForwardWeight1, float centralWeight1){
    score = score1;
    manWeight=manWeight1;
    kingWeight=kingWeight1;
    depth=depth1;
    nbMoveWeight=nbMoveWeight1;
    advancementForwardWeight=advancementForwardWeight1;
    centralWeight=centralWeight1;
}


int match(Individu i1, Individu i2){
    // Renvoie 1 si le premier individu gagne; 2 si le second; 0 si personne ne gagne.
    Board B;
    Move m1,m2;
    int compteur =0;
    while(compteur<200){
        cout << compteur << endl;

        if(B.playableMoves("white").size()>0){

            m1=B.bestMoveAlphaBeta("white",i1.getDepth(),i1.getManWeight(),i1.getKingWeight(),i1.getNbMoveWeight(),i1.getAdvancementForwardWeight(),i1.getCentralWeight());

            B.playMove(m1);

        }
        else{
            return 2;
        }
        if(B.playableMoves("black").size()>0){

            m2=B.bestMoveAlphaBeta("black", i2.getDepth(),i2.getManWeight(),i2.getKingWeight(),i2.getNbMoveWeight(),i2.getAdvancementForwardWeight(),i2.getCentralWeight());

            B.playMove(m2);

        }
        else{
            return 1;
        }
        compteur+=1;
    }
    return 0;
}



int main(){
    Individu i1(0,5,20,2,1,1,1);
    Individu i2(0,5,20,2,1,1,1);
    cout << match(i1,i2) << endl;

    return 0;
}
