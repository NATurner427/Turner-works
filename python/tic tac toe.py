#!/usr/bin/env python
# coding: utf-8

# In[1]:


from IPython.display import clear_output

def display_board(board):
    print('   |   |   ')
    print(f' {board[6]} | {board[7]} | {board[8]}')
    print('   |   |   ')
    print('### ### ###')
    print('   |   |   ')
    print(f' {board[3]} | {board[4]} | {board[5]}')
    print('   |   |   ')
    print('### ### ###')
    print('   |   |   ')
    print(f' {board[0]} | {board[1]} | {board[2]}')
    print('   |   |   ')


# In[2]:


def player_input():
    check = False
    global player1
    global player2
    while check == False:
        player1=input("Player one, do you want to be X or O? ")
        #check
        player1=player1.upper()
        if player1=='X'or player1 == 'O':
            check=True
        else:
            print('please pick X or O!')
            check=False
    if player1=='X':
        player2='O'
    else:
        player2='X'

    print(f'Player one selected {player1}')
    print(f'Player two will play as {player2}') 


# In[3]:


def place_marker(board, marker, position):
    board[position-1]=marker


# In[4]:


def win_check(board, marker):
    n=0
    # check horizontal
    while n <=6:
        if n == 0 or n == 3 or n == 6:
            if board[n]== marker and board[n+1]==marker and board[n+2]==marker:
                return True
        n+=1
    n=0
    # check vertical
    while n <=2:
        if board[n]==marker and board[n+3]==marker and board[n+6]==marker:
            return True
        n+=1
    n=0
    # check diagonal
    if board[0]==marker and board[4]==marker and board[8]==marker:
        return True
    if board[6]== marker and board[4]== marker and board[2]==marker:
        return True
    return False


# In[5]:


import random

def choose_first():
    global onefirst
    coin_flip=random.randint(0,1)
    if coin_flip ==1:
        print('Coin flip has decided that player one will go first')
        onefirst=True
    else:
        print('Coin flip has decided that player two will go first')
        onefirst=False


# In[6]:


def space_check(board, position):
    return board[position-1] ==' '


# In[7]:


def full_board_check(board):
    for x in board:
        if x == ' ':
            return False
    return True


# In[8]:


def player_choice(board):
    global position
    check = True
    while check==True:
        position=int(input('make a move on the keypad 1-9'))
        if position <=9 and position >= 1:
            if space_check(board,position) == True:
                check = False
                return position
            else:
                print('This is not a free position')
                check = True
        else:
            print('please pick a number between 1 and 9')


# In[9]:


def replay():
    again= False
    while again==False:
        prompt=(input('Would you like to play again? ')).lower()
        if prompt == 'yes' or prompt == 'y' or prompt == 'ye':
            print('ok!')
            return True
            again = True
        elif prompt == 'no' or prompt =='n':
            print('Thanks for playing!')
            return False
            again = True
        else:
            print('please say yes or no')
            again = False


# In[10]:


import random
import time

again = True
print('Welcome to Tic Tac Toe!')
while again==True:
# Set up the board
    board=[' ',' ',' ',' ',' ',' ',' ',' ',' ']
    # players choose their marker
    player_input()
    choose_first()
    time.sleep(3)
    game_on=True
    while game_on ==True:
        while onefirst == True:
            clear_output()
            display_board(board)
            print("Player 1, its your turn")
            player_choice(board)
            place_marker(board,player1,position)
            if win_check(board,player1) == True:
                game_on= False
                break
            if full_board_check(board) == True:
                game_on= False
                break
            onefirst=False
        while onefirst == False:
            clear_output()
            display_board(board)
            print("Player 2, its your turn")
            player_choice(board)
            place_marker(board,player2,position)
            if win_check(board,player2) == True:
                game_on= False
                break
            if full_board_check(board) == True:
                game_on= False
                break
            onefirst=True
    if onefirst== True and win_check(board,player1) == True:
        clear_output()
        display_board(board)
        print("Player 1 wins!")
    elif onefirst == False and win_check(board,player1) == True:
        clear_output()
        display_board(board)
        print("Player 2 wins!")
    elif full_board_check(board)== True:
        clear_output()
        display_board(board)
        print("stalemate!")
    
    
    if replay() == True:
        again = True
    else:
        again = False
        break


# ## Good Job!

# In[ ]:




