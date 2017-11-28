import pygame
from pygame.locals import *

class Piece:
    def __init__(self,position,color):
        self.position=position
        self.color=color
    def deepCopy(self,pieceToCopy):
        self.position=pieceToCopy.position
        self.color=pieceToCopy.color
    def display(self,window):
        increment=0
        if(self.position%10<5):
            increment+=50
        if self.color=="white":
             pygame.draw.circle(window,(255,255,255),(262+increment+100*(self.position%5)+25,109+50*(self.position//5)+25),20)
        else:
            #pygame.draw.circle(window,(255,255,255),(262+increment+100*(self.position%5)+25,109+50*(self.position//5)+25),20,2)
            pygame.draw.circle(window,(255,0,0),(262+increment+100*(self.position%5)+25,109+50*(self.position//5)+25),20)
            

class Man:
    def __init__(self,position,color):
        Piece.__init__(self,position,color)
    def selectionner(self,board):
        moves=[]
        virtualBoard=Board()
        virtualBoard.deepCopy(board)
        def computeMoves(currentPosition,counter,virtualBoard):
            if counter==0:
                moves.extend(virtualBoard.deplacement_sans_bouffe(self.position))
            movesToAdd=virtualBoard.deplacement_avec_bouffe(self.position)
            for move in movesToAdd:
                move.kills=counter
            moves.extend(movesToAdd)
            for move in movesToAdd:
                computeMoves(move.arrival,counter+1,board.playVirtualMove(move))
        computeMoves(self.position,1,virtualBoard)
        maxKills=max(moves).kills
        return [move for move in moves if maxKills==move.kills]
    def display(self,window):
        Piece.display(self,window)
    def deepCopy(self,manToCopy):
        Piece.deepCopy(self,manToCopy)


class King:
    def __init__(self,position,color):
        Piece.__init__(self,position,color)

class Move:
    def __init__(self,start,arrival,kills):
        self.start=start
        self.arrival=arrival
        self.kills=kills
    def __lt__(self,moveToCompare):
        return self.kills<moveToCompare.kills

class Board:
    def __init__(self):
        self.pieces=[]
        for i in range(20):
            self.pieces.append(Man(i,"black"))
        for i in range(30,50):
            self.pieces.append(Man(i,"white"))
    def deepCopy(self,boardToCopy):
        for i in range(len(boardToCopy.pieces)):
            self.pieces[i].deepCopy(boardToCopy.pieces[i])
    def display(self,window):
        pygame.draw.rect(window,(255,255,255),Rect(262,109,500,500),1)
        for i in range(10):
            for j in range(10):
                if (i+j)%2==0:
                    pygame.draw.rect(window,(255,255,255),Rect(262+i*50,109+j*50,50,50))
                else:
                    pygame.draw.rect(window,(0,0,0),Rect(262+i*50,109+j*50,50,50))
        pygame.draw.rect(window,(255,255,255),Rect(262,109,500,500),1)
        for piece in self.pieces:
            piece.display(window)
    #Méthode de la classe pieces
    
    def index_man_here(self, pos) :
        n = len(self.pieces)
        for i in range(n) :
            if self.pieces[i].position == pos :
                return i
                
    def man_here(self, pos) :
        n = len(self.pieces)
        for i in range(n) :
            if self.pieces[i].position == pos :
                return self.pieces[i]
        
    def is_man_here(self, pos) :
        n = len(self.pieces)
        for i in range(n) :
            if self.pieces[i].position == pos :
                return True
        return False
    
    def deplacement_sans_bouffe(self, pos) :
        man = self.man_here(pos)
        possibleMove = []
        if (man.position%10 <5) : #ligne bordure droite
            
            if (man.color == "white") :
                if ((man.position%10 !=4) and not (self.is_man_here(man.position-4))):
                    # En haut à droite
                    possibleMove.append(Move(man.position,man.position-4,0))
                if (self.is_man_here(man.position-5)) :
                    # En haut à gauche
                    possibleMove.append(Move(man.position,man.position-5,0))
            
            if (man.color == "black") :
                if ((man.position%10 !=4) and not (self.is_man_here(man.position+6))):
                    # En bas à droite
                    possibleMove.append(Move(man.position,man.position+6,0))
                if (self.is_man_here(man.position+5)) :
                    # En bas à gauche
                    possibleMove.append(Move(man.position,man.position+5,0))
    
                    
        if (man.position%10 >4) : #ligne bordure gauche
        
            if (man.color == "white") :
                if ((man.position%10 !=5) and not (self.is_man_here(man.position-5))):
                    # En haut à gauche
                    possibleMove.append(Move(man.position,man.position-5,0))
                if (self.is_man_here(man.position-6)) :
                    # En haut à droite
                    possibleMove.append(Move(man.position,man.position-6,0))
            
            if (man.color == "black") :
                if ((man.position%10 !=5) and not (self.is_man_here(man.position+4))):
                    # En bas à gauche
                    possibleMove.append(Move(man.position,man.position+5,0))
                if (not self.is_man_here(man.position+5)) :
                    # En bas à droite
                    possibleMove.append(Move(man.position,man.position+4,0))
                    
        return possibleMove
        
        
    def deplacement_avec_bouffe(self, pos) :
        possibleMove = []
        man = self.man_here(pos)
        
    
        if (man.position%10 <5) : #ligne bordure droite
            
            if (man.color == "white") :
                if ((man.position%10 !=4) \
                and (self.is_man_here(man.position-4)  \
                and (not self.is_man_here(man.position-9)) \
                and (self.man_here(man.position-4).color == "black"))):
                    # En haut à droite
                    possibleMove.append(Move(man.position,man.position-9,1))
                    
                if (self.is_man_here(man.position-5)) \
                and (not self.is_man_here(man.position-11)) \
                and (self.man_here(man.position-5).color == "black") :
                    # En haut à gauche
                    possibleMove.append(Move(man.position,man.position-11,1))
                    
                if ((man.position%10 !=4) \
                and (self.is_man_here(man.position+6)) \
                and (not self.is_man_here(man.position+11)) \
                and (self.man_here(man.position+6).color == "black")):
                    # En bas à droite
                    possibleMove.append(Move(man.position,man.position+11,1))
                    
                if ((self.is_man_here(man.position+5)) \
                and (not self.is_man_here(man.position+9)) \
                and (self.man_here(man.position+5).color == "black")) :
                    # En bas à gauche
                    possibleMove.append(Move(man.position,man.position+9,1))
            
            if (man.color == "black") :
                if ((man.position%10 !=4) \
                and (self.is_man_here(man.position-4)  \
                and (not self.is_man_here(man.position-9)) \
                and (self.man_here(man.position-4).color == "white"))):
                    # En haut à droite
                    possibleMove.append(Move(man.position,man.position-9,1))
                    
                if (self.is_man_here()[man.position-5]==0 \
                and (not self.is_man_here(man.position-11)) \
                and (self.man_here(man.position-4).color == "white")) :
                    # En haut à gauche
                    possibleMove.append(Move(man.position,man.position-11,1))
                    
                if ((man.position%10 !=4) \
                and (self.is_man_here(man.position+6)) \
                and (not self.is_man_here(man.position+11)) \
                and (self.man_here(man.position+6).color == "white")):
                    # En bas à droite
                    possibleMove.append(Move(man.position,man.position+11,1))
                    
                if ((self.is_man_here(man.position+5)) \
                and (not self.is_man_here(man.position+9)) \
                and (self.man_here(man.position+5).color == "white")) :
                    # En bas à gauche
                    possibleMove.append(Move(man.position,man.position+9,1))
    
                    
        if (man.position%10 >4) : #ligne bordure gauche
        
            if (man.color == "white") :
                if ((man.position%10 !=5) \
                and (self.is_man_here(man.position-5)  \
                and (not self.is_man_here(man.position-9)) \
                and (self.man_here(man.position-5).color == "black"))):
                    # En haut à droite
                    possibleMove.append(Move(man.position,man.position-9,1))
                    
                if ((self.is_man_here(man.position-6)) \
                and (not self.is_man_here(man.position-11)) \
                and (self.man_here(man.position-6).color == "black")) :
                    # En haut à gauche
                    possibleMove.append(Move(man.position,man.position-11,1))
                    
                if ((man.position%10 !=5) \
                and (self.is_man_here(man.position+5)) \
                and (not self.is_man_here(man.position+11)) \
                and (self.man_here(man.position+5).color == "black")):
                    # En bas à droite
                    possibleMove.append(Move(man.position,man.position+11,1))
                    
                if ((self.is_man_here(man.position+4)) \
                and (not self.is_man_here(man.position+9)) \
                and (self.man_here(man.position+4).color == "black")) :
                    # En bas à gauche
                    possibleMove.append(Move(man.position,man.position+9,1))
            
            if (man.color == "black") :
                if ((man.position%10 !=5) \
                and (self.is_man_here(man.position-5)  \
                and (not self.is_man_here(man.position-9)) \
                and (self.man_here(man.position-5).color == "white"))):
                    # En haut à droite
                    possibleMove.append(Move(man.position,man.position-9,1))
                    
                if ((self.is_man_here(man.position-6)) \
                and (not self.is_man_here(man.position-11)) \
                and (self.man_here(man.position-6).color == "white")) :
                    # En haut à gauche
                    possibleMove.append(Move(man.position,man.position-11,1))
                    
                if ((man.position%10 !=5) \
                and (self.is_man_here(man.position+5)) \
                and (not self.is_man_here(man.position+11)) \
                and (self.man_here(man.position+5).color == "white")):
                    # En bas à droite
                    possibleMove.append(Move(man.position,man.position+11,1))
                    
                if ((self.is_man_here(man.position+4)) \
                and (not self.is_man_here(man.position+9)) \
                and (self.man_here(man.position+4).color == "white")) :
                    # En bas à gauche
                    possibleMove.append(Move(man.position,man.position+9,1))
        return possibleMove
    #def playVirtualMove(self,move):
        



if __name__ == "__main__":
    pygame.init()
    window=pygame.display.set_mode((1024,768))
    window.fill((0,0,0))
    board=Board()
    board.display(window)
    board.pieces[0].position=26
    board.pieces[1].position=27
    board.display(window)
    pygame.display.flip()
    print((board.pieces[21]).selectionner(board))
    
