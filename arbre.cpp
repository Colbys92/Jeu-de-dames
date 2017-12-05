#include "arbre.h"


void Man::killFreeMove(Board& B,vector<Move>& possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }
    if(color == "white"){
        if(position%10!=4 && !B.isManHere(position-5+positionValue)){
            possibleMoves.push_back(Move(position,position-5+positionValue,0));
        }
        if(!B.isManHere(position-5+positionValue)){
            possibleMoves.push_back(Move(position,position-6+positionValue,0));
        }
    }
    if(color == "black"){
        if(position%10!=4 && !B.isManHere(position+5+positionValue)){
            possibleMoves.push_back(Move(position,position+5+positionValue,0));
        }
        if(!B.isManHere(position+4+positionValue)){
            possibleMoves.push_back(Move(position,position+4+positionValue,0));
        }
    }
}

void Man::killingMove(Board& B, vector<Move>& possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }


    // En haut à droite
    if(position%10!=4 && position%10!=9
            && B.isManHere(position-5+positionValue)
            && !B.isManHere(position-9)
            && B.getPiece(B.index_man_here(position-5+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position-9,1));

    }
    // En haut à gauche
    if(position%10!=5 && position%10!=0
            && B.isManHere(position-6+positionValue)
            && !B.isManHere(position-11)
            && B.getPiece(B.index_man_here(position-6+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position-11,1));

    }
    // En bas à droite
    if(position%10!=4 && position%10!=9
            && B.isManHere(position+5+positionValue)
            && !B.isManHere(position+11)
            && B.getPiece(B.index_man_here(position+5+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+11,1));

    }
    // En bas à gauche
    if(position%10!=5 && position%10!=0
            && B.isManHere(position+4+positionValue)
            && !B.isManHere(position+9)
            && B.getPiece(B.index_man_here(position+4+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+9,1));
    }
}

int main(){

    Board* Plateau = new Board;
    vector<Move> PossibleMoves;

    Plateau->getPiece(0)->setPosition(26);
    Plateau->getPiece(1)->setPosition(27);
//    Plateau->getPiece(0)->killFreeMove(*Plateau,PossibleMoves);
//    for(std::vector<Move>::iterator it = PossibleMoves.begin(); it<PossibleMoves.end();it++){
//        cout << (*it).start << " " << (*it).arrival<< endl;
//    }

    Plateau->getPiece(21)->killingMove(*Plateau,PossibleMoves);
    for(std::vector<Move>::iterator it = PossibleMoves.begin(); it<PossibleMoves.end();it++){
        cout << (*it).getStart() << " " << (*it).getArrival()<< endl;
    }




    return 0;
}
