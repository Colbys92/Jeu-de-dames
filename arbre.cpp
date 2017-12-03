#include "arbre.h"




int main(){

    Board* Plateau = new Board;
    vector<Move> PossibleMoves;

    Plateau->getpiece(0).setPosition(26);
    Plateau->getpiece(1).setPosition(27);
    Plateau->getPiece(21).killFreeMove(Plateau,PossibleMoves);
    for(std::vector<Move>::iterator it = PossibleMoves.begin(); it<PossibleMoves.end();it++){
        cout << (*it).start << " " << (*it).arrival<< endl;
    }



    return 0;
}
