#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;



// Déclaration des classes pour les utiliser après :
class Piece;
class Man;
class King;
class Board;
class Move;



class Piece {
protected:
    int position;
    std::string color;
public:
    //Defined Function :
    virtual ~Piece(){}
    Piece(int pos, std::string col){
        position = pos;
        color = col;
    }
    int getPosition(){
        return position;
    }
    void setPosition(int pos){position = pos;}
    std::string Color(){return color;}
    virtual bool IsMan() const =0;
    virtual bool IsKing() const {return !IsMan();}
    virtual void killFreeMove(Board& B, vector<Move> &possibleMoves) =0;
    virtual void killingMove(Board& B, vector<Move> &possibleMoves)=0;
};

class Man : public Piece {
public:
    ~Man(){}
    Man(int pos, std::string col) : Piece(pos,col){}
    virtual bool IsMan() const { return true;}
    virtual void killFreeMove(Board& B,vector<Move> &possibleMoves);
    virtual void killingMove(Board& B, vector<Move> &possibleMoves);
};

class Move{
public:
    int start;
    int arrival;
    int kills;
    Move(int s, int a, int k){
        start = s;
        arrival = a;
        kills = k;
    }
    int Start() {return start;}
    int Arrival() {return arrival;}
    int Kills() {return kills;}

};


class Board{
    std::vector<Piece*> pieces;
public :
    Board();
    int index_man_here(int pos){

        for(int i=0; i<pieces.size(); i++){
            if (pieces[i]->getPosition()==pos)   return i;
        }
        return -1;
    }
    int isManHere(int pos){
        for(int i=0; i<pieces.size(); i++){
            if (pieces[i]->getPosition()==pos and pieces[i]->IsMan())   return true;
        }
        return false;
    }
    Piece* getPiece(int index){
        return pieces[index];
    }
};


//class King : public Piece {
//    ~King();
//    virtual bool IsMan() const {return false;}

//};


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

Board::Board(){
    for(int i=0; i<20; i++){
        pieces.push_back(new Man(i,"black"));
    }
    for(int i=30; i<50;i++){
        pieces.push_back(new Man(i,"white"));
    }
}







