#pragma once

#include <string>
#include <iostream>
#include <stdexcept>



class Move{
    int start;
    int arrival;
    int kills;
    Move(int s, int a, int k){
        start = s;
        arrival = a;
        kill = k;
    }
    int Start() {return start;}
    int Arrival() {return arrival;}
    int Kills() {return kills;}

};

class Piece{
protected:
    int position;
    std::string color;

public :
    Piece(int pos, std::string col){
        position = pos;
        color = col;
    }
    int getPosition(){
        return position;
    }
    std::string Color(){return color;}
    virtual bool IsMan() const =0;
    virtual bool IsKing() const {return !IsMan();}
    virtual void killFreeMove(Board& B,Move* possibleMoves);
    virtual void killingMove(Board& B, Move* possibleMoves);
};


class Man : public Piece {
    virtual bool IsMan() const { return true;}
    virtual void killFreeMove(Board& B,vector<Move> possibleMoves){
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
    virtual void killingMove(Board& B,vector<Move> possibleMoves){
        int positionValue=0;
        if(position%10<5){
            positionValue=1;
        }



        // En haut à droite
        if(position%10!=4 && position%10!=9
                && B.isManHere(position-5+positionValue)
                && B.isManHere(position-9)
                && B.getPiece(B.index_man_here(position-5+positionValue)).color()!=color){
            possibleMoves.push_back(Move(position,position-9,1));

        }
        // En haut à gauche
        if(position%10!=5 && position%10!=0
                && B.isManHere(position-6+positionValue)
                && B.isManHere(position-11)
                && B.getPiece(B.index_man_here(position-6+positionValue)).color()!=color){
            possibleMoves.push_back(Move(position,position-11,1));

        }
        // En bas à droite
        if(position%10!=4 && position%10!=9
                && B.isManHere(position+5+positionValue)
                && B.isManHere(position+11)
                && B.getPiece(B.index_man_here(position+5+positionValue)).color()!=color){
            possibleMoves.push_back(Move(position,position+11,1));

        }
        // En bas à gauche
        if(position%10!=5 && position%10!=0
                && B.isManHere(position+4+positionValue)
                && B.isManHere(position+9)
                && B.getPiece(B.index_man_here(position+4+positionValue)).color()!=color){
            possibleMoves.push_back(Move(position,position+9,1));

        }
    }
};

class King : public Piece {
    virtual bool IsMan() const {return false;}

};


class Board{
    std::vector<Piece> pieces;
public :
    Board(){
        for(int i=0; i<20; i++){
            pieces.push_back(Man(i,"white"));
        }
        for(int i=30; i<50;i++){
            pieces.push_back(Man(i,"Black"));
        }
    }
    int index_man_here(int pos){

        for(int i=0; i<pieces.size(); i++){
            if (pieces[i].getPosition()==pos)   return i;
        }
        return -1;
    }
    int isManHere(int pos){
        for(int i=0; i<pieces.size(); i++){
            if (pieces[i].getPosition()==pos and pieces[i].IsMan())   return true;
        }
        return false;
    }
    Piece getPiece(int index){
        return pieces[index];
    }
};
