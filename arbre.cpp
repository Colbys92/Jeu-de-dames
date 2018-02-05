
#include "arbre.h"
#include <queue>
#include <algorithm>
#include "tools.h"




Move::Move(const Move &m){
    for(vector<int>::const_iterator it=m.path.begin(); it!=m.path.end(); it++){
        path.push_back(*it);
    }
    kills=m.kills;
}
bool Move::operator==(Move &m){
    return(m.kills==kills && m.path == path);
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

        currentPosition = position;
        currentPositionValue= positionValue;
        onEdges = checkEdges(i, currentPosition);
        isPiece = B.isPieceHere(currentPosition+Orientation1[i]+currentPositionValue);

        while (!isPiece && !onEdges){
            possibleMoves.push_back(Move(position,currentPosition+Orientation1[i]+currentPositionValue,0));
            currentPosition = currentPosition+Orientation1[i]+currentPositionValue;
            currentPositionValue= (currentPositionValue+1)%2;
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
                possibleMoves.push_back(Move(position,currentPosition+orientation1[i]+currentPositionValue,1));
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
            virtualBoard.playMove(currentMove, true);
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
            virtualBoard.playMove(currentMove, true);
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
    for(vector<Piece*>::iterator it=pieces.begin();it!=pieces.end();it++)
        delete *it;
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

void Board::playMove(const Move &m, bool inSelect) {
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
                bool onEdges=checkEdges(j,currentPosition);
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
                    if(isPieceHere(currentPosition)){
                        if((pieces[index_man_here(currentPosition)]->Color())!=(pieces[index_man_here(start)]->Color())){
                            ennemyFound=true;
                            positionKill=currentPosition;
                        }
                        else{
                            possibleDirection=false;
                        }
                    }
                    positionValue=(positionValue+1)%2;

                }
                while(possibleDirection && !onEdges && ennemyFound && !arrivalFound){
                    currentPosition+=positionValue+orientation1[j];
                    possibleDirection = !isPieceHere(currentPosition);
                    onEdges=checkEdges(j,currentPosition);
                    arrivalFound=(arrival-currentPosition==0);
                    positionValue= (positionValue+1)%2;
                }
                if(!arrivalFound){
                    positionKill=-1;
                }
                else {
                    killAt(positionKill);
                    pieces[index_man_here(start)]->setPosition(arrival);
                    break;
                }

            }
            if(positionKill==-1){
                cerr << "No ennemy found" << endl;
            }
        }
        int arrival = m.getPath()[m.getPath().size()-1];
        if(!inSelect){
            if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color() == "white" && arrival<5){
                turnToKing(arrival);
            }
            else if (pieces[index_man_here(arrival)]->isMan() && pieces[index_man_here(arrival)]->Color() == "black" && arrival >44){
                turnToKing(arrival);
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


float Board::evaluateBetter(float manWeight, float kingWeight,float nbMoveWeight, float advancementForwardWeight, float centralWeight, string color){
    float value=0;

    for(vector<Piece*>::iterator it=pieces.begin();it!=pieces.end();it++) {
        if((*it)->Color() == color){
            value+=((*it)->isMan())?manWeight:kingWeight;
        }
        else {
            value-=((*it)->isMan())?manWeight:kingWeight;
        }

        if((*it)->isMan()){
            int i = (*it)->getPosition();
            if(i%5<4 && i%5>0){
                value+=((*it)->Color()==color)?centralWeight:(-1.)*centralWeight;
            }

            float advanceWhite =((49-(i))-(49-i)%5);
            float advanceBlack = (i-i%5);
            if(color=="white"){
                value=((*it)->Color()=="white")?(value+advanceWhite*advancementForwardWeight):(value-advanceBlack*advancementForwardWeight);
            }
            else{
                value=((*it)->Color()=="black")?(value+advanceBlack*advancementForwardWeight):(value-advanceWhite*advancementForwardWeight);
            }
        }

    }

    map<int,vector<Move> > playableMove = playableMoves((color=="white")?"black":"white");
    if (playableMove.size()!=0){
        for(map<int,vector<Move> >::iterator it=playableMove.begin(); it!=playableMove.end();it++){
            for(int i=0; i<(*it).second.size(); i++){
                value-=nbMoveWeight;
            }
        }
    }
    return value;
}

std::pair<float,Move> Board::bestMoveAlphaBeta2(string color,int depth, float manWeight, float kingWeight,float nbMoveWeight, float centralWeight, float advanceWeight, bool maxNode,float alpha, float beta ){
    Board virtualBoard(*this);
    pair<float,Move> move(0,Move());
    pair<float,Move> currentMove;
    map<int,vector<Move> > currentPlayableMove;

    if(maxNode){
        currentPlayableMove = virtualBoard.playableMoves(color);
    }
    else{
        currentPlayableMove = virtualBoard.playableMoves((color=="white")?"black":"white");
    }

    if(currentPlayableMove.size()==0 || depth==0){
        move.first = virtualBoard.evaluateBetter(manWeight,kingWeight,nbMoveWeight,advanceWeight,centralWeight,color);
        return move;
    }
    else{
        if(!maxNode){
            move.first=+10000000;
            for(map<int,vector<Move> >::iterator it = currentPlayableMove.begin(); it!=currentPlayableMove.end(); it++){
                for(int i=0; i<(*it).second.size(); i++){
                    currentMove.second=(*it).second[i];
                    virtualBoard.playMove(currentMove.second);
                    currentMove.first = virtualBoard.bestMoveAlphaBeta2(color,depth-1,manWeight,kingWeight, nbMoveWeight, centralWeight, advanceWeight,true,alpha,beta).first;
                    if(currentMove.first<move.first){
                        move.first=currentMove.first;
                        move.second=currentMove.second;
                    }
                    if(move.first<=alpha){
                        return move;
                    }
                    beta = std::min<float>(beta,move.first);
                    virtualBoard= *this;
                }
            }
        }
        else {
            move.first=-10000000;
            for(map<int,vector<Move> >::iterator it = currentPlayableMove.begin(); it!=currentPlayableMove.end(); it++){
                for(int i=0; i<(*it).second.size(); i++){
                    currentMove.second=(*it).second[i];
                    virtualBoard.playMove(currentMove.second);
                    currentMove.first = virtualBoard.bestMoveAlphaBeta2(color,depth-1,manWeight,kingWeight, nbMoveWeight,centralWeight, advanceWeight,false,alpha,beta).first;
                    if(currentMove.first>move.first){
                        move.first=currentMove.first;
                        move.second=currentMove.second;
                    }
                    if(move.first>=beta){
                        return move;
                    }
                    alpha=std::max<float>(alpha,move.first);
                    virtualBoard= *this;
                }
            }
        }
        return move;
    }
}


int Board::endGame(){

    bool white=false;
    bool black=false;
    int i=0;
    while((!white || !black) && i<pieces.size()){
        if(getPiece(i)->Color()=="white"){
            white=true;
        }
        else{
            black=true;
        }
    i+=1;
    }
    return(!white || !black);
}


int Board::timeMatch(){
    // Renvoie 0 en début de partie, 1 en milieu de partie, 2 en fin de partie :
    if(pieces.size()<10){
        return 2;
    }
    else if(pieces.size()<25){
        return 1;
    }
    return 0;
}


Move getSecond(pair<float,Move> A){
    return A.second;
}
