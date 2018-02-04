#include "algogenetique.h"
#include <iostream>
#include <ctime>
//===========================Les classes d'individus==================



Individu::operator<(Individu i2){
    return(score<i2.getScore());
}
Individu::operator=(const Individu& i){
    score = i.score;
    depth=i.depth;
    manWeight=i.manWeight;
    kingWeight=i.kingWeight;
    nbMoveWeight=i.nbMoveWeight;
    centralWeight=i.centralWeight;
    advancementForwardWeight=i.advancementForwardWeight;
}
Individu::addToScore(int toAdd){
    score+=toAdd;
}
PowerfulIndividu::addToScore(int toAdd){
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
//==================================Tools===========================================


vector<Individu> genIndividu(int size){
    vector<Individu> individus;
    for(int i=0; i<size; i++){
        individus.push_back(Individu(0,std::rand()%1000/1000.,std::rand()%1000/1000.,2,std::rand()%1000/1000.,std::rand()%1000/1000.,std::rand()%1000/1000.));
    }
    return individus;
}


vector<PowerfulIndividu> genPowerfulIndividu(int size){
    vector<PowerfulIndividu> powerfulIndividus;
    vector<Individu> individus;
    individus = genIndividu(3*size);
    for(int i=0; i<size; i++){
        powerfulIndividus.push_back(PowerfulIndividu(individus[3*i],individus[3*i+1],individus[3*i+2]));
    }
    return powerfulIndividus;
}

//==================================Retrouver la valeur d'une fonction donnée========================
int matchCout(std::queue<Move>& game, Individu i1, Individu i2){
    Board B;
    Move m1,m2;
    float val;
    int compteur =0;
    while(compteur<100){
        if(B.playableMoves("white").size()>0){
            m1=B.bestMoveAlphaBeta2("white",i1.getDepth(),i1.getManWeight(),i1.getKingWeight(),i1.getNbMoveWeight(),i1.getCentralWeight(),i1.getAdvancementForwardWeight(),true,-10000,10000).second;
            B.playMove(m1);
            game.push(m1);
        }
        else{
            cout << "2 gagne" << endl;
            return 2;
        }
        if(B.playableMoves("black").size()>0){
            m2=B.bestMoveAlphaBeta2("black", i2.getDepth(), i2.getManWeight(),i2.getKingWeight(),i1.getNbMoveWeight(),i1.getCentralWeight(),i1.getAdvancementForwardWeight(),true, -10000,10000).second;
            B.playMove(m2);
            game.push(m2);
        }
        else{
            cout << "1 gagne" << endl;
            return 1;
        }
        compteur+=1;
    }
    return 0;
}



vector<Individu> genSample(Individu individu, int accuracy){
    if (accuracy==1){
        individu = Individu(0,0.5,0.5,4,0,0.5,0.5);
    }
    vector<Individu> sample;
    for(int i=0;i<10;i++){
        for(int j=0; j<10; j++){
            for(int k=0; k<10; k++){
                for(int l=0 ; l<10; l++){
                    sample.push_back(Individu(0,individu.getManWeight()+(5-i)/10/accuracy,individu.getKingWeight()+(5-j)/10/accuracy, \
                                              2,0,individu.getAdvancementForwardWeight()+(5-k)/10/accuracy,individu.getCentralWeight()+(5-l)/10/accuracy));
                }
            }
        }
    }
    return sample;
}



void rankIndividu2(std::queue<Move> game, vector<Individu>& sampleTest, string color){
    Board B;
    Move m1,m2;
    while(game.size()>0){
        m1 = game.front();
        if(color=="white"){
            for(vector<Individu>::iterator it=sampleTest.begin(); it!=sampleTest.end(); it++){
                if((*it).bestMoveAlphaBeta(B,color,-1000,1000)==m1){ (*it).addToScore(1);}
            }
        }
        B.playMove(game.front());
        game.pop();
        m2=game.front();
        if(color=="black"){
            for(vector<Individu>::iterator it=sampleTest.begin(); it!=sampleTest.end(); it++){
                if((*it).bestMoveAlphaBeta(B,color,-1000,1000)==m2){ (*it).addToScore(1);}
            }
        }
        B.playMove(m2);
        game.pop();
    }
}

Individu compatibleIndividu(std::queue<Move> game, string color){
    int accuracy = 1;
    int compteur =0;
    Individu approximateIndividu(0,0,0,0,0,0,0);
    while(compteur<1){
        vector<Individu> sampleTest = genSample(approximateIndividu,accuracy);
        rankIndividu2(game,sampleTest,color);
        sort(sampleTest.begin(),sampleTest.end());
        approximateIndividu = sampleTest[sampleTest.size()-1];
        sampleTest.clear();
        accuracy=accuracy*10;
        compteur+=1;
    }
    return approximateIndividu;
}



//==================================Méthode pour l'évaluation individu simple========================

int match(Individu i1, Individu i2){
    // Renvoie 1 si le premier individu gagne; 2 si le second; 0 si personne ne gagne.
    Board B;
    Move m1,m2;
    float val;
    int compteur =0;
    while(compteur<200){
        if(B.playableMoves("white").size()>0){
            m1=B.bestMoveAlphaBeta("white",i1.getDepth(),i1.getManWeight(),i1.getKingWeight(),true,-10000,10000).second;
            B.playMove(m1);
        }
        else{
            cout << "2 gagne" << endl;
            return 2;
        }
        if(B.playableMoves("black").size()>0){
            m2=B.bestMoveAlphaBeta("black", i2.getDepth(), i2.getManWeight(),i2.getKingWeight(),true, -10000,10000).second;
            B.playMove(m2);
        }
        else{
            cout << "1 gagne" << endl;
            return 1;
        }
        compteur+=1;
    }
    return 0;
}
int match2(Individu i1, Individu i2){
    // Renvoie 1 si le premier individu gagne; 2 si le second; 0 si personne ne gagne.
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
            return 2;
        }
        if(B.playableMoves("black").size()>0){
            m2=B.bestMoveAlphaBeta2("black", i2.getDepth(), i2.getManWeight(),i2.getKingWeight(),i1.getNbMoveWeight(),i1.getCentralWeight(),i1.getAdvancementForwardWeight(),true, -10000,10000).second;
            B.playMove(m2);
        }
        else{
            cout << "1 gagne" << endl;
            return 1;
        }
        compteur+=1;
    }
    return 0;
}
int matchPowerfulIndividu(PowerfulIndividu i1, PowerfulIndividu i2){
    // Renvoie 1 si le premier individu gagne; 2 si le second; 0 si personne ne gagne.

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
    // Si un individu gagne, on ajoute 2 points à son score, si il y a match nul on ajoute 1, 0 si il perd.
    //int results = match1(i1,i2);
    int results = match2(i1,i2);
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
    // Si un individu gagne, on ajoute 2 points à son score, si il y a match nul on ajoute 1, 0 si il perd.
    //int results = match1(i1,i2);
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
            cout<<"("<<i<<","<<j<<")"<<endl;
            resultsMatchPowerfulIndividu(individus[i],individus[j]);
            resultsMatchPowerfulIndividu(individus[j],individus[i]);
            cout << "end " << endl;
        }
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
vector<Individu> selection(vector<Individu>& individus, int numberChosen){
    std::sort(individus.begin(), individus.end());
    vector<Individu> chosenOnes;
    for(int i=individus.size()-1; i>individus.size()-numberChosen-1; i--){
        chosenOnes.push_back(individus[i]);
    }
    return chosenOnes;
}

vector<PowerfulIndividu> selectionPowerful(vector<PowerfulIndividu>& individus, int numberChosen){
    std::sort(individus.begin(), individus.end());
    vector<PowerfulIndividu> chosenOnes;
    for(int i=individus.size()-1; i>individus.size()-numberChosen-1; i--){
        chosenOnes.push_back(individus[i]);
    }
    return chosenOnes;
}

//=============================Hérédité et mutations ==========================
void mutation(Individu individus){
    if((std::rand()%100)<10){
        individus.setKingWeight(std::rand()%1000/1000.);
    }
    if(std::rand()%100<10){
        individus.setManWeight(std::rand()%1000/1000.);
    }
}

void mutation2(Individu individu, int proba){
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
    mutation2(Ibegin,proba);
    mutation2(Imiddle,proba);
    mutation2(Iend,proba);
}
void heredity2(vector<Individu>& individus, vector<Individu> chosenOnes){
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
        mutation2(individus[k],10);
    }
}
void heredityPowerful(vector<PowerfulIndividu>& individus, vector<PowerfulIndividu> chosenOnes){
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
void heredity(vector<Individu>& individus, vector<Individu> chosenOnes){
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
        individus.push_back(chosenOnes.at(compteur));
        mutation(individus[k]);
    }
}





//===============================Execution===============================
int main(){

    //=====================Algo Génétique======================
    //    Individu i1(0,2,5,4,0,0,0);
    //    Individu i2(0,5,20,4,0.1,1,1);
    //    cout << match2(i1,i2) << endl;
    //    vector<Individu> individus;
    //    vector<Individu> chosenOnes;
    //    for(int i=0; i<20; i++){
    //        individus.push_back(Individu(0,std::rand()%1000/1000.,std::rand()%1000/1000.,4,std::rand()%1000/1000.,std::rand()%1000/1000.,std::rand()%1000/1000.));
    //    }

    //    for(int i=0; i<4;i++){
    //        cout << i << endl;
    //        evaluation(individus);
    //        chosenOnes = selection(individus,10);

    //        heredity2(individus,chosenOnes);
    //        chosenOnes.clear();
    //    }
    //    std::sort(individus.begin(),individus.end());
    //    cout << "KingWeight : " <<individus.begin()->getKingWeight() << "ManWeight : "<<individus.begin()->getManWeight() << endl;
    //    srand(time(NULL));
    //    vector<PowerfulIndividu> individus;
    //    vector<PowerfulIndividu> chosenOnes;
    //    individus=genPowerfulIndividu(70);
    //    for(int i=0; i<5; i++){
    //        cout << i << endl;
    //        evaluationPowerfulIndividu(individus);
    //        chosenOnes = selectionPowerful(individus,5);
    //        heredityPowerful(individus,chosenOnes);
    //        chosenOnes.clear();
    //    }
    //    evaluationPowerfulIndividu(individus);
    //    std::sort(individus.begin(),individus.end());
    //    PowerfulIndividu i = individus[individus.size()-1];
    //    Individu i1 = i.getIbegin();
    //    Individu i2 = i.getImiddle();
    //    Individu i3 = i.getIend();
    //    cout << "Man Weight : " << i1.getManWeight() << "King Weight : " << i1.getKingWeight() <<  "Advance Forward Weight" << i1.getAdvancementForwardWeight() \
    //           << "Depth : " << i1.getDepth() << " Central Weight : " << i1.getCentralWeight() << " Number of possible move weight " << i1.getNbMoveWeight() << endl;
    //    cout << "Man Weight : " << i2.getManWeight() << "King Weight : " << i2.getKingWeight() <<  "Advance Forward Weight" << i2.getAdvancementForwardWeight() \
    //           << "Depth : " << i2.getDepth() << " Central Weight : " << i2.getCentralWeight() << " Number of possible move weight " << i2.getNbMoveWeight()<< endl;
    //    cout << "Man Weight : " << i3.getManWeight() << "King Weight : " << i3.getKingWeight() <<  "Advance Forward Weight" << i3.getAdvancementForwardWeight() \
    //           << "Depth : " << i3.getDepth() << " Central Weight : " << i3.getCentralWeight() << " Number of possible move weight " << i3.getNbMoveWeight()<< endl;
    //================Test individu==================
    //Individu i1(0,0.756,0.313,4,0.945,0.832,0.808);
    //Individu i2(0,0.512,0.441,4,0.549,0.434,0.555);
    //Individu i3(0,0.139,0.753,4,0.718,0.06,0.145);

    ////    Individu i1(0,1,0.313,4,0.945,0.832,0.808);
    ////    Individu i2(0,1,0.441,4,0.549,0.434,0.555);
    ////    Individu i3(0,0.5,1,4,0.718,0,0.5);
    //    PowerfulIndividu iref(i1,i2,i3);
    //    Individu i4(0,0.388,0.1,4,0.146,0.400,0.044);
    //    Individu i5(0,0.407,0.09,4,0.6,0.6,0.016);
    //    Individu i6(0,0.643,0.681,4,0.142,0.4,0.3);
    //    PowerfulIndividu ifight(i4,i5,i6);

    //    cout << matchPowerfulIndividu(iref,ifight) << endl;
    //    cout << matchPowerfulIndividu(ifight,iref)<<endl;


    Individu itest(0,1,5,2,0,0,0);
    Individu i2(0,1,0,2,0,0,0);

    std::queue<Move> game;
    cout << matchCout(game,itest,i2);
    Individu i1= compatibleIndividu(game,"white");
    cout << "Man Weight : " << i1.getManWeight() << "King Weight : " << i1.getKingWeight() <<  "Advance Forward Weight" << i1.getAdvancementForwardWeight() \
         << "Depth : " << i1.getDepth() << " Central Weight : " << i1.getCentralWeight() << " Number of possible move weight " << i1.getNbMoveWeight() << endl;

}
