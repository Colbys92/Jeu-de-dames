#if importing fails :
#import sys
#sys.path.append('c:\\users\\matthieu\\documents\\ponts\\2A\\td log\\projet')
#sys.path.append('D:\\ENPC\\Semestre 3\\ProjetMOPSI\\Projet\\Jeu-de-dames')

import sys
import datetime
import sqlite3
from arbre import *
from algogenetique import *
from pygame.locals import *
import pygame
import matplotlib
matplotlib.use("Agg")
import matplotlib.backends.backend_agg as agg
import pylab
import time


def drawKing(king,window):
    increment=0
    position=king.getPosition()
    if(position%10<5):
        increment+=50
    if king.Color()=="white":
        window.blit(textureWhiteKing,(262+increment+100*(position%5)-5,109+50*(position//5)+20))
    else:
        window.blit(textureBlackKing,(262+increment+100*(position%5)-5,109+50*(position//5)+20))

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
        for i in range(self.nbPieces()):
            if(self.getPiece(i).isMan()):
                drawMan(self.getPiece(i),window)
            else:
                drawKing(self.getPiece(i),window)

class MainMenu():
    buttonWidth=610
    buttonHeight=155
    buttonAbs=207
    buttonOrd1=72
    buttonOrd2=310
    buttonOrd3=542
    def __init__(self):
        self.state=1 #1 for gameMode selection, 2 for number of players selection, 3 for difficulty
        self.window=pygame.display.set_mode((1024,768))
        self.textureMenu1=pygame.image.load("textures/menu/menu.png").convert_alpha()
        self.textureMenu2=pygame.image.load("textures/menu/menu_1.1.png").convert_alpha()
        self.textureMenu3=pygame.image.load("textures/menu/menu_1.1.2.png").convert_alpha()
    def run(self):
        choiceMade=False
        while(not choiceMade):
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()
                if event.type==KEYDOWN and event.key==K_ESCAPE and self.state>1:
                    self.state-=1
                if event.type == MOUSEBUTTONDOWN and event.button==1:
                    (x,y)=pygame.mouse.get_pos()
                    if coordInRect((x,y),(self.buttonAbs,self.buttonOrd1,self.buttonWidth,self.buttonHeight)):
                        print(1)
                        if(self.state==3):
                            #learningMode
                            choiceMade=True
                        else:
                            self.state+=1
                    elif coordInRect((x,y),(self.buttonAbs,self.buttonOrd2,self.buttonWidth,self.buttonHeight)):
                        choiceMade=True
                        print(2)
                        if(self.state==1):
                            choiceMade=True
                            #replay
                        elif(self.state==2):
                            choiceMade=True
                            #player vs player
                        else:
                            choiceMade=True
                            #difficulty moyen
                    elif coordInRect((x,y),(self.buttonAbs,self.buttonOrd3,self.buttonWidth,self.buttonHeight)):
                        choiceMade=True
                        if(self.state==1):
                            pygame.quit()
                        elif(self.state==2):
                            choiceMade=True
                            #IA vs IA
                        else:
                            choiceMade=True
                            #difficile
            self.display()
    def display(self):
        if(self.state==1):
            self.window.blit(self.textureMenu1,(0,0))
        elif(self.state==2):
            self.window.blit(self.textureMenu2,(0,0))
        else:
            self.window.blit(self.textureMenu3,(0,0))
        pygame.display.flip()
 
 
class GameWindow(Board):
    def __init__(self):
        a=0
 
 
 
 
 
def coordInRect(pos,rect):
    #check whether pos is within the rectangle rect
    return not (pos[0]<rect[0] or pos[0]>rect[0]+rect[2] or pos[1]<rect[1] or pos[1]>rect[1]+rect[3])


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
    
def highlightKing(position,color,window):
    increment=0
    if(position%10<5):
        increment+=50
    i=(position%5)
    j=position//5
    if color=="white":
        window.blit(textureWhiteKingSelected,(262+i*100+increment-5,109+j*50+20))
    else:
        window.blit(textureBlackKingSelected,(262+i*100+increment-5,109+j*50+20))
    


def displayTest(movestest):
    for i in movestest.keys():
        for j in movestest[i]:
            print(j.getStart(),j.getArrival())

def loadMoveList(gameId):
    #renvoie la liste des coups joués dans la partie numérotée gameId de la base de données 
    #on suppose que id est unique
    conn=sqlite3.connect('example.db')
    c=conn.cursor()
    for row in c.execute("SELECT listeCoups FROM games WHERE id=?",(gameId,)):
        #on utilise la fonction eval... Qui peut conduire à des problèmes de sécurité.. ON s'autorise à le faire vu le cadre du projet.
        conn.close()
        return eval(row[0])

def updateGraphSize(valueList,ax):
    if ax.get_ylim()[0]>listeValeurs[-1]:
            ax.set_ylim(ax.get_ylim()[0]-10,ax.get_ylim()[1])
    if ax.get_ylim()[1]<listeValeurs[-1]:
        ax.set_ylim(ax.get_ylim()[0],ax.get_ylim()[1]+10)
    if ax.get_xlim()[1]<len(listeValeurs):
        ax.set_xlim(0,ax.get_xlim()[1]+10)

def drawGraph(window,fig):
    canvas=agg.FigureCanvasAgg(fig)
    canvas.draw()
    renderer=canvas.get_renderer()
    raw_data=renderer.tostring_rgb()
    size=canvas.get_width_height()
    surf=pygame.image.fromstring(raw_data,size,"RGB")
    screen.blit(surf,(1024,184))
    pygame.display.flip()

if __name__ == "__main__":
    pygame.init()
    menu=MainMenu()
    menu.run()
    
    #on charge la base données
    conn=sqlite3.connect('example.db')
    c=conn.cursor()
    #initialisation affichage
    pygame.init()
    window=pygame.display.set_mode((1424,768))
    screen=pygame.display.get_surface()
    textureBlackMan=pygame.image.load("textures/pion_noir_basique.png").convert_alpha()
    textureWhiteMan=pygame.image.load("textures/pion_blanc_basique.png").convert_alpha()
    textureBlackKing=pygame.image.load("textures/reine_noir_basique.png").convert_alpha()
    textureWhiteKing=pygame.image.load("textures/reine_blanche_basique.png").convert_alpha()
    textureWhiteManSelected=pygame.image.load("textures/pion_blanc_lueur.png").convert_alpha()
    textureBlackManSelected=pygame.image.load("textures/pion_noir_lueur.png").convert_alpha()
    textureWhiteKingSelected=pygame.image.load("textures/reine_blanche_lueur.png").convert_alpha()
    textureBlackKingSelected=pygame.image.load("textures/reine_noir_lueur.png").convert_alpha()
    textureBlueSquare=pygame.image.load("textures/case_sombre_bleue.png")
    textureGreenSquare=pygame.image.load("textures/case_sombre_verte.png")
    #bots=[Individu(0,1,5,2,0,0,0),Individu(0,2,5,2,0.5,0.5,0.5),Individu(0,2,5,4,0.5,0.5,0.5)] #différents niveaux de difficulté
    
    #choix du type de partie : 0 pour 2 joueurs, 1 pour JvIA, 2 pour IAvIA, 3 pour regarder une partie, 4 learning Mode
    gameType=4
    #choix du niveau de difficulté
    difficulty=1
    

   #   
    fond=pygame.image.load("textures/surface_jeu_V1.png").convert()
    window.blit(fond,(0,0))
    plateau=PyBoard()
    plateau.display(window)
    
    
    fig = pylab.figure(figsize=[4, 4], dpi=100,)
    ax = fig.gca()
    ax.set_xlim(0,20)
    ax.set_ylim(-20,20)
    
    
    #initialisation variable de calcul
    compteur=0
    forceQuit=False
    couleurs=["white","black"]
    moves=plateau.playableMoves(couleurs[compteur])
    listeCoups=[]
    listeValeurs=[]
    chosenPiece=-1

   #   window.blit(fond,(0,0))
    plateau.display(window)
    pygame.display.flip()
    
    if(gameType==2):
        while(not plateau.endGame()):
            pygame.event.pump()
            window.blit(fond,(0,0))
            plateau.display(window)
            time.sleep(0.1)
            pygame.display.update()
            if compteur==1 :
                move = getSecond(plateau.bestMoveAlphaBeta2(couleurs[compteur],4,1,1,0,0,0,True,-1000,1000))
            else :
                move = getSecond(plateau.bestMoveAlphaBeta2(couleurs[compteur],6,1,1,0,0,0,True,-1000,1000))
            plateau.playMove(move,False)
            moves=plateau.playableMoves(couleurs[compteur])
            compteur=1-compteur
        
    elif(gameType<=1 or gameType==4):
        while(not plateau.endGame() and not forceQuit):
            for event in pygame.event.get():
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
                #plt.show()
                if event.type == QUIT:
                    pygame.quit()
                    forceQuit=True
                    break
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
                        move=moves[chosenPiece][list(map(Move.getArrival,moves[chosenPiece])).index(pos)]
                        #on enregistre le move sous la forme d'une liste d'entiers (positions par lesquelles le pion passe pour jouer)
                        listeCoups.append(([move.getPath()[i] for i in range(len(move.getPath()))],move.getKills()))
                        plateau.playMove(move,False)
                        plateau.display(window)
                        if gameType==0:
                            compteur=1-compteur
                            moves=plateau.playableMoves(couleurs[compteur])
                        elif (gameType==1 or gameType==4) and not plateau.endGame():
                            window.blit(fond,(0,0))
                            plateau.display(window)
                            pygame.display.flip()
                            if(gameType==4):
                                listeValeurs.append(plateau.evaluateBetter(1,5,0.1,0.3,0.2,"white"))
                            move=getSecond(plateau.bestMoveAlphaBeta2(couleurs[1-compteur],6,5,10,0,0,0,True,-10000,10000))
                            listeCoups.append(([move.getPath()[i] for i in range(len(move.getPath()))],move.getKills()))
                            plateau.playMove(move,False)
                            moves=plateau.playableMoves(couleurs[compteur])
                        chosenPiece=-1
            if(gameType==4 and len(listeValeurs)>1):
                ax.plot(listeValeurs,'b-')
                updateGraphSize(listeValeurs,ax)
                drawGraph(window,fig)
        stringMove=listeCoups.__str__()
        stringDate=datetime.date.today().__str__()
        c.execute("INSERT INTO games VALUES (10,?,?)",(stringDate,stringMove))
        conn.commit()
        conn.close()
    elif(gameType==3):
        gameId=10 #demander à l'utilisateur de choisir la partie qu'il veut regarder
        moveList=loadMoveList(gameId)
        waitingTime=1000 #temps d'attente entre 2 coups en millisecondes
        pause=False
        while len(moveList)>0:
            #rajouter affichage pause (pour comprendre que c'est pas buggé) + vitesse actuelle + pouvoir jouer coup par coup avec un clic quand on a mis le jeu en pause
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    forceQuit=True
                    break
                if event.type == KEYDOWN and event.key==K_1:
                    if(waitingTime==250):
                        waitingTime=1000
                    else:
                        waitingTime-=250
                if event.type == KEYDOWN and event.key==K_2s:
                    pause=not pause
                if event.type == KEYDOWN and event.key==K_SPACE:
                    if(pause):
                        plateau.playMove(Move(vectori(moveList[0][0]),moveList[0][1]),False)
                        moveList=moveList[1:]
                    else:
                        pause=True
            if( not pause):
                plateau.playMove(Move(vectori(moveList[0][0]),moveList[0][1]),False)
                moveList=moveList[1:]
            window.blit(fond,(0,0))
            plateau.display(window)
            pygame.display.flip()
            pygame.time.delay(waitingTime)

    #   pygame.quit()




    
    
