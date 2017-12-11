#include "arbre.h"
#include <queue>
#include <algorithm>

Move::Move(const Move &m){
    for(vector<int>::const_iterator it=m.path.begin(); it!=m.path.end(); it++){
        path.push_back(*it);
    }
    kills=m.kills;
}
Move Move::extendMove(Move m){
    if (getArrival()!=m.getStart()){
        throw(new invalid_argument("That move can't follow the first one"));
    }
    else {
        Move newMove(*this);
        newMove.kills++;
        newMove.path.push_back(m.getArrival());
        return newMove;
    }
}

void Move::operator=(const Move& m) {
    path.clear();
    for(vector<int>::const_iterator it=m.path.begin();it!=m.path.end();it++) {
        path.push_back(*it);
    }
    kills=m.kills;
}

bool Move::operator<(const Move& m) const {
    return kills<m.kills;
}

Board::Board(const Board &b){
    for(vector<Piece*>::const_iterator it=b.pieces.begin(); it!=b.pieces.end(); it++){
        pieces.push_back((*it)->clone());
    }
}



King::~King(){}
Piece* Man::clone(){
    return new Man(*this);
}
Piece* King::clone(){
    return new King(*this);
}

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

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+SE+currentPositionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
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

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+SW+currentPositionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
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

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+NE+currentPositionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
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

    if (isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+NW+currentPositionValue)))->Color() != color) ){ // Il faut récupérer la position précèdente
        isSecondPiece = B.isPieceHere(currentPosition);
        while((currentPosition%10)!=5 && currentPosition<45 && !isSecondPiece){
            possibleMoves.push_back(Move(position,currentPosition,1));
            currentPosition = currentPosition+SE+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2;
            isSecondPiece = B.isPieceHere(currentPosition);
        }
    }
}

void Man::select(Board& b, vector<Move> &possibleMoves){
    vector<Move> moveToAdd;
    vector<Move> complementaryMove;
    Move currentMove;
    Board virtualBoard(b);
    int size;
    this->killingMove(virtualBoard,moveToAdd);
    if(moveToAdd.size()==0){
        this->killFreeMove(virtualBoard,possibleMoves);
        return;
    }
    else {
        size = moveToAdd.size();
        while(size!=0){
            currentMove = moveToAdd.front();
            //We play virtually the current move on the board
            virtualBoard.playMove(currentMove);
            //We compute the follow up move to the current move in complementrayMove
            virtualBoard.getPiece(virtualBoard.index_man_here(currentMove.getArrival()))->killingMove(virtualBoard, complementaryMove);
            //if the current move can not be extended we addit to the list of possible moves
            if(complementaryMove.size()==0){
                possibleMoves.push_back(currentMove);
            }
            //We add to the movesToAdd the extended current move
            for(vector<Move>::iterator it=complementaryMove.begin();it!=complementaryMove.end();it++) {
                moveToAdd.push_back(currentMove.extendMove(*it));
            }
            complementaryMove.clear();
            //we can remove the current move to the list of moves to add
            moveToAdd.erase(moveToAdd.begin());
            //we reset the board to study the next move
            virtualBoard=b;
            size=moveToAdd.size();
        }

        sort(possibleMoves.begin(),possibleMoves.end());
        int maxKills=possibleMoves[possibleMoves.size()-1].getKills();
        int index;
        for(index=possibleMoves.size()-1;index>-1;index--) {
            if(possibleMoves[index].getKills()<maxKills)
                break;
        }
        possibleMoves.erase(possibleMoves.begin(),possibleMoves.begin()+index+1);
    }
}

void Man::killFreeMove(Board& B,vector<Move>& possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }
    if(color == "white"){
        //Deplacement NE :
        if(position%10!=4 && !B.isPieceHere(position+NE+positionValue)){
            possibleMoves.push_back(Move(position,position+NE+positionValue,0));
        }
        // Déplacement NW :
        if(position%10!=5 && !B.isPieceHere(position+NW+positionValue)){
            possibleMoves.push_back(Move(position,position+NW+positionValue,0));
        }
    }
    if(color == "black"){
        // Déplacement SE :
        if(position%10!=4 && !B.isPieceHere(position+SE+positionValue)){
            possibleMoves.push_back(Move(position,position+SE+positionValue,0));
        }
        // Déplacement SW :
        if(position%10!=5 && !B.isPieceHere(position+SW+positionValue)){
            possibleMoves.push_back(Move(position,position+SW+positionValue,0));
        }
    }
}

void Man::killingMove(Board& B, vector<Move>& possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }


    // Déplacement NE :
    if(position%10!=4 && position%10!=9
            && B.isPieceHere(position+NE+positionValue)
            && !B.isPieceHere(position+NE2)
            && B.getPiece(B.index_man_here(position+NE+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position-NE2,1));

    }
    // Déplacement NW :
    if(position%10!=5 && position%10!=0
            && B.isPieceHere(position+NW+positionValue)
            && !B.isPieceHere(position+NW2)
            && B.getPiece(B.index_man_here(position+NW+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+NW2,1));

    }
    // Déplacement SE :
    if(position%10!=4 && position%10!=9
            && B.isPieceHere(position+SE+positionValue)
            && !B.isPieceHere(position+11)
            && B.getPiece(B.index_man_here(position+5+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+11,1));

    }
    // Déplacement SW :
    if(position%10!=5 && position%10!=0
            && B.isPieceHere(position+4+positionValue)
            && !B.isPieceHere(position+9)
            && B.getPiece(B.index_man_here(position+4+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+9,1));
    }
}


void Board::operator=(const Board& b){
    pieces.clear();
    for(vector<Piece*>::const_iterator it=b.pieces.begin();it!=b.pieces.end();it++) {
        pieces.push_back((*it)->clone());
    }
}

void Board::killAt(int pos) {
    int index=index_man_here(pos);
    delete pieces[index];
    pieces.erase(pieces.begin()+index);
}

void Board::playMove(const Move &m) {
    //ca ne marche pas avec iterateur, pourquoi ?
//    for(int i=0;i<m.getPath().size()-1;i++) {
//        int depart=m.getPath()[i];
//        int arrivee=m.getPath()[i+1];
//        if(abs(depart-arrivee)>8) {
//            int positionValue=0;
//            if((depart)%10<5){
//                positionValue=1;
//            }
//            if(arrivee==depart+NW2) {
//                killAt(depart + NW+positionValue);
//            }
//            else if(arrivee==depart+NE2) {
//                killAt(depart + NE +positionValue);
//            }
//            else if(arrivee==depart+SE2) {
//                killAt(depart+SE+positionValue);
//            }
//            else if(arrivee==depart+SW2) {
//                killAt(depart+SW+positionValue);
//            }
//        }
//        pieces[index_man_here(depart)]->setPosition(arrivee);
//    }
    if(m.getKills()==0){
        pieces[index_man_here(m.getPath()[0])]->setPosition(m.getPath()[1]);

    }
    else {
        for(int i=0;i<m.getPath().size()-1;i++){
            int start = m.getPath()[i];
            int arrival = m.getPath()[i+1];
            //positionValue depuis le départ :
            int positionValue = 0;
            if(start%10<5){
                positionValue=1;
            }
            //South, North separation :
            if(arrival-start>0){ // South :
                int currentPositionWest = start;
                int currentPositionEst = start;
                int positionKilledWest = -1;
                int positionKilledEst = -1;


                while(currentPositionEst!=arrival && currentPositionWest!=arrival){// We move following both diagonals searching for a Piece and the arrival
                    // EST
                    if((currentPositionEst%10!=4)){
                        if(isPieceHere(currentPositionEst)){
                            positionKilledEst=currentPositionEst;
                        }
                        currentPositionEst=currentPositionEst+positionValue+SE;
                    }
                    //WEST :
                    if(currentPositionWest%10!=5){
                        if(isPieceHere(currentPositionWest)){
                            positionKilledWest=currentPositionWest;
                        }
                        currentPositionWest=currentPositionWest+positionValue+SW;
                    }
                    // Actualisation of position value :
                    positionValue=(positionValue+1)%2;
                }


                if(currentPositionEst==arrival){
                    if (positionKilledEst==-1){
                        cerr << "Erreur, pas de pion ennemi selon le déplacement selon la diagonale Sud Est" << endl;
                    }

                    else killAt(positionKilledEst);
                }
                else {
                    if(positionKilledWest==-1){
                        cerr << "Erreur, pas de pion ennemi selon le déplacement selon la diagonale SudWest" << endl;
                    }
                    else killAt(positionKilledWest);
                }
            }

            else{//North :
                int currentPositionWest = start;
                int currentPositionEst = start;
                int positionKilledWest = -1;
                int positionKilledEst = -1;


                while(currentPositionEst!=arrival && currentPositionWest!=arrival){// We move following both diagonals searching for a Piece and the arrival
                    // EST
                    if((currentPositionEst%10!=4)){
                        if(isPieceHere(currentPositionEst)){
                            positionKilledEst=currentPositionEst;
                        }
                        currentPositionEst=currentPositionEst+positionValue+NE;
                    }
                    //WEST :
                    if(currentPositionWest%10!=5){
                        if(isPieceHere(currentPositionWest)){
                            positionKilledWest=currentPositionWest;
                        }
                        currentPositionWest=currentPositionWest+positionValue+NW;
                    }
                    // Actualisation of position value :
                    positionValue=(positionValue+1)%2;
                }


                if(currentPositionEst==arrival){
                    if (positionKilledEst==-1){
                        cerr << "Erreur, pas de pion ennemi selon le déplacement selon la diagonale NordEst" << endl;
                    }

                    else killAt(positionKilledEst);
                }
                else {
                    if(positionKilledWest==-1){
                        cerr << "Erreur, pas de pion ennemi selon le déplacement selon la diagonale NordWest" << endl;
                    }
                    else killAt(positionKilledWest);
                }
            }
            pieces[index_man_here(m.getPath()[i])]->setPosition(m.getPath()[i+1]);
            if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color()=="white" && arrival<5){
                turnToKing(arrival);
            }
            if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color()=="black" && arrival>=45){
                turnToKing(arrival);
            }
        }
    }

}

void Board::turnToKing(int pos){
    int index=index_man_here(pos);
    string color=getPiece(index)->Color();
    delete pieces[index];
    pieces[index]=new King(pos,color);
}

map<int,vector<Move> > Board::playableMoves(string color) {
    vector<Move> possibleMoves;
    map<int,vector<Move> > playableMoves;
    int maxKills=0;
    //version ci-dessous ne marche pas, pourquoi ?
//    for(vector<Piece*>::iterator it;it!=pieces.end();it++) {
//        cout<<"ici"<<endl;
//        cout<<(*it)->getPosition()<<endl;
//        (*it)->select(*this,possibleMoves);
//        maxKills=max(maxKills,possibleMoves[possibleMoves.size()-1].getKills());
//    }
    for(int i=0;i<pieces.size();i++) {
    	if(pieces[i]->Color()==color) {
        	pieces[i]->select(*this,possibleMoves);
        	if(possibleMoves.size()>0) {
            	maxKills=max(maxKills,possibleMoves[possibleMoves.size()-1].getKills());
        	}
   		}
	}

    for(vector<Move>::iterator it=possibleMoves.begin();it!=possibleMoves.end();it++) {
        if(it->getKills()==maxKills) {
            playableMoves[it->getStart()].push_back(*it);
        }
    }
    return playableMoves;
}


int main(){

    Board* Plateau = new Board;
    vector<Move> PossibleMoves;

<<<<<<< HEAD
    Plateau->getPiece()

//    Plateau->getPiece(1)->setPosition(26);
//    Plateau->getPiece(2)->setPosition(29);
////    Plateau->getPiece(21)->killingMove(*Plateau,PossibleMoves);
////    cout<<PossibleMoves[0].getKills()<<endl;
////    cout<<PossibleMoves.size()<<endl;
////    Plateau->getPiece(11)->setPosition(16);
////    Plateau->getPiece(0)->killFreeMove(*Plateau,PossibleMoves);
////    for(std::vector<Move>::iterator it = PossibleMoves.begin(); it<PossibleMoves.end();it++){
////        cout << (*it).start << " " << (*it).arrival<< endl;
////    }
//    Plateau->getPiece(3)->setPosition(27);
//    Plateau->getPiece(22)->setPosition(24);
//    map<int,vector<Move> > m=Plateau->playableMoves();
//    for(map<int,vector<Move> >::iterator it=m.begin();it!=m.end();it++) {
//        for(vector<Move>::iterator it2=it->second.begin();it2!=it->second.end();it2++) {
//            cout<<it->first<<" "<<it2->getStart()<<" "<<it2->getArrival()<<endl;
//        }
//    }

    Plateau->getPiece(3)->setPosition(27);
    Plateau->getPiece(22)->setPosition(24);
    map<int,vector<Move> > m=Plateau->playableMoves("white");
    for(map<int,vector<Move> >::iterator it=m.begin();it!=m.end();it++) {
        for(vector<Move>::iterator it2=it->second.begin();it2!=it->second.end();it2++) {
            cout<<it->first<<" "<<it2->getStart()<<" "<<it2->getArrival()<<endl;
        }
    }
=======



//    Plateau->getPiece(19)->setPosition(28);
//    Plateau->turnToKing(32);


//    Plateau->turnToKing(34);
//    cout <<Plateau->getPiece(24)->isMan()<<endl;
//    cout << Plateau->getPiece(24)->getPosition() << endl;
//    Plateau->getPiece(24)->killFreeMove(*Plateau,PossibleMoves);
   for(vector<Move>::iterator it=PossibleMoves.begin(); it!=PossibleMoves.end(); it++){
        cout << it->getStart() << " : " << it->getArrival() << endl;
   }


>>>>>>> MethodKing

    return 0;
}
