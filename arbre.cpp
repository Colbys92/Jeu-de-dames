#include "arbre.h"

void King::killFreeMove(Board& B, vector<Move>& possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }
    bool isPiece;
    int currentPosition;
    int currentPositionValue;

    // Diagonale NW :
    currentPosition = position;
    currentPositionValue= positionValue;
    isPiece = B.isPieceHere(currentPosition+NW+currentPositionValue);
    while (!isPiece && (currentPosition%10)!=5 && currentPosition>4){
        possibleMoves.push_back(Move(position,currentPosition+NW+currentPositionValue,0));
        currentPosition = currentPosition+NW+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2; // Changement de ligne de position
        isPiece = B.isPieceHere(currentPosition+NW+currentPositionValue);
    }

    // Diagonale NE :
    currentPosition = position;
    currentPositionValue= positionValue;
    isPiece = B.isPieceHere(currentPosition+NE+currentPositionValue);
    while (!isPiece && (currentPosition%10)!=4 && currentPosition>4){
        possibleMoves.push_back(Move(position,currentPosition+NE+currentPositionValue,0));
        currentPosition = currentPosition+NE+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2; // Changement de ligne de position
        isPiece = B.isPieceHere(currentPosition+NE+currentPositionValue);
    }

    // Diagonale SE :
    currentPosition = position;
    currentPositionValue= positionValue;
    isPiece = B.isPieceHere(currentPosition+SE+currentPositionValue);
    while (!isPiece && (currentPosition%10)!=4 && currentPosition<45){
        possibleMoves.push_back(Move(position,currentPosition+SE+currentPositionValue,0));
        currentPosition = currentPosition+SE+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2; // Changement de ligne de position
        isPiece = B.isPieceHere(currentPosition+SE+currentPositionValue);
    }

    // Diagonale SW :
    currentPosition = position;
    currentPositionValue= positionValue;
    isPiece = B.isPieceHere(currentPosition+SW+currentPositionValue);
    while (!isPiece && (currentPosition%10)!=5 && currentPosition<45){
        possibleMoves.push_back(Move(position,currentPosition+SW+currentPositionValue,0));
        currentPosition = currentPosition+SW+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2; // Changement de ligne de position
        isPiece = B.isPieceHere(currentPosition+SW+currentPositionValue);
    }
}


void King::killingMove(Board &B, vector<Move> &possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }
    bool isPiece;
    bool isSecondPiece;
    int currentPosition;
    int currentPositionValue;

    //Diagonale NW :
    currentPosition = position+NW+positionValue;
    currentPositionValue= (positionValue+1)%2;
    isPiece = false;
    while ((currentPosition%10)!=5 && currentPosition>4 && !isPiece){
        isPiece = B.isPieceHere(currentPosition);
        currentPosition = currentPosition+NW+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2;
    }

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+SE+positionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
        isSecondPiece = B.isPieceHere(currentPosition);
        while((currentPosition%10)!=5 && currentPosition>4 && !isSecondPiece){
            possibleMoves.push_back(Move(position,currentPosition,1));
            currentPosition = currentPosition+NW+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2;
            isSecondPiece = B.isPieceHere(currentPosition);
        }
    }

    //Diagonale NE :
    currentPosition = position+NE+positionValue;
    currentPositionValue= (positionValue+1)%2;
    isPiece = false;
    while ((currentPosition%10)!=4 && currentPosition>4 && !isPiece){
        isPiece = B.isPieceHere(currentPosition);
        currentPosition = currentPosition+NE+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2;
    }

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+SW+positionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
        isSecondPiece = B.isPieceHere(currentPosition);
        while((currentPosition%10)!=4 && currentPosition>4 && !isSecondPiece){
            possibleMoves.push_back(Move(position,currentPosition,1));
            currentPosition = currentPosition+NE+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2;
            isSecondPiece = B.isPieceHere(currentPosition);
        }
    }

    //Diagonale SW :
    currentPosition = position+SW+positionValue;
    currentPositionValue= (positionValue+1)%2;
    isPiece = false;
    while ((currentPosition%10)!=5 && currentPosition<45 && !isPiece){
        isPiece = B.isPieceHere(currentPosition);
        currentPosition = currentPosition+SW+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2;
    }

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+NE+positionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
        isSecondPiece = B.isPieceHere(currentPosition);
        while((currentPosition%10)!=5 && currentPosition<45 && !isSecondPiece){
            possibleMoves.push_back(Move(position,currentPosition,1));
            currentPosition = currentPosition+SW+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2;
            isSecondPiece = B.isPieceHere(currentPosition);
        }
    }

    //Diagonale SE :
    currentPosition = position+SE+positionValue;
    currentPositionValue= (positionValue+1)%2;
    isPiece = false;
    while ((currentPosition%10)!=4 && currentPosition<45 && !isPiece){
        isPiece = B.isPieceHere(currentPosition);
        currentPosition = currentPosition+SE+currentPositionValue;
        currentPositionValue= (currentPositionValue+1)%2;
    }

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+NW+positionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
        isSecondPiece = B.isPieceHere(currentPosition);
        while((currentPosition%10)!=5 && currentPosition<45 && !isSecondPiece){
            possibleMoves.push_back(Move(position,currentPosition,1));
            currentPosition = currentPosition+SE+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2;
            isSecondPiece = B.isPieceHere(currentPosition);
        }
    }
}





void Man::killFreeMove(Board& B,vector<Move>& possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }
    if(color == "white"){
        if(position%10!=4 && !B.isPieceHere(position-5+positionValue)){
            possibleMoves.push_back(Move(position,position-5+positionValue,0));
        }
        if(!B.isPieceHere(position-5+positionValue)){
            possibleMoves.push_back(Move(position,position-6+positionValue,0));
        }
    }
    if(color == "black"){
        if(position%10!=4 && !B.isPieceHere(position+5+positionValue)){
            possibleMoves.push_back(Move(position,position+5+positionValue,0));
        }
        if(!B.isPieceHere(position+4+positionValue)){
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
            && B.isPieceHere(position-5+positionValue)
            && !B.isPieceHere(position-9)
            && B.getPiece(B.index_man_here(position-5+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position-9,1));

    }
    // En haut à gauche
    if(position%10!=5 && position%10!=0
            && B.isPieceHere(position-6+positionValue)
            && !B.isPieceHere(position-11)
            && B.getPiece(B.index_man_here(position-6+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position-11,1));

    }
    // En bas à droite
    if(position%10!=4 && position%10!=9
            && B.isPieceHere(position+5+positionValue)
            && !B.isPieceHere(position+11)
            && B.getPiece(B.index_man_here(position+5+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+11,1));

    }
    // En bas à gauche
    if(position%10!=5 && position%10!=0
            && B.isPieceHere(position+4+positionValue)
            && !B.isPieceHere(position+9)
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
