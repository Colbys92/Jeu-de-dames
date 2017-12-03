#pragma once

#include <string>
#include <iostream>
#include <stdexcept>


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
};

class Move{

};

class Piece{
    int position;
    std::string color;

public :
    Piece(int pos, std::string col){
        position = pos;
        color = col;
    }

    virtual bool IsMan(){}
};


class Man : public Piece {
    virtual bool IsMan(){return true.}


};

class King : public Piece {
    vi

};


