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
    isSecondPiece = false;
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
    isSecondPiece = false;
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
    isSecondPiece = false;
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
    isSecondPiece = false;
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

void King::select(Board& b, vector<Move>& possibleMoves){
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
        if(position%10!=4 && !B.isPieceHere(position+NE+positionValue) && position>=5){
            possibleMoves.push_back(Move(position,position+NE+positionValue,0));
        }
        // Déplacement NW :
        if(position%10!=5 && !B.isPieceHere(position+NW+positionValue) && position>=5){
            possibleMoves.push_back(Move(position,position+NW+positionValue,0));
        }
    }
    if(color == "black"){
        // Déplacement SE :
        if(position%10!=4 && !B.isPieceHere(position+SE+positionValue) && position<45){
            possibleMoves.push_back(Move(position,position+SE+positionValue,0));
        }
        // Déplacement SW :
        if(position%10!=5 && !B.isPieceHere(position+SW+positionValue) && position<45){
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
            && position+NE2>=0
            && B.getPiece(B.index_man_here(position+NE+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+NE2,1));

    }
    // Déplacement NW :
    if(position%10!=5 && position%10!=0
            && B.isPieceHere(position+NW+positionValue)
            && !B.isPieceHere(position+NW2)
            && position+NW2>=0
            && B.getPiece(B.index_man_here(position+NW+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+NW2,1));

    }
    // Déplacement SE :
    if(position%10!=4 && position%10!=9
            && B.isPieceHere(position+SE+positionValue)
            && !B.isPieceHere(position+SE2)
            && position+SE2>=0
            && B.getPiece(B.index_man_here(position+SE+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+SE2,1));

    }
    // Déplacement SW :
    if(position%10!=5 && position%10!=0
            && B.isPieceHere(position+SW+positionValue)
            && !B.isPieceHere(position+SW2)
            && position+SW2>=0
            && B.getPiece(B.index_man_here(position+SW+positionValue))->Color()!=color){
        possibleMoves.push_back(Move(position,position+SW2,1));
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
        int arrival = m.getPath()[1];

        // Transformation en dame :
        if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color()=="white" && arrival<5){
            turnToKing(arrival);
        }
        if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color()=="black" && arrival>=45){
            turnToKing(arrival);
        }

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

            pieces[index_man_here(m.getPath()[i])]->setPosition(m.getPath()[i+1]); // Déplacement effectif
        }
        int arrival = m.getPath()[m.getPath().size()-1];
        if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color()=="white" && arrival<5){
            turnToKing(arrival);
        }
        if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color()=="black" && arrival>=45){
            turnToKing(arrival);
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
//    version ci-dessous ne marche pas, pourquoi ?
//    for(vector<Piece*>::iterator it=pieces.begin();it!=pieces.end();it++) {
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

float Board::evaluate(float manWeight, float kingWeight, string color) {
    float value=0;
    for(vector<Piece*>::iterator it=pieces.begin();it!=pieces.end();it++) {
        if((*it)->Color() == color){
            value+=((*it)->isMan())?manWeight:kingWeight;
        }
        else {
            value-=((*it)->isMan())?manWeight:kingWeight;
        }
    }
    return value;
}

std::pair<float,Move> Board::bestMove(map<int, vector<Move> > playableMove,string color, int profondeur, float manWeight, float kingWeight){
    // Renvoie le cout et le meilleur move, attention utiliser avec profondeur pair car sinon le move n'existe pas
    Board virtualBoard(*this);
    Board virtualBoardOpposite;
    map<int,vector<Move> > currentPlayableMove;
    map<int,vector<Move> > currentPlayableMoveOpposite;
    float valueCurrentMove=0;
    float valueCurrentMoveOpposite=5000;
    pair<float, Move> bestMoveOpposite;
    pair<float, Move> bestMove;
    bestMove.first = valueCurrentMove;
    bestMoveOpposite.first=valueCurrentMoveOpposite;
    string colorOpposite;
    if(color == "white"){
        colorOpposite="black";
    }
    else {
        colorOpposite="white";
    }

    if(profondeur>0){
        for(map<int,vector<Move> >::iterator it1=playableMove.begin(); it1!=playableMove.end(); it1++){
            for(int playedMove=0; playedMove<(*it1).second.size(); playedMove++){

                virtualBoard.playMove((*it1).second[playedMove]);
                currentPlayableMove = virtualBoard.playableMoves(colorOpposite);
                virtualBoardOpposite=virtualBoard;


                for(map<int,vector<Move> >::iterator it=currentPlayableMove.begin(); it!=currentPlayableMove.end(); it++){
                    for(int j=0; j<(*it).second.size();j++){
                        virtualBoardOpposite.playMove((*it).second[j]);
                        currentPlayableMoveOpposite = virtualBoardOpposite.playableMoves(color);

                        valueCurrentMoveOpposite = virtualBoardOpposite.bestMove(currentPlayableMoveOpposite,color,profondeur-2,manWeight,kingWeight).first;

                        if(bestMoveOpposite.first<valueCurrentMoveOpposite){
                            bestMoveOpposite.first=valueCurrentMoveOpposite;
                            bestMoveOpposite.second=(*it).second[j];
                        }
                        //Nettoyage des valeurs :
                        currentPlayableMoveOpposite.clear();
                        virtualBoardOpposite=virtualBoard;
                    }
                }


                if(bestMoveOpposite.first>bestMove.first){
                    bestMove.second= (*it1).second[playedMove];
                    bestMove.first=bestMoveOpposite.first;
                }



                // Nettoyage des valeurs :
                currentPlayableMove.clear();
                virtualBoard = *this;
            }
        }

        return bestMove;
    }
    else {
        bestMove.first=evaluate(manWeight,kingWeight,color);
        return bestMove;

    }
}



int main(){

    Board* Plateau = new Board;
    vector<Move> PossibleMoves;
    Board b(true);
    Move m=b.bestMove(b.playableMoves("white"),"white",2,1,1).second;
    cout << b.bestMove(b.playableMoves("white"),"white",2,1,1).first << endl;
    for(int i=0;i<3;i++) {
        cout<<b.getPiece(i)->getPosition()<<endl;
    }
    std::cout<<b.bestMove(b.playableMoves("white"),"white",2,1,1).first<<endl;






// Move sans bouffe, pas d'apparition du King : Problème dans PlayMove ? Select ? PlayableMove ?

// Test final :
//    map<int,vector<Move> > P = (*Plateau).playableMoves("black");
//    for(int i=0; i<=19; i++){
//        for(std::vector<Move>::iterator it=P[i].begin(); it!=P[i].end();it++){
//            cout << (*it).getStart() << " " << (*it).getArrival()<< endl;
//        }
//    }


//    cout << "Position du futur bouffe : " <<Plateau->getPiece(23)->getPosition() << endl;
//    cout <<" Position du bouffeur :" <<Plateau->getPiece(19)->getPosition() << endl;

//    Plateau->getPiece(23)->setPosition(23);
//    cout << Plateau->getPiece(23)->getPosition() << endl;
//    Plateau->killAt(32);
//    map<int,vector<Move> > Q = (*Plateau).playableMoves("black");
//    for(int i=0; i<=19; i++){
//        for(std::vector<Move>::iterator it=Q[i].begin(); it!=Q[i].end();it++){
//            cout << (*it).getStart() << " " << (*it).getArrival()<< " Kills :" <<(*it).getKills()<< endl;
//        }
//    }

// Play Move ok :
//    Plateau->getPiece(19)->killingMove(*Plateau,PossibleMoves);

//    for(std::vector<Move>::iterator it = PossibleMoves.begin(); it<PossibleMoves.end();it++){
//         cout << (*it).getStart() << " " << (*it).getArrival()<< " Kills : " <<(*it).getKills() << endl;
//    }

//    Plateau->playMove(PossibleMoves[0]);
//    cout <<" Position du bouffeur :" <<Plateau->getPiece(19)->getPosition() << endl;
//    cout <<"Normalement, mauvaise position du bouffé !"<< Plateau->getPiece(23)->getPosition() << endl;


// Test select ok :

//    Plateau->getPiece(19)->select(*Plateau, PossibleMoves);
//    for(std::vector<Move>::iterator it = PossibleMoves.begin(); it<PossibleMoves.end();it++){
//        cout << (*it).getStart() << " " << (*it).getArrival()<< " Kills : " <<(*it).getKills() << endl;
//    }



    // Test de la transformation en dame :

//    for(int i=31; i<50; i++){
//        Plateau->killAt(i);
//    }
//    Plateau->getPiece(20)->setPosition(43);
//    Plateau->getPiece(18)->setPosition(38);
//    map<int,vector<Move> > R = Plateau->playableMoves("black");
//    cout << R[38][0].getStart() << " : "<< R[38][0].getArrival() << endl;
//    Plateau->playMove(R[38][0]);
//    cout << Plateau->isManHere(49) << endl;
//    cout << true << endl;




    return 0;
}
