#if importing fails :
#import sys
#sys.path.append('c:\\users\\matthieu\\documents\\ponts\\2A\\td log\\projet')
#sys.path.append('D:\\ENPC\\Semestre 3\\ProjetMOPSI\\Projet\\Jeu-de-dames')


from arbre import *
from pygame.locals import *
import pygame


<<<<<<< HEAD
class PyMan(Man):
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
=======
def drawKing(king,window):
    increment=0
    position=king.getPosition()
    if(position%10<5):
        increment+=50
    if king.Color()=="white":
            pygame.draw.circle(window,(255,255,255),(262+increment+100*(position%5)+25,109+50*(position//5)+20),20)
    else:
        #pygame.draw.circle(window,(255,255,255),(262+increment+100*(self.position%5)+25,109+50*(self.position//5)+25),20,2)
        pygame.draw.circle(window,(255,0,0),(262+increment+100*(position%5)+25,109+50*(position//5)+20),20)

def drawMan(man,window):
    increment=0
    position=man.getPosition()
    if(position%10<5):
        increment+=50
    if man.Color()=="white":
        window.blit(textureWhiteMan,(262+increment+100*(position%5)-5,109+50*(position//5)+20))
            # pygame.draw.circle(window,(255,255,255),(262+increment+100*(position%5)+25,109+50*(position//5)+25),20)
    else:
        #pygame.draw.circle(window,(255,255,255),(262+increment+100*(self.position%5)+25,109+50*(self.position//5)+25),20,2)
        # pygame.draw.circle(window,(255,0,0),(262+increment+100*(position%5)+25,109+50*(position//5)+25),20)
        window.blit(textureBlackMan,(262+increment+100*(position%5)-5,109+50*(position//5)+20))
    
>>>>>>> FixingKing

class PyBoard(Board):
    def __init__(self):
        super().__init__()
    def playTurn(self,window,color):
        moves=self.playableMoves(color)
        choiceMade=False
        
        while(not choiceMade):
            self.display(window)
            displayMovablePieces(moves,window)
            (x,y)=pygame.mouse.get_pos()
            pos=coordToPos(x,y)
            if(pos in moves.keys()):
                for square in moves[pos]:
                    highlightSquare(square.getArrival(),(0,0,255),window)
    def displayMovablePieces(self,m,window):
        for pos in m.keys():
            currentPiece=self.getPiece(self.index_man_here(pos))
            if(currentPiece.isMan()):
                highlightMan(pos,currentPiece.Color(),window)
            else:
                highlightKing(pos,currentPiece.Color(),window)
        
    def display(self,window):
        # pygame.draw.rect(window,(255,255,255),Rect(262,109,500,500),1)
        # for i in range(10):
        #     for j in range(10):
        #         if (i+j)%2==0:
        #             pygame.draw.rect(window,(255,255,255),Rect(262+i*50,109+j*50,50,50))
        #         else:
        #             pygame.draw.rect(window,(0,0,0),Rect(262+i*50,109+j*50,50,50))
        # pygame.draw.rect(window,(255,255,255),Rect(262,109,500,500),1)
        for i in range(self.nbPieces()):
            if(self.getPiece(i).isMan()):
                drawMan(self.getPiece(i),window)
            else:
                drawKing(self.getPiece(i),window)

def coordToPos(x,y):
    i=(x-262)//50
    j=(y-109-25)//50
    if (i+j)%2==1:
        return(5*j+i//2)
    else: return -1

def highlightSquare(position,color,window):
    increment=0
    if(position%10<5):
        increment+=50
    i=(position%5)
    j=position//5
    if color=="green":
        window.blit(textureGreenSquare,(262+i*100+increment,109+j*50+25))
    else:
        window.blit(textureBlueSquare,(262+i*100+increment,109+j*50+25))
    # pygame.draw.rect(window,color,Rect(262+i*100+increment,109+j*50+25,50,50),3)

def highlightMan(position,color,window):
    increment=0
    if(position%10<5):
        increment+=50
    i=(position%5)
    j=position//5
    if color=="white":
        window.blit(textureWhiteManSelected,(262+i*100+increment-5,109+j*50+20))
    else:
        window.blit(textureBlackManSelected,(262+i*100+increment-5,109+j*50+20))
    
def highlightKing(position,window):
    increment=0
    if(position%10<5):
        increment+=50
    i=(position%5)
    j=position//5
    # window.blit(textureWhiteManSelected,(262+i*100+increment,109+j*50+25))
    


def displayTest(movestest):
    for i in movestest.keys():
        for j in movestest[i]:
            print(j.getStart(),j.getArrival())
        

if __name__ == "__main__":
    #initialisation affichage
    pygame.init()
    window=pygame.display.set_mode((1024,768))
    textureBlackMan=pygame.image.load("textures/pion_noir_basique.png").convert_alpha()
    textureWhiteMan=pygame.image.load("textures/pion_blanc_basique.png").convert_alpha()
    textureBlackKing=pygame.image.load("textures/reine_noir_basique.png").convert_alpha()
    textureWhiteKing=pygame.image.load("textures/reine_blanche_basique.png").convert_alpha()
    textureWhiteManSelected=pygame.image.load("textures/pion_blanc_lueur.png").convert_alpha()
    textureBlackManSelected=pygame.image.load("textures/pion_noir_lueur.png").convert_alpha()
    textureBlueSquare=pygame.image.load("textures/case_sombre_bleue.png")
    textureGreenSquare=pygame.image.load("textures/case_sombre_verte.png")
    
    fond=pygame.image.load("textures/surface_jeu_V1.png").convert()
    window.blit(fond,(0,0))
    plateau=PyBoard()
    # plateau.getPiece(0).setPosition(27)
    plateau.display(window)
    
    #initialisation variable de calcul
    compteur=0
    couleurs=["white","black"]
<<<<<<< HEAD
    run=True
    while(plateau.this.nbPieces()>0 and run):
        moves=plateau.this.playableMoves(couleurs[compteur])
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
                        plateau.this.playMove(moves[chosenPiece][list(map(Move.getArrival,moves[chosenPiece])).index(pos)])
                        plateau.display(window)
                        played=True
=======
    moves=plateau.playableMoves(couleurs[compteur])
    chosenPiece=-1
    while(True):
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
            elif event.type == MOUSEBUTTONDOWN and event.button==1:
                pos=coordToPos(event.pos[0],event.pos[1])
                if chosenPiece==-1 and pos in moves.keys():
                    chosenPiece=pos
                    for square in moves[pos]:
                        highlightSquare(square.getArrival(),"green",window)
                elif chosenPiece!=-1 and pos not in map(Move.getArrival,moves[chosenPiece]):
                    chosenPiece=-1
                    plateau.display(window)
                    plateau.displayMovablePieces(moves,window)
                elif chosenPiece!=-1:
                    plateau.playMove(moves[chosenPiece][list(map(Move.getArrival,moves[chosenPiece])).index(pos)],False)
                    plateau.display(window)
                    compteur=1-compteur
                    moves=plateau.playableMoves(couleurs[compteur])
                    chosenPiece=-1
>>>>>>> FixingKing
            if(chosenPiece==-1):
                window.blit(fond,(0,0))
                plateau.display(window)
                plateau.displayMovablePieces(moves,window)
                (x,y)=pygame.mouse.get_pos()
                pos=coordToPos(x,y)
                if(pos in moves.keys()):
                    for square in moves[pos]:
                        highlightSquare(square.getArrival(),"blue",window)
            pygame.display.flip()
pygame.quit()

    


    
    
