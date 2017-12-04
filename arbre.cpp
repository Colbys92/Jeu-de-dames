#include "arbre.h"




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
        cout << (*it).start << " " << (*it).arrival<< endl;
    }




    return 0;
}
