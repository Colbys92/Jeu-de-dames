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
import random


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
    else:
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
        self.textureMenu4=pygame.image.load("textures/menu/menu_choix-pions_V1.png")
    def run(self):
        choiceMade=False
        while(not choiceMade):
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    choiceMade=True
                    break
                if event.type==KEYDOWN and event.key==K_ESCAPE and self.state>1:
                    self.state-=1
                if event.type == MOUSEBUTTONDOWN and event.button==1:
                    (x,y)=pygame.mouse.get_pos()
                    if coordInRect((x,y),(self.buttonAbs,self.buttonOrd1,self.buttonWidth,self.buttonHeight)):
                        if(self.state==3):
                            #learningMode
                            choiceMade=True
                            return(LearningGameWindow(Individu(0,1,5,4,0,0,0)))
                        elif self.state==4:
                            possibleGameMod=[IAvPGameWindow(Individu(0,1,3,4,0.1,0.2,0.3)),IAvPGameWindow(Individu(0,1,5,6,0,0,0))]
                            return(possibleGameMod[self.difficulty])
                        else:
                            self.state+=1
                    elif coordInRect((x,y),(self.buttonAbs,self.buttonOrd2,self.buttonWidth,self.buttonHeight)):
                        if(self.state==1):
                            choiceMade=True
                            #choisir gameID
                            conn=sqlite3.connect('example.db')
                            c=conn.cursor()
                            #on ne peut que regarder la dernière partie enregistrée avec cette version
                            gameID=c.execute("SELECT Count(id) FROM games").fetchone()[0]-1
                            print(gameID)
                            conn.close()
                            return(SpectatorWindow(gameID))
                            #replay
                        elif(self.state==2):
                            choiceMade=True
                            return(PvPGameWindow())
                            #player vs player
                        elif self.state==3:
                            self.difficulty=0 #normal difficulty
                            self.state+=1
                        else:
                            choiceMade=True
                            possibleGameMod=[IAvPGameWindow(Individu(0,1,3,4,0.1,0.2,0.3),"black"),IAvPGameWindow(Individu(0,1,5,6,0,0,0),"black")]
                            return(possibleGameMod[0])
                            #normal or difficult, black side
                    elif coordInRect((x,y),(self.buttonAbs,self.buttonOrd3,self.buttonWidth,self.buttonHeight)):
                        if(self.state==1):
                            choiceMade=True
                            pygame.quit()
                            break
                        elif(self.state==2):
                            choiceMade=True
                            return(IAvIAGameWindow(Individu(0,1,5,4,0.1,0.2,0.3),Individu(0,1,5,4,0.1,0.2,0.3)))
                            #IA vs IA
                            #pour la partie IAvIA il faudrait générer aléatoirement les valeurs des coefficients pour la fonction de coût pour pouvoir assister à différentes parties
                        elif self.state==3:
                            self.difficulty=1
                            self.state+=1
                        else:
                            color=colors[random.randint(0,1)]
                            choiceMade=True
                            if self.difficulty==0:
                                return(IAvPGameWindow(Individu(0,1,5,4,0.1,0.2,0.3),color))
                            else:
                                return(IAvPGameWindow(Individu(0,1,5,6,0,0,0),color))
            if not choiceMade:                    #choix de la couleur alléatoire
                self.display()
    def display(self):
        if(self.state==1):
            self.window.blit(self.textureMenu1,(0,0))
        elif(self.state==2):
            self.window.blit(self.textureMenu2,(0,0))
        elif(self.state==3):
            self.window.blit(self.textureMenu3,(0,0))
        else:
            self.window.blit(self.textureMenu4,(0,0))
        pygame.display.flip()

class GameWindow():
    def __init__(self,resolution):
        self.board=PyBoard()
        pygame.init()
        self.window=pygame.display.set_mode(resolution)
        self.moves=self.board.playableMoves("white")
        self.forceQuit=False
        self.player=0 #next player to play, 0 for white 1 for black
        self.moveList=[]
        
    def display(self):
        self.window.blit(fond,(0,0))
        self.board.display(self.window)
    
    def playSmooth(self,move):
        self.moveList.append(([move.getPath()[i] for i in range(len(move.getPath()))],move.getKills())) #we save the played move. we'll later store it in our database
        piece=self.board.index_man_here(move.getPath()[0])
        #play slowly the move to make clear how the move goes
        if len(vectori(move.getPath()))>2:
            for i in move.getPath():
                time.sleep(0.2)
                self.board.getPiece(piece).setPosition(i)
                self.display()
                pygame.display.flip()
        self.board.getPiece(piece).setPosition(move.getPath()[0])
        self.board.playMove(move,False)
        self.display()
        pygame.display.flip()

    def highlightPossibleMoves(self,event):
        self.board.displayMovablePieces(self.moves,self.window)
        (x,y)=pygame.mouse.get_pos()
        pos=coordToPos(x,y)
        if pos in self.moves.keys():
            for square in self.moves[pos]:
                highlightSquare(square.getArrival(),"blue",window)

    def selectPiece(self,pos):
        if pos in self.moves.keys():
            for square in self.moves[pos]:
                highlightSquare(square.getArrival(),"green",self.window)
            self.chosenPiece=pos

    def selectMove(self,pos):
        if pos not in map(Move.getArrival,self.moves[self.chosenPiece]):
            self.chosenPiece=-1
        else:
            move=self.moves[self.chosenPiece][list(map(Move.getArrival,self.moves[self.chosenPiece])).index(pos)]
            self.playSmooth(move)
            self.display()
            self.moveChosen=True
    
    def saveGame(self):
        conn=sqlite3.connect('example.db')
        c=conn.cursor()
        stringMove=self.moveList.__str__()
        stringDate=datetime.date.today().__str__()
        gameNumber=c.execute("SELECT Count(id) FROM games").fetchone()[0]
        c.execute("INSERT INTO games VALUES (?,?,?)",(gameNumber,stringDate,stringMove))
        conn.commit()
        conn.close()

    def inputMove(self):
        self.chosenPiece=-1
        self.moveChosen=False
        while not self.moveChosen and not self.forceQuit:
            for event in pygame.event.get():
                if event.type == QUIT:
                    self.forceQuit=True
                    pygame.quit()
                    break
                elif event.type==MOUSEBUTTONDOWN and event.button == 1:
                    pos=coordToPos(event.pos[0],event.pos[1])
                    if self.chosenPiece==-1:
                        self.selectPiece(pos)
                    else:
                        self.selectMove(pos)
                elif self.chosenPiece==-1:
                    self.display()
                    self.highlightPossibleMoves(event)
                pygame.display.flip()

class SpectatorWindow(GameWindow):
    def __init__(self,gameID):
        GameWindow.__init__(self,(1024,768))
        self.moveList=[]
        self.waitingTime=1000 #time between each move (in milliseconds)
        self.gameID=gameID
        self.pause=False
    
    def display(self):
        GameWindow.display(self)
        self.window.blit(stepButton,(385,685))
        if(self.pause):
            self.window.blit(pauseButtonActivated,(485,685))
        else:
            self.window.blit(pauseButton,(485,685))
        self.window.blit(fastForward,(585,685))
        pygame.display.flip()
        

    def loadMoveList(self,gameId):
        #renvoie la liste des coups joués dans la partie numérotée gameId de la base de données 
        #on suppose que id est unique
        conn=sqlite3.connect('example.db')
        c=conn.cursor()
        for row in c.execute("SELECT listeCoups FROM games WHERE id=?",(gameId,)):
            #on utilise la fonction eval... Qui peut conduire à des problèmes de sécurité.. ON s'autorise à le faire vu le cadre du projet.
            conn.close()
            self.moveList= eval(row[0])

    def run(self):
        self.moveList=loadMoveList(self.gameID)
        self.pause=False
        compteur=0
        while compteur<len(self.moveList) and not self.forceQuit:
            for event in pygame.event.get():
                if event.type == QUIT:
                    self.forceQuit=True
                    pygame.quit()
                    break
                if (event.type == KEYDOWN and event.key==K_1) or (event.type==MOUSEBUTTONDOWN and event.button == 1 and coordInRect(event.pos,(585,685,50,36))):
                    if(self.waitingTime==250):
                        self.waitingTime=1000
                    else:
                        self.waitingTime-=250
                if (event.type == KEYDOWN and event.key==K_2) or (event.type==MOUSEBUTTONDOWN and event.button == 1 and coordInRect(event.pos,(485,685,50,36))):
                    self.pause= not self.pause
                    self.display()
                if (event.type == KEYDOWN and event.key==K_SPACE) or (event.type==MOUSEBUTTONDOWN and event.button == 1 and coordInRect(event.pos,(385,685,50,36))):
                    if(self.pause):
                        self.playSmooth(Move(vectori(self.moveList[0][0]),self.moveList[0][1]))
                        self.moveList=self.moveList[1:]
                        compteur+=1
                    else:
                        self.pause=True
            if(not self.pause and compteur<len(self.moveList) and not self.forceQuit):
                self.playSmooth(Move(vectori(self.moveList[0][0]),self.moveList[0][1]))
                self.moveList=self.moveList[1:]
                compteur+=1
            pygame.time.delay(self.waitingTime)
        pygame.quit()


class PvPGameWindow(GameWindow): #player vs Player
    def __init__(self):
        GameWindow.__init__(self,(1024,768))
        self.movesList=[]
    def run(self):
        while not self.forceQuit or self.board.endGame():
            self.inputMove()
            self.player=1-self.player
            self.moves=self.board.playableMoves(colors[self.player])
        self.saveGame()
        pygame.quit()

class IAvPGameWindow(GameWindow): #player vs IA
    def __init__(self,individu,colorPlayer="white"): #colorPlayer is the color chosen by the Player
        GameWindow.__init__(self,(1024,768))
        self.individu=individu
        self.colorPlayer=colorPlayer

    def playBestMove(self,individu):
        move=individu.bestMoveAlphaBeta(self.board,colors[self.player],-100000,100000)
        self.playSmooth(move)

    def run(self):
        while not self.forceQuit and not self.board.endGame():
            if self.colorPlayer==colors[self.player]:
                self.inputMove()
                self.player=1-self.player
                self.moves=self.board.playableMoves(colors[self.player])
            else:
                self.playBestMove(self.individu)
                self.player=1-self.player
                self.moves=self.board.playableMoves(colors[self.player])
        self.saveGame()
        pygame.quit()

class IAvIAGameWindow(IAvPGameWindow):
    def __init__(self,individu1,individu2):
        IAvPGameWindow.__init__(self,individu1)
        self.individu2=individu2
        
    def run(self):
        while not self.forceQuit and not self.board.endGame():
            pygame.time.wait(500)
            if self.player==0:
                self.playBestMove(self.individu)
            else:
                self.playBestMove(self.individu2)
            self.player=1-self.player
            self.moves=self.board.playableMoves(colors[self.player])
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    self.forceQuit=True
                    pygame.quit()
                    break
        pygame.quit()

class LearningGameWindow(IAvPGameWindow):
    def __init__(self,individu,colorPlayer="white"):
        IAvPGameWindow.__init__(self,individu,colorPlayer)
        GameWindow.__init__(self,(1424,768))
        self.evaluationList=[]
        self.fig = pylab.figure(figsize=[4, 4], dpi=100,)
        self.ax=self.fig.gca()
        self.ax.set_xlim(0,20)
        self.ax.set_ylim(-20,20)
        self.screen = pygame.display.get_surface()
        
    
    def display(self):
        self.window.blit(fondLarge,(0,0))
        self.board.display(self.window)
        if(len(self.evaluationList)>0):
            self.ax.plot(self.evaluationList,'b-')
            updateGraphSize(self.evaluationList,self.ax)
            drawGraph(window,self.fig,self.screen)
        self.window.blit(helpButton,(485,685))
        
    def run(self):
        while not self.forceQuit and not self.board.endGame():
            if self.colorPlayer==colors[self.player]:
                self.chosenPiece=-1
                self.moveChosen=False
                while not self.moveChosen and not self.forceQuit:
                    for event in pygame.event.get():
                        if event.type == QUIT:
                            self.forceQuit=True
                            pygame.quit()
                            break
                        elif event.type==MOUSEBUTTONDOWN and event.button == 1:
                            if coordInRect(event.pos,(485,685,50,36)):
                                move=self.individu.bestMoveAlphaBeta(self.board,colors[self.player],-100000,100000)
                                piece=self.board.index_man_here(move.getPath()[0])
                                #play slowly the move to make clear what the best move is
                                for i in move.getPath():
                                    self.board.getPiece(piece).setPosition(i)
                                    self.display()
                                    pygame.display.flip()
                                    time.sleep(1)
                                self.board.getPiece(piece).setPosition(move.getPath()[0])
                            else:
                                pos=coordToPos(event.pos[0],event.pos[1])
                                if self.chosenPiece==-1:
                                    self.selectPiece(pos)
                                else:
                                    self.selectMove(pos)
                        elif self.chosenPiece==-1:
                            self.display()
                            self.highlightPossibleMoves(event)
                        pygame.display.flip()
                self.player=1-self.player
                self.moves=self.board.playableMoves(colors[self.player])
                self.evaluationList.append(self.board.evaluateBetter(self.individu.getManWeight(),self.individu.getKingWeight(),self.individu.getNbMoveWeight(),self.individu.getAdvancementForwardWeight(),self.individu.getCentralWeight(),self.colorPlayer))
            else:
                self.playBestMove(self.individu)
                self.player=1-self.player
                self.moves=self.board.playableMoves(colors[self.player])
        pygame.quit()


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
    if ax.get_ylim()[0]>valueList[-1]:
            ax.set_ylim(ax.get_ylim()[0]-10,ax.get_ylim()[1])
    if ax.get_ylim()[1]<valueList[-1]:
        ax.set_ylim(ax.get_ylim()[0],ax.get_ylim()[1]+10)
    if ax.get_xlim()[1]<len(valueList):
        ax.set_xlim(0,ax.get_xlim()[1]+10)

def drawGraph(window,fig,screen):
    canvas=agg.FigureCanvasAgg(fig)
    canvas.draw()
    renderer=canvas.get_renderer()
    raw_data=renderer.tostring_rgb()
    size=canvas.get_width_height()
    surf=pygame.image.fromstring(raw_data,size,"RGB")
    screen.blit(surf,(874,184))
    pygame.display.flip()

if __name__ == "__main__":
    pygame.init()
    window=pygame.display.set_mode((1024,768))
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
    fond=pygame.image.load("textures/surface_jeu_V1.png").convert()
    fondLarge=pygame.image.load("textures/surface_jeu_etendue.png").convert()
    fastForward=pygame.image.load("textures/buttons/fastforward_button_V3.png").convert_alpha()
    pauseButton=pygame.image.load("textures/buttons/pause_button_V3.png").convert_alpha()
    pauseButtonActivated=pygame.image.load("textures/buttons/pause_button_V3_shadow.png").convert_alpha()
    stepButton=pygame.image.load("textures/buttons/step_button_V3.png").convert_alpha()
    helpButton=pygame.image.load("textures/buttons/help_button_V3.png").convert_alpha()
    
    colors=["white","black"]
    
    menu=MainMenu()
    gamewindow=menu.run()
    if gamewindow!=None:
        gamewindow.run()
    





    
    
