%module arbre



%{
#define SWIG_FILE_WITH_INIT
#include "arbre.h"
%}

struct Piece{
public:
	Piece(int,std::string);
	~Piece();
	int getPosition();
	void setPosition(int);
	std::string Color();
	virtual bool isMan() const=0;
	virtual bool isKing() const;
	virtual void killFreeMove(Board&,std::vector<Move>&)=0;
	virtual void killingMove(Board&,std::vector<Move>&)=0;
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
};

struct Move{
private:
    int start;
    int arrival;
    int kills;
public:
    Move(int , int , int );
    int getStart();
    int getArrival();
    int getKills();

};

struct Board {
private:
	std::vector<Piece*> pieces;
public:
	Board();
	int index_man_here(int);
	int isManHere(int);
	Piece* getPiece(int);
};

%include "std_vector.i"
namespace std {
	%template(VectorMove) vector<Move>;
};
