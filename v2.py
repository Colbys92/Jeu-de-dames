#if importing fails :
#import sys
#sys.path.append('c:\\users\\matthieu\\documents\\ponts\\2A\\td log\\projet')
#sys.path.append('D:\\ENPC\\Semestre 3\\ProjetMOPSI\\Projet\\Jeu-de-dames')


from arbre import *
from pygame.locals import *
import pygame


class PyMan:
    def __init__(self,man):
        self.this=man
    def display(self,window):
        increment=0
        position=self.this.getPosition()
        if(position%10<5):
            increment+=50
        if self.this.Color()=="white":
                pygame.draw.circle(window,(255,255,255),(262+increment+100*(position%5)+25,109+50*(position//5)+25),20)
        else:
            #pygame.draw.circle(window,(255,255,255),(262+increment+100*(self.position%5)+25,109+50*(self.position//5)+25),20,2)
            pygame.draw.circle(window,(255,0,0),(262+increment+100*(position%5)+25,109+50*(position//5)+25),20)

class PyBoard:
    def __init__(self,board):
        self.this=board
    def playTurn(self,window,color):
        moves=self.this.playableMoves(color)
        choiceMade=False
        
        while(not choiceMade):
            self.display(window)
            displayMovablePieces(moves,window)
            (x,y)=pygame.mouse.get_pos()
            pos=coordToPos(x,y)
            if(pos in moves.keys()):
                for square in moves[pos]:
                    highlightSquare(square.getArrival(),(0,0,255),window)
            
            
            
            
        
    def display(self,window):
        pygame.draw.rect(window,(255,255,255),Rect(262,109,500,500),1)
        for i in range(10):
            for j in range(10):
                if (i+j)%2==0:
                    pygame.draw.rect(window,(255,255,255),Rect(262+i*50,109+j*50,50,50))
                else:
                    pygame.draw.rect(window,(0,0,0),Rect(262+i*50,109+j*50,50,50))
        pygame.draw.rect(window,(255,255,255),Rect(262,109,500,500),1)
        for i in range(self.this.nbPieces()):
            PyMan(self.this.getPiece(i)).display(window)

def coordToPos(x,y):
    i=(x-262)//50
    j=(y-109)//50
    if (i+j)%2==1:
        return(5*j+i//2)
    else: return -1

def highlightSquare(position,color,window):
    increment=0
    if(position%10<5):
        increment+=50
    i=(position%5)
    j=position//5
    pygame.draw.rect(window,color,Rect(262+i*100+increment,109+j*50,50,50),3)

def displayMovablePieces(m,window):
    for pos in m.keys():
        highlightSquare(pos,(0,255,0),window)

def displayTest(movestest):
    for i in movestest.keys():
        for j in movestest[i]:
            print(j.getStart(),j.getArrival())
        

if __name__ == "__main__":
    pygame.init()
    window=pygame.display.set_mode((1024,768))
    window.fill((0,0,0))

    board=Board()

    plateau=PyBoard(board)

    plateau.this.getPiece(0).setPosition(27)
    plateau.display(window)
    compteur=0
    possibleMoves=VectorMove()

    couleurs=["white","black"]
    run=True
    print(plateau.this.playableMoves("white"))

    # plateau.this.getPiece(23).setPosition(29)
    # plateau.this.getPiece(28).killFreeMove(plateau,possibleMoves)

    
    # for i in range(36,0,-1) :
        # print(i)
        # plateau.this.killAt(plateau.this.getPiece(i).getPosition())
    #   
    # plateau.this.getPiece(0).setPosition(47)
    # plateau.this.getPiece(1).setPosition(41)
    # plateau.this.getPiece(2).setPosition(42)
    # plateau.this.getPiece(3).setPosition(36)



    
    # bestmove = plateau.this.bestMove(plateau.this.playableMoves("white"),"white",2,1,1)
    # print(bestmove.first)
    while(plateau.this.nbPieces()>0 and run):
        print("debut")
        moves=plateau.this.playableMoves(couleurs[compteur])
        displayTest(moves)
        print("Fin")
        played=False
        chosenPiece=-1
        while(not played):
            for event in pygame.event.get():
                if event.type == QUIT:
                    run=False
                    played=True
                    choiceMade=True
                    break
                elif event.type == MOUSEBUTTONDOWN and event.button==1:
                    pos=coordToPos(event.pos[0],event.pos[1])
                    if chosenPiece==-1 and pos in moves.keys():
                        chosenPiece=pos
                        for square in moves[pos]:
                            highlightSquare(square.getArrival(),(255,0,0),window)
                    elif chosenPiece!=-1 and pos not in map(Move.getArrival,moves[chosenPiece]):
                        chosenPiece=-1
                        plateau.display(window)
                        displayMovablePieces(moves,window)
                    elif chosenPiece!=-1:
                        print("pasjoué")
                        plateau.this.playMove(moves[chosenPiece][list(map(Move.getArrival,moves[chosenPiece])).index(pos)])
                        plateau.display(window)
                        played=True
                        print("joué")
            if(chosenPiece==-1):
                print("1")
                plateau.display(window)
                displayMovablePieces(moves,window)
                (x,y)=pygame.mouse.get_pos()
                pos=coordToPos(x,y)
                if(pos in moves.keys()):
                    for square in moves[pos]:
                        highlightSquare(square.getArrival(),(0,0,255),window)
            print("2")
            pygame.display.flip()
        print("3")
        compteur=1-compteur
        print("sortie")
pygame.quit()

    


    
    
