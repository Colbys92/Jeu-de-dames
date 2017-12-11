#if importing fails :
#import sys
#sys.path.append('c:\\users\\matthieu\\documents\\ponts\\2A\\td log\\projet')


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

        
            
if __name__ == "__main__":
    pygame.init()
    window=pygame.display.set_mode((1024,768))
    window.fill((0,0,0))
    board=Board()
    plateau=PyBoard(board)
    # plateau.this.getPiece(0).setPosition(27)
    plateau.this.getPiece(30).setPosition(23)
    plateau.this.getPiece(31).setPosition(28)
    plateau.display(window)
    compteur=0
    couleurs=["white","black"]
    m=plateau.this.playableMoves(couleurs[compteur])
    displayMovablePieces(m,window)
    pygame.display.flip()
    
        
    
    


