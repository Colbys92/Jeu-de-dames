%module arbre
%include <std_vector.i>
%include <std_string.i>
%include <std_map.i>


%{
#define SWIG_FILE_WITH_INIT
#include "arbre.h"
%}

struct Piece{
public:
	Piece(int,std::string);
	Piece(const Piece&);
	~Piece();
	int getPosition();
	void setPosition(int);
	std::string Color();
	virtual bool isMan() const=0;
	virtual bool isKing() const;
	virtual void killFreeMove(Board&,std::vector<Move>&)=0;
	virtual void killingMove(Board&,std::vector<Move>&)=0;
	virtual void select(Board&,vector<Move>&)=0;
	virtual Piece* clone()=0;
protected:
	int position;
	std::string color;
};

struct Man : public Piece{
public:
	Man(int,std::string);
	~Man();
	virtual bool isMan() const;
	virtual void killFreeMove(Board&,std::vector<Move>&);
	virtual void killingMove(Board&,std::vector<Move>&);
	virtual void select(Board&,std::vector<Move>&);
	virtual Piece* clone();
};

struct Move{
private:
    vector<int> path;
    int kills;
public:
    Move(int , int , int );
    Move(const Move&);
    void operator=(const Move&);
    bool operator<(const Move&) const;
    int getStart();
    int getArrival();
    int getKills();
    std::vector<int> getPath() const;
    Move extendMove(Move);

};

struct Board {
private:
	std::vector<Piece*> pieces;
public:
	Board();
	Board(const Board&);
	void operator=(const Board&);
	int index_man_here(int);
	bool isManHere(int);
	bool isKingHere(int);
	bool isPieceHere(int);
	void playMove(const Move&);
	void killAt(int);
	Piece* getPiece(int);
	int nbPieces() const;
	std::map<int, std::vector<Move> > playableMoves(std::string);
};

struct King : public Piece {
public:
	King(int,std::string);
	King(const King&);
	~King();
	virtual bool isMan() const;
	virtual void killFreeMove(Board&,std::vector<Move>&);
	virtual void killingMove(Board&,std::vector<Move>&);
	virtual void select(Board&,std::vector<Move>&);
	virtual Piece* clone();

};


namespace std {
	%template(VectorMove) vector<Move>;
	%template(map_int_moves) map<int,vector<Move> >;
};