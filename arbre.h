#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <limits>
using namespace std;

// Constant globale en position value =0:

// 1 Case :

const int NW =-6;
const int NE =-5;
const int SE =+5;
const int SW =+4;



// 2 Cases :

const int NW2 = -11;
const int NE2 = -9;
const int SE2 = 11;
const int SW2 = 9;

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
    Piece(const Piece& p){
        position = p.position;
        color = p.color;
    }
    int getPosition(){return position;}
    void setPosition(int pos){position = pos;}
    std::string Color(){return color;}
    virtual bool isMan() const =0;
    virtual bool isKing() const {return !isMan();}
    virtual void killFreeMove(Board& B, vector<Move> &possibleMoves) =0;
    virtual void killingMove(Board& B, vector<Move> &possibleMoves)=0;
    virtual void select(Board& B,vector<Move> &possibleMoves)=0;
    virtual Piece* clone()=0;
};

class Man : public Piece {
public:
    ~Man(){}
    Man(int pos, std::string col) : Piece(pos,col){}
    Man(const Man& m) : Piece(m.position, m.color){}
    virtual bool isMan() const { return true;}
    virtual void killFreeMove(Board& B,vector<Move> &possibleMoves);
    virtual void killingMove(Board& B, vector<Move> &possibleMoves);
    virtual void select(Board& B, vector<Move> &possibleMoves);
    virtual Piece* clone();
};

class Move{
private:
    vector<int> path;
    int kills;
public:
    Move() {
        kills=-1;
    }
    Move(const Move& m);
    Move(int s, int a, int k){
        path.push_back(s);
        path.push_back(a);
        kills = k;
    }
    void operator=(const Move& m);
    bool operator==(Move& m);
    bool operator<(const Move& m) const;
    int getStart() {return path[0];}
    int getArrival() {return path[path.size()-1];}
    int getKills() const {return kills;}
    vector<int> getPath() const {return path;}
    Move extendMove(Move m); //Can only extend with one elementary move (path.size()=2)

};


class Board{
    std::vector<Piece*> pieces;
public :
    Board();
    ~Board();
    Board(const Board& b);
    void operator=(const Board& b);
    int index_man_here(int pos);
    bool isManHere(int pos);
    bool isKingHere(int pos);
    bool isPieceHere(int pos);
    void playMove(const Move& m, bool inSelect=false);
    void killAt(int pos);
    Piece* getPiece(int index){return pieces[index];}
    void turnToKing(int pos);
    int nbPieces() const{return pieces.size();}
    float evaluateBetter(float manWeight, float kingWeight,float nbMoveWeight, float advancementForwardWeight, float centralWeight, string color);
    std::pair<float,Move> bestMoveAlphaBeta2(string color,int depth, float manWeight, float kingWeight,float nbMoveWeight, float centralWeight, float advanceWeight, bool maxNode,float alpha, float beta );
    map<int,vector<Move> > playableMoves(string color);
    int endGame();
    int timeMatch();
};



class King : public Piece {
public :
        ~King(){}
        virtual bool isMan() const {return false;}
        King(int pos, std::string col) : Piece(pos,col){}
        King(const King& k) : Piece(k.position, k.color){}
        virtual void killFreeMove(Board& B,vector<Move> &possibleMoves);
        virtual void killingMove(Board& B, vector<Move> &possibleMoves);
        virtual Piece* clone();
        virtual void select(Board& b, vector<Move> &possibleMoves);

};

Move getSecond(pair<float, Move> A );












