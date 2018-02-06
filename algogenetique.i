%module algogenetique
%include <std_vector.i>
%include <std_string.i>
%include <std_map.i>
%include <std_pair.i>
%import "arbre.i"

%{
#define SWIG_FILE_WITH_INIT
#include "algogenetique.h"
%}



struct Individu {
private:
	int score;
    float manWeight;
    float kingWeight;
    int depth;
    float nbMoveWeight;
    float advancementForwardWeight;
    float centralWeight;
public:
    Individu(int score1, float manWeight1, float kingWeight1, int depth1, float nbMoveWeight1, float advancementForwardWeight1, float centralWeight1);
    float getAdvancementForwardWeight();
    float getNbMoveWeight();
    int getDepth();
    float getKingWeight();
    float getManWeight();
    float getCentralWeight();
    Move bestMoveAlphaBeta(Board& B,std::string color,float alpha, float beta );
};