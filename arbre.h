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
    virtual bool isMan() const =0;
    virtual bool isKing() const {return !isMan();}
    virtual void killFreeMove(Board& B, vector<Move> &possibleMoves) =0;
    virtual void killingMove(Board& B, vector<Move> &possibleMoves)=0;
};

class Man : public Piece {
public:
    ~Man(){}
    Man(int pos, std::string col) : Piece(pos,col){}
    virtual bool isMan() const { return true;}
    virtual void killFreeMove(Board& B,vector<Move> &possibleMoves);
    virtual void killingMove(Board& B, vector<Move> &possibleMoves);
};

class Move{
private:
    int start;
    int arrival;
    int kills;
public:
    Move() {
        start=-1;
        arrival=-1;
        kills=-1;
    }
    Move(int s, int a, int k){
        start = s;
        arrival = a;
        kills = k;
    }
    int getStart() {return start;}
    int getArrival() {return arrival;}
    int getKills() {return kills;}

};


class Board{
    std::vector<Piece*> pieces;
public :
    Board() {
    for(int i=0; i<20; i++){
        pieces.push_back(new Man(i,"black"));
    }
    for(int i=30; i<50;i++){
        pieces.push_back(new Man(i,"white"));
    }
}
    int index_man_here(int pos){

        for(int i=0; i<pieces.size(); i++){
            if (pieces[i]->getPosition()==pos)   return i;
        }
        return -1;
    }
    int isManHere(int pos){
        for(int i=0; i<pieces.size(); i++){
            if (pieces[i]->getPosition()==pos && pieces[i]->isMan())   return true;
        }
        return false;
    }
    Piece* getPiece(int index){
        return pieces[index];
    }
};


//class King : public Piece {
//    ~King();
//    virtual bool isMan() const {return false;}

//};











