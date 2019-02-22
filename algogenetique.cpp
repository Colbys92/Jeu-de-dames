#include "algogenetique.h"
#include <iostream>
#include <ctime>
//===========================Individu's methods==================



bool Individu::operator<(const Individu i2) const{
    return(score<i2.getScore());
}

bool PowerfulIndividu::operator<(const PowerfulIndividu i) const {
    return(score<i.getScore());
}


void Individu::operator=(const Individu& i){
    score = i.score;
    depth=i.depth;
    manWeight=i.manWeight;
    kingWeight=i.kingWeight;
    nbMoveWeight=i.nbMoveWeight;
    centralWeight=i.centralWeight;
    advancementForwardWeight=i.advancementForwardWeight;
}

void Individu::addToScore(int toAdd){
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

Move Individu::bestMoveAlphaBeta(Board & B,string color,float alpha, float beta ){
    return B.bestMoveAlphaBeta2(color,getDepth(),getManWeight(),getKingWeight(),getNbMoveWeight(),getCentralWeight(),getAdvancementForwardWeight(),true,alpha,beta).second;
}
//=========================PowerfulIndividu's methods==========================


Move PowerfulIndividu::bestMoveAlphaBeta(Board &B, string color, float alpha, float beta){
    if(B.timeMatch()==2){
        return Iend.bestMoveAlphaBeta(B,color,alpha,beta);
    }
    else if(B.timeMatch()==1){
        return Imiddle.bestMoveAlphaBeta(B,color,alpha,beta);
    }
    else{
        return Ibegin.bestMoveAlphaBeta(B,color,alpha,beta);
    }
}

void PowerfulIndividu::addToScore(int toAdd){
    score+=toAdd;
}


//==================================Random Generation===========================================


vector<Individu> genIndividu(int size){
    // Generation of random individuals :
    vector<Individu> individus;
    for(int i=0; i<size; i++){
        individus.push_back(Individu(0,std::rand()%1000/1000.,std::rand()%1000/1000.,2,std::rand()%1000/1000.,std::rand()%1000/1000.,std::rand()%1000/1000.));
    }
    return individus;
}


vector<PowerfulIndividu> genPowerfulIndividu(int size){
    // Generation of random powerful individuals :
    vector<PowerfulIndividu> powerfulIndividus;
    vector<Individu> individus;
    individus = genIndividu(3*size);
    for(int i=0; i<size; i++){
        powerfulIndividus.push_back(PowerfulIndividu(individus[3*i],individus[3*i+1],individus[3*i+2]));
    }
    return powerfulIndividus;
}


//==================================Evaluation========================

int match(Individu i1, Individu i2){
    // Return 1 if the first individual wins, 2 if the second wins and 0 if even
    Board B;
    Move m1,m2;
    float val;
    int compteur =0;
    while(compteur<100){
        if(B.playableMoves("white").size()>0){
            m1=B.bestMoveAlphaBeta2("white",i1.getDepth(),i1.getManWeight(),i1.getKingWeight(),i1.getNbMoveWeight(),i1.getCentralWeight(),i1.getAdvancementForwardWeight(),true,-10000,10000).second;
            B.playMove(m1);
        }
        else{
            cout << "2 gagne" << endl;
            return compteur;
        }
        if(B.playableMoves("black").size()>0){
            m2=B.bestMoveAlphaBeta2("black", i2.getDepth(), i2.getManWeight(),i2.getKingWeight(),i1.getNbMoveWeight(),i1.getCentralWeight(),i1.getAdvancementForwardWeight(),true, -10000,10000).second;
            B.playMove(m2);
        }
        else{
            cout << "1 gagne" << endl;
            return compteur;
        }
        compteur+=1;
    }
    return compteur;
}

int matchPowerfulIndividu(PowerfulIndividu i1, PowerfulIndividu i2){
    // Return 1 if the first individual wins, 2 if the second wins and 0 if even
    Board B;
    Move m1,m2;
    int compteur =0;
    while(compteur<75){
        if(B.playableMoves("white").size()>0){
            m1=i1.bestMoveAlphaBeta(B,"white",-1000,1000);
            B.playMove(m1);
        }
        else{
            return 2;
        }
        if(B.playableMoves("black").size()>0){
            m2=i2.bestMoveAlphaBeta(B,"black",-1000,1000);
            B.playMove(m2);
        }
        else{
            return 1;
        }
        compteur+=1;
    }

    return 0;
}

void resultsMatch(Individu& i1, Individu& i2){
    // If an individual win, 2 is added to its score, if the game is even, 1 is added to its score :
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

void resultsMatchPowerfulIndividu(PowerfulIndividu i1, PowerfulIndividu i2){
    // If an individual win, 2 is added to its score, if the game is even, 1 is added to its score :
    int results = matchPowerfulIndividu(i1,i2);
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

int evaluationPowerfulIndividu(vector<PowerfulIndividu>& individus){
    for(int i=0; i< individus.size(); i++){
        for(int j=i+1; j<individus.size(); j++){
            resultsMatchPowerfulIndividu(individus[i],individus[j]);
            resultsMatchPowerfulIndividu(individus[j],individus[i]);
        }
    }
}

int evaluation(vector<Individu>& individus){
    for(int i=0; i< individus.size(); i++){
        for(int j=i+1; j<individus.size(); j++){
            resultsMatch(individus[i],individus[j]);
            resultsMatch(individus[j],individus[i]);
        }
    }
}

//=============================Best individual selection ===============================

vector<Individu> selection(vector<Individu>& individus, int numberChosen){
    // Selection of the best PowerfulIndividu
    std::sort(individus.begin(), individus.end());
    vector<Individu> chosenOnes;
    for(int i=individus.size()-1; i>individus.size()-numberChosen-1; i--){
        chosenOnes.push_back(individus[i]);
    }
    return chosenOnes;
}

vector<PowerfulIndividu> selectionPowerful(vector<PowerfulIndividu>& individus, int numberChosen){
    // Selection of the best PowerfulIndividu
    std::sort(individus.begin(), individus.end());
    vector<PowerfulIndividu> chosenOnes;
    for(int i=individus.size()-1; i>individus.size()-numberChosen-1; i--){
        chosenOnes.push_back(individus[i]);
    }
    return chosenOnes;
}

//============================= Mutations ==========================

void mutation(Individu individu, int proba){
    if((std::rand()%100)<proba){
        individu.setKingWeight(std::rand()%1000/1000.);
    }
    if(std::rand()%100<proba){
        individu.setManWeight(std::rand()%1000/1000.);
    }
    if(std::rand()%100<proba){
        individu.setNbMoveWeight(std::rand()%1000/1000.);
    }
    if((std::rand()%100)<proba){
        individu.setAdvancementForwardWeight(std::rand()%1000/1000.);
    }
    if(std::rand()%100<proba){
        individu.setCentralWeight(std::rand()%1000/1000.);
    }
}

void PowerfulIndividu::mutationPowerful(int proba){
    mutation(Ibegin,proba);
    mutation(Imiddle,proba);
    mutation(Iend,proba);
}

//=======================Heredity==================================
void heredity(vector<Individu>& individus, vector<Individu> chosenOnes){
    // Passing the genetic legacy through generation for Individu
    int size =individus.size();
    individus.clear();
    float maxScore;
    float dices;
    for(int i=0; i<chosenOnes.size();i++){
        maxScore += float(chosenOnes[i].getScore());
    }
    vector<float> proba;
    proba.push_back(chosenOnes[0].getScore()/maxScore);
    for(int i=1;i<chosenOnes.size()-1;i++){
        proba.push_back(chosenOnes[i].getScore()/maxScore+proba[i-1]);
    }
    proba.push_back(1.);

    for(int k=0; k<size; k++){
        dices = float(std::rand()%1000)/1000.;
        int compteur = 0;
        bool trouve = false;
        if(proba[0]>=dices){
            trouve=true;
        }
        while(compteur<chosenOnes.size()-1 && !trouve){
            compteur+=1;
            if(proba[compteur]>=dices){
                trouve=true;
            }
        }
        compteur = compteur%10;
        individus.push_back(chosenOnes.at(compteur));
        mutation(individus[k],10);
    }
}

void heredityPowerful(vector<PowerfulIndividu>& individus, vector<PowerfulIndividu> chosenOnes){
    // Passing the genetic legacy through generation for PowerIndividu
    int size =individus.size();
    individus.clear();
    float maxScore;
    float dices1,dices2,dices3;
    for(int i=0; i<chosenOnes.size();i++){
        maxScore += float(chosenOnes[i].getScore());
    }
    vector<float> proba;
    proba.push_back(chosenOnes[0].getScore()/maxScore);
    for(int i=1;i<chosenOnes.size()-1;i++){
        proba.push_back(chosenOnes[i].getScore()/maxScore+proba[i-1]);
    }
    proba.push_back(1.);

    for(int k=0; k<size; k++){
        dices1 = float(std::rand()%1000)/1000.;
        dices2 = float(std::rand()%1000)/1000.;
        dices3 = float(std::rand()%1000)/1000.;

        int compteur1 = 0;
        int compteur2 = 0;
        int compteur3 =0;
        bool trouve1 = false;
        bool trouve2 =false;
        bool trouve3 = false;
        if(proba[0]>=dices1){
            trouve1=true;
        }
        if(proba[0]>=dices2){
            trouve2=true;
        }
        if(proba[0]>=dices3){
            trouve3=true;
        }
        while(compteur1<chosenOnes.size()-1 && !trouve1){
            compteur1+=1;
            if(proba[compteur1]>=dices1){
                trouve1=true;
            }
        }
        while(compteur2<chosenOnes.size()-1 && !trouve2){
            compteur2+=1;
            if(proba[compteur1]>=dices2){
                trouve2=true;
            }
        }
        while(compteur3<chosenOnes.size()-1 && !trouve3){
            compteur3+=1;
            if(proba[compteur3]>=dices3){
                trouve3=true;
            }
        }
        individus.push_back(PowerfulIndividu(chosenOnes.at(compteur1).getIbegin(),chosenOnes.at(compteur2).getImiddle(),chosenOnes.at(compteur3).getIend()));
        individus[k].mutationPowerful(10);
    }
}






//=====================Execution of the Genetic Algorithm======================

int main(){


        srand(time(NULL));
        vector<PowerfulIndividu> individus;
        vector<PowerfulIndividu> chosenOnes;
        individus=genPowerfulIndividu(50);
        for(int i=0; i<8; i++){
            cout << i << endl;
            evaluationPowerfulIndividu(individus);
            chosenOnes = selectionPowerful(individus,5);
            heredityPowerful(individus,chosenOnes);
            chosenOnes.clear();
        }
        evaluationPowerfulIndividu(individus);
        std::sort(individus.begin(),individus.end());
        PowerfulIndividu i = individus[individus.size()-1];
        Individu i1 = i.getIbegin();
        Individu i2 = i.getImiddle();
        Individu i3 = i.getIend();
        cout << "Man Weight : " << i1.getManWeight() << "King Weight : " << i1.getKingWeight() <<  "Advance Forward Weight" << i1.getAdvancementForwardWeight() \
               << "Depth : " << i1.getDepth() << " Central Weight : " << i1.getCentralWeight() << " Number of possible move weight " << i1.getNbMoveWeight() << endl;
        cout << "Man Weight : " << i2.getManWeight() << "King Weight : " << i2.getKingWeight() <<  "Advance Forward Weight" << i2.getAdvancementForwardWeight() \
               << "Depth : " << i2.getDepth() << " Central Weight : " << i2.getCentralWeight() << " Number of possible move weight " << i2.getNbMoveWeight()<< endl;
        cout << "Man Weight : " << i3.getManWeight() << "King Weight : " << i3.getKingWeight() <<  "Advance Forward Weight" << i3.getAdvancementForwardWeight() \
               << "Depth : " << i3.getDepth() << " Central Weight : " << i3.getCentralWeight() << " Number of possible move weight " << i3.getNbMoveWeight()<< endl;


}
