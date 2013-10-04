//
//  PokerHand.h
//  PokerRanks
//
//  Created by Christopher Tarquini on 10/3/13.
//  Copyright (c) 2013 Christopher Tarquini. All rights reserved.
//

#ifndef __PokerRanks__PokerHand__
#define __PokerRanks__PokerHand__

#include <iostream>
#include <bitset>
#include <vector>

// temporary macros for generating types
#define K(t, s) t,
#define SK(t, s, c) t,
#define F(t, s) bool t : 1;
#define RED 0xFF
#define BLACK 0x00


#define SUITS(T) \
    T(Hearts, ♥, RED) \
    T(Diamonds, ♦, RED) \
    T(Spades, ♠, BLACK) \
    T(Clubs, ♣, BLACK)

#undef RED
#undef BLACK

#define RANKS(R) \
    R(AceLow, A) \
    R(Two, 2) \
    R(Three, 3) \
    R(Four, 4) \
    R(Five, 5) \
    R(Six, 6) \
    R(Seven, 7) \
    R(Eight, 8) \
    R(Nine, 9) \
    R(Ten, T) \
    R(Jack, J) \
    R(Queen, Q) \
    R(King, K) \
    R(Ace, A)

#define HANDS(T) \
    T(HighCard, High Card) \
    T(OnePair, Pair) \
    T(TwoPair, Two Pair) \
    T(ThreeOfAKind, Three of a Kind) \
    T(Straight, Straight) \
    T(Flush, Flush) \
    T(FullHouse, Full House) \
    T(FourOfAKind, Four of a Kind) \
    T(StraightFlush, Straight Flush) \
    T(RoyalFlush, Royal Flush)

namespace Poker {
    using std::string;
    
    enum Suit {
        SUITS(SK)
        SUIT_COUNT
    };
    
    enum Rank {
        RANKS(K)
        RANK_COUNT
    };

    enum HandType : unsigned char {
        HANDS(K)
        HAND_COUNT
    };

    struct Card {
        Rank rank;
        Suit suit;
    };
    
    union ResultCards {
        struct {
            RANKS(F)
        };
        unsigned short weight : 14;
    };
    
    #undef K
    #undef SK
    #undef F
    
    union Result {
        struct {
            ResultCards ranks;
            HandType type:4;
        };
        unsigned int score;
    };
    
    typedef std::bitset<RANK_COUNT> RankSet;
    typedef std::bitset<SUIT_COUNT> SuitSet;
    typedef std::vector<Card> Hand;
    
    Result Evaluate(Hand hand);
    Result Evaluate(string hand);
    Hand ParseHand(string str);
    string RankName(Rank rank);
    string SuitName(Suit suit);
    string HandName(HandType type);
    string StringifyHand(Hand hand);
}
#endif /* defined(__PokerRanks__PokerHand__) */
