# -*- coding: utf-8 -*-
"""
Created on Sun Oct 13 09:25:04 2019

@author: 16142
"""

import random

suits = ('Clubs', ' Diamonds', 'Hearts', 'Spades')
ranks = ('Two', 'Three', 'Four', 'Five', 'Six', 'Seven', 'Eight', 'Nine', 'Ten', 'Jack', 'Queen', 'King', 'Ace')
values = {'Two':2, 'Three':3, 'Four':4, 'Five':5, 'Six':6, 'Seven':7, 'Eight':8, 'Nine':9, 'Ten':10, 'Jack':10, 'Queen':10, 'King':10, 'Ace':11}

playing = True

class Card:
    
    def __init__(self,suits,ranks):
        self.suits = suits
        self.ranks = ranks
    def __str__(self):
        return f"{self.ranks} of {self.suits}"
    
        
class Deck:
    def __init__(self):
        self.deck=[]
        for suit in suits:
            for rank in ranks:
                self.deck.append(Card(suit,rank))
            
    def __str__(self):
        this_deck = ''
        for card in self.deck:
            this_deck += f'\n {card.__str__()}'
        return f"This deck has : {this_deck}"
    
    def shuffle(self):
        random.shuffle(self.deck)
        
    def deal(self):
        a_card=self.deck.pop()
        return a_card


class Hand:
    
    def __init__(self):
        self.cards=[]
        self.value = 0
        self.aces=0
    
    def add_card(self,card):
        self.cards.append(card)
        self.value+= values[card.ranks]
        if card.ranks == 'Ace':
            self.aces += 1
    
    def ace_check(self):
        while self.value > 21 and self.aces:
            self.value -= 10
            self.aces -= 1
    
    
class Chips:    
    
    def __init__(self):
        self.total =100
        self.bet =0
        
    def win(self):
        self.total += self.bet
    
    def lose(self):
        self.total -= self.bet
     
        
        
def take_bet(chips):
    bet = True
    while  bet == True:
        try:
            chips.bet=int(input("How much you bettin'?"))
        except ValueError:
            print('Bet must be in whole chips!')
        else:
            if chips.bet > chips. total:
                print(f"You can't bet more than you have. \n You have {chips.total} and you bet {chips.bet}")
                bet =True
            else:
                print(f'you bet {chips.bet}')
                break
       

def hit(deck,hand):
    hand.add_card(deck.deal())
    hand.ace_check()


def hit_or_stand(deck,hand):
    global playing
    
    while True:
        q = input('Hit or Stand? entery "h" or "s" ')
        
        if q[0].lower() == 'h':
            hit(deck,hand)
        
        elif q[0].lower() == 's':
            print("Player stands. Dealers turn")
            playing = False
        
        else:
            print('Please type "h" or "s"')
            continue
        break
    

def show_some(player,dealer):
    print("\nDealer's Hand:\n")
    print("face down card\n")
    print(f"{dealer.cards[1]}")
    print("\nPlayer's Hand:\n", *player.cards, sep='\n ')

def show_all(player,dealer):
    print("\nDealer's Hand:\n", *dealer.cards, sep='\n ')
    print(f"Dealer's Hand is at {dealer.value}")
    print("\nPlayer's Hand:\n", *player.cards, sep='\n ')
    print(f"Player's Hand is at {player.value}")
    
def player_bust(player,dealer,chips):
    print("Player busts!")
    chips.lose()

def player_win(player,dealer,chips):
    print("player wins!")
    chips.win()
    
def dealer_bust(player,dealer,chips):
    print("Dealer busts!")
    chips.win()
    
def dealer_win(player,dealer,chips):
    print("Dealer wins!")
    chips.lose()
    
def push(player,dealer):
    print("Dealer and Player tie! Its a push.")
    
   #initialize player chips
player_chips = Chips()  
    
while True:
    print('Welcome to BlackJack! Get as close to 21 as you can without going over!\n\
    Dealer hits until they reach 17. Aces count as 1 or 11.')
    
    #create deck and shuffle, deal two cards to each player
    deck = Deck()
    deck.shuffle()
    
    player_hand = Hand()
    player_hand.add_card(deck.deal())
    player_hand.add_card(deck.deal())
    
    dealer_hand = Hand()
    dealer_hand.add_card(deck.deal())
    dealer_hand.add_card(deck.deal())
    
    # take player bet
    take_bet(player_chips)
    
    #show the cards
    show_some(player_hand,dealer_hand)
    
    while playing:
        # prompt for player to hit or stand
        hit_or_stand(deck,player_hand)
        show_some(player_hand,dealer_hand)
        
        if player_hand.value >21:
            player_bust(player_hand,dealer_hand,player_chips)
            break
        
    if player_hand.value <= 21:
        while dealer_hand.value < 17:
            hit(deck,dealer_hand)
            
        show_all(player_hand,dealer_hand)
        
        if dealer_hand.value > 21:
            dealer_bust(player_hand,dealer_hand,player_chips)
        
        elif dealer_hand.value > player_hand.value:
            dealer_win(player_hand,dealer_hand,player_chips)
            
        elif dealer_hand.value < player_hand.value:
            player_win(player_hand,dealer_hand,player_chips)
            
        else:
            push(player_hand,dealer_hand)
            
    print(f"\nPlayer has {player_chips.total}\n")
    
    again = input("would you like to play another hand? enter 'y' or 'n' ")
    if again[0].lower() =='y':
        playing=True
        continue
    else:
        print("thanks for playing!")
        playing = False
        break
