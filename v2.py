#if importing fails :
#import sys
#sys.path.append('c:\\users\\matthieu\\documents\\ponts\\2A\\td log\\projet')


from arbre import *


possibleMoves=VectorMove()
plateau=Board()
plateau.getPiece(0).setPosition(26)
plateau.getPiece(1).setPosition(27)

plateau.getPiece(21).killingMove(plateau,possibleMoves)