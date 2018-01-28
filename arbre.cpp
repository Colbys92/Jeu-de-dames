#include "arbre.h"
#include <queue>
#include <algorithm>
#include "tools.h"


//enum Color { white, black };

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
    vector<int> Orientation1;
    vector<int> Orientation2;
    {Orientation1.push_back(NW);
    Orientation2.push_back(NW2);
    Orientation1.push_back(NE);
    Orientation2.push_back(NE2);
    Orientation1.push_back(SE);
    Orientation2.push_back(SE2);
    Orientation1.push_back(SW);
    Orientation2.push_back(SW2);
    }

    bool isPiece;
    bool onEdges;
    int currentPosition;
    int currentPositionValue;
    for(int i=0; i<4;i++){
        // Diagonale :
        currentPosition = position;
        currentPositionValue= positionValue;
        onEdges = checkEdges(i, currentPosition);
        isPiece = B.isPieceHere(currentPosition+Orientation1[i]+currentPositionValue);

        while (!isPiece && !onEdges){
            possibleMoves.push_back(Move(position,currentPosition+Orientation1[i]+currentPositionValue,0));
            currentPosition = currentPosition+Orientation1[i]+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2; // Changement de ligne de position
            onEdges = checkEdges(i,currentPosition);
            isPiece = B.isPieceHere(currentPosition+Orientation1[i]+currentPositionValue);
        }
    }
}


void King::killingMove(Board &B, vector<Move> &possibleMoves){
    int positionValue=0;
    if(position%10<5){
        positionValue=1;
    }
    vector<int> orientation1;
    vector<int> orientation2;
    {orientation1.push_back(NW);
    orientation2.push_back(NW2);
    orientation1.push_back(NE);
    orientation2.push_back(NE2);
    orientation1.push_back(SE);
    orientation2.push_back(SE2);
    orientation1.push_back(SW);
    orientation2.push_back(SW2);
    }
    bool isPiece;
    bool isSecondPiece;
    bool onEdges;
    int currentPosition;
    int currentPositionValue;

    for(int i=0; i<4; i++){
        //Diagonale i :
        currentPosition = position;
        currentPositionValue= positionValue;
        isSecondPiece = false;

        isPiece = B.isPieceHere(currentPosition+orientation1[i]+currentPositionValue);
        onEdges=checkEdges(i,currentPosition);

        while (!isPiece && !onEdges){
            currentPosition = currentPosition+orientation1[i]+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2;
            onEdges=checkEdges(i,currentPosition);
            isPiece=B.isPieceHere(currentPosition+orientation1[i]+currentPositionValue);
        }
        if (!onEdges && isPiece &&  ((B.getPiece(B.index_man_here(currentPosition+orientation1[i]+currentPositionValue)))->Color() != color) ){
            currentPosition +=orientation1[i] + currentPositionValue;
            currentPositionValue = (currentPositionValue+1)%2;
            onEdges=checkEdges(i, currentPosition);
            isSecondPiece = B.isPieceHere(currentPosition+orientation1[i]+currentPositionValue);
            while(!onEdges && !isSecondPiece){
                possibleMoves.push_back(Move(position,currentPosition,1));
                currentPosition = currentPosition+orientation1[i]+currentPositionValue;
                currentPositionValue= (currentPositionValue+1)%2;
                onEdges=checkEdges(i, currentPosition);
                isSecondPiece = B.isPieceHere(currentPosition+orientation1[i]+currentPositionValue);
            }
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
    int currentPosition;
    bool onEdges=false;
    bool onEdges2 = false;
    bool isPiece=false;
    bool isPiece2=false;
    if(position%10<5){
        positionValue=1;
    }
    vector<int> orientation1;
    vector<int> orientation2;
    {orientation1.push_back(NW);
    orientation2.push_back(NW2);
    orientation1.push_back(NE);
    orientation2.push_back(NE2);
    orientation1.push_back(SE);
    orientation2.push_back(SE2);
    orientation1.push_back(SW);
    orientation2.push_back(SW2);
    }
    for(int i=0; i<4; i++){
        currentPosition=position;
        onEdges=checkEdges(i,currentPosition);
        currentPosition=position+positionValue+orientation1[i];

        isPiece = B.isPieceHere(currentPosition);
        onEdges2=checkEdges(i,currentPosition);

        isPiece2 = B.isPieceHere(position+orientation2[i]);
        if(!onEdges && !onEdges2 && isPiece && !isPiece2 &&
                (B.getPiece(B.index_man_here(currentPosition))->Color() != color)){
            possibleMoves.push_back(Move(position,position+orientation2[i],1));
        }
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

    {    //ca ne marche pas avec iterateur, pourquoi ?
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
}
    vector<int> orientation1;
    vector<int> orientation2;
    {orientation1.push_back(NW);
    orientation2.push_back(NW2);
    orientation1.push_back(NE);
    orientation2.push_back(NE2);
    orientation1.push_back(SE);
    orientation2.push_back(SE2);
    orientation1.push_back(SW);
    orientation2.push_back(SW2);
    }

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

            int positionKill = -1;
            for(int j=0; j<4; j++){
                //Initialisation du parcours :
                int positionValue= 0;
                if(start%10<5){
                    positionValue=1;
                }
                int currentPosition =start;
                bool onEdges=checkEdges(i,currentPosition);
                bool possibleDirection=true;
                bool ennemyFound=false;
                bool arrivalFound=false;
                if(arrival-start > 0 && j<2){
                    possibleDirection=false;
                }
                if(arrival-start < 0 && j>1){
                    possibleDirection=false;
                }

                while(possibleDirection && !onEdges && !ennemyFound){
                    currentPosition+= positionValue + orientation1[j];
                    onEdges=checkEdges(j,currentPosition);
                    if(isManHere(currentPosition)){
                        if((pieces[index_man_here(currentPosition)]->Color())!=(pieces[index_man_here(start)]->Color())){
                            ennemyFound=true;
                            positionKill=currentPosition;
                        }
                        else{
                            possibleDirection=false;
                        }
                    }
                    positionValue+=1%2;

                }
                while(possibleDirection && !onEdges && ennemyFound && !arrivalFound){
                    currentPosition+=positionValue+orientation1[j];
                    possibleDirection = !isManHere(currentPosition);
                    onEdges=checkEdges(j,currentPosition);
                    arrivalFound=(arrival-currentPosition==0);
                    positionValue+=1%2;
                }
                if(!arrivalFound){
                    positionKill=-1;
                }
                else {
                    killAt(positionKill);
                    pieces[index_man_here(start)]->setPosition(arrival);
                }

            }
            if(positionKill==-1){
                cerr << "No ennemy found" << endl;

            }
        }
    }

}

void Board::turnToKing(int pos){
    int index=index_man_here(pos);
    string color=getPiece(index)->Color();
    killAt(pos);
    pieces.push_back(new King(pos,color));
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
                maxKills=max(maxKills,possibleMoves[possibleMoves.size()-1].getKills()); // A Optimiser
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
std::pair<float,Move> Board::bestMoveAlphaBeta(string color, int depth, float manWeight, float kingWeight, bool maxNode, float alpha, float beta){
    // A Factoriser en écrivant avec moins min/ Fuite mémoire ? (clear les virtuals board)
    // Réécrire que avec currentMove, probleme avec val et currentMove :
    Board virtualBoard(*this);
    map<int,vector<Move> > currentPlayableMove;
    pair<float, Move> bestMove(0,Move());
    float val=0.;

    if(depth==0){
        bestMove.first=evaluate(manWeight,kingWeight,color);
        return bestMove;
    }
    else {
        if(!maxNode){
            val = numeric_limits<float>::max();
            currentPlayableMove=virtualBoard.playableMoves(color);
            for(map<int,vector<Move> >::iterator it1=currentPlayableMove.begin(); it1!=currentPlayableMove.end(); it1++){
                for(int playedMove=0; playedMove<(*it1).second.size(); playedMove++){
                    virtualBoard.playMove((*it1).second[playedMove]);
                    pair<float,Move> currentMove = virtualBoard.bestMoveAlphaBeta((color=="white")?"black":"white",depth-1,manWeight,kingWeight,true,alpha,beta);
                    val = std::min<float>(val,currentMove.first);
                    if (val<= alpha){
                        bestMove.second=currentMove.second;
                        bestMove.first=currentMove.first;
                        return bestMove;
                    }
                    beta = min<float>(beta,val);
                    virtualBoard = *this;
                }
            }
        }
        else{
            val = numeric_limits<float>::min();
            currentPlayableMove=virtualBoard.playableMoves(color);
            for(map<int,vector<Move> >::iterator it1=currentPlayableMove.begin(); it1!=currentPlayableMove.end(); it1++){
                for(int playedMove=0; playedMove<(*it1).second.size(); playedMove++){
                    virtualBoard.playMove((*it1).second[playedMove]);
                    pair<float,Move> currentMove = virtualBoard.bestMoveAlphaBeta((color=="white")?"black":"white",depth-1,manWeight,kingWeight,true,alpha,beta);
                    val = std::max<float>(val,currentMove.first);
                    if (val<= beta){
                        bestMove.second=currentMove.second;
                        bestMove.first=currentMove.first;
                        return bestMove;
                    }
                    alpha = max<float>(alpha,val);
                    virtualBoard = *this;
                }
            }
        }
    bestMove.first=val;
    return bestMove;
    }
}


std::pair<float,Move> Board::bestMove(map<int, vector<Move> > playableMove,string color, int profondeur, float manWeight, float kingWeight){
    // fuite de mémoire ? clear les boards ?
    // Renvoie le cout et le meilleur move, attention utiliser avec profondeur pair car sinon le move n'existe pas
    Board virtualBoard(*this);
    Board virtualBoardOpposite;
    map<int,vector<Move> > currentPlayableMove;
    map<int,vector<Move> > currentPlayableMoveOpposite;
    float valueCurrentMove=0;
    float valueCurrentMoveOpposite=5000; // A changer.
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

//    Plateau->getPiece(21)->select(*Plateau,PossibleMoves);

//    for(std::vector<Move>::iterator it=PossibleMoves.begin(); it!=PossibleMoves.end();it++){
//        cout << (*it).getStart() << " " << (*it).getArrival()<< endl;
//    }
//    Board b(true);
//    Move m=b.bestMove(b.playableMoves("white"),"white",2,1,1).second;
//    cout << b.bestMove(b.playableMoves("white"),"white",2,1,1).first << endl;
//    for(int i=0;i<3;i++) {
//        cout<<b.getPiece(i)->getPosition()<<endl;
//    }
//    std::cout<<b.bestMove(b.playableMoves("white"),"white",2,1,1).first<<endl;






// Move sans bouffe, pas d'apparition du King : Problème dans PlayMove ? Select ? PlayableMove ?

// Test final :
    for(int i=30; i<49; i++){
        Plateau->killAt(i);
    }

    for(int i=1; i<20; i++){
        Plateau->killAt(i);
    }
    Plateau->turnToKing(0);
    cout << Plateau->getPiece(0)->isKing() << true << endl;
    cout << Plateau->getPiece(1)->isKing() << true << endl;
    Plateau->getPiece(0)->setPosition(9);
    Plateau->getPiece(1)->setPosition(27);

    map<int,vector<Move> > P = (*Plateau).playableMoves("black");
    for(int i=0; i<=39; i++){
        for(std::vector<Move>::iterator it=P[i].begin(); it!=P[i].end();it++){
            cout << (*it).getStart() << " " << (*it).getArrival()<< endl;
        }
    }


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

//    for(int i=31; i<48; i++){
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
