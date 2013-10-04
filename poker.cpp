//
//  PokerHand.cpp
//  PokerRanks
//
//  Created by Christopher Tarquini on 10/3/13.
//  Copyright (c) 2013 Christopher Tarquini. All rights reserved.
//

#include "poker.h"
#define lookup(c) table[toupper(c) - 'A']
#define parse_rank(c) static_cast<Rank>(isdigit(c) ? atoi(&c) - 1 : lookup(c))
#define parse_suit(c) static_cast<Suit>(table[toupper(c) - 'A'])
#define K(t, s) #s,
#define SK(t, s, c) #t,
#define KV(t,v) K(t)
#define NAME_TABLE(T, K) T##_NAMES[] { T##S(K) };

namespace Poker {
    
    // Used to convert letters from a string to a card rank
    const int table[] = {
        Ace,
        -1,Clubs,
        Diamonds,
        -1,-1,-1,Hearts,
        -1,Jack,
        King,
        -1,-1,-1,-1,-1,Queen,
        -1,Spades, 
        Ten,-1,-1,-1,-1,-1,-1
    };
    
    // Table mapping the index from our magical bit flipping routine to a card rank
    static const HandType types[] {
        FourOfAKind,StraightFlush,Straight,Flush,HighCard,OnePair,TwoPair,RoyalFlush,ThreeOfAKind,FullHouse
        
    };
    
    static const string NAME_TABLE(HAND, K)
    static const string NAME_TABLE(SUIT, SK)
    static const string NAME_TABLE(RANK, K)
    
    Result Evaluate(Hand hand) {
        RankSet ranks;
        SuitSet suits;
        Result result;
        result.score = 0;
        result.ranks.weight = 0;
        unsigned long s;
        unsigned long index = 0x0;
        
        for(auto i = hand.begin(); i != hand.end(); i++) {
            Card card = *i;
            Rank rank = card.rank;
            
            // keep track of ranks and suits for detecting straights/flushes and determining a score weight
            ranks.set(rank, true);
            suits.set(card.suit, true);
            
            // grab a pointer to the number of cards with this rank from the index
            // Rank indices are offset by one
            char* byte = (char *)(&index) + (rank-1)/2;
            // use lower nibbles with lower ranks
            char rem = (rank-1) % 2;
            char nibble = *byte & (0xF0 >> (rem * 4));
            
            // adjusts the flags from 0001 (one card) -> 0011 (pair) --> 0111 (two pair) -> 1111 (four of a kind)
            nibble = (nibble << 1 | nibble);
            
            // increment the nibble if it hasn't been set yet
            if (nibble == 0) {
                nibble = 0x10 >>  rem * 4;
            }
            
            // write the updated count back to the index
            *byte |= (int)nibble;
        }
        
        s = ranks.to_ulong();
        
        // I'm still not 100% sure why this works
        index =  index % 15 - 1;

        // Normalize LSB using two's complement and test for straights
        // 0x31 is the binary representation of 5 sequential ranks being set
        // 0x201e is the binary representation of a low-ace straight (1 in MSB, 4 in LSB)
        if ((s / (s & -s)) == 31 || s == 0x201e) {
            index -= 2;
            if (ranks.test(Ace) && !ranks.test(King)) {
                ranks.set(Ace, false);
                ranks.set(AceLow, true);
            }
        }
        
        // Check for flushes
        unsigned long flush = suits.to_ulong();
        // Same magic as before, normalizes flush bitset so set bits shift to LSB
        flush = flush / (flush & -flush);
        // flushes will only have one suit, so one bit will be set
        if (flush == 0x1) {
            // if the MSB's are set it's a royal flush
            if(s == 0x3e00)
                index+=5;
            else
                index -=1;
        }
        

        
        // Because result.type becomes the MSBs of the score
        // higher ranking hands will always be higher than lower ranking hands, regardless of the weight value
        // the weight is determined what rank cards are in the hand (higher ranks = MSB, lower ranks = LSB)
        // meaning that in the event of a tie, the hand with a higher ranking card will have a higher score
        result.type = types[index];
        result.ranks.weight = s;
        
        return result;
    };
    
    Hand ParseHand(string str) {
        if (str.length() == 0) return {};
        Hand hand;
        for (auto i = str.begin(), end = str.end(); i != end && i + 1 != end; ++i) {
            if (*i == ' ' || *i == ',') continue;
            Card card;
            card.rank = parse_rank(*i);
            card.suit = parse_suit(*(++i));
            hand.push_back(card);
        }
        return hand;
    };
    
    Result Evaluate(string hand) {
        return Evaluate(ParseHand(hand));
    };
    
    string HandName(HandType type) {
        return HAND_NAMES[type];
    }
    
    string RankName(Rank rank) {
        return RANK_NAMES[rank];
    };
    
    string SuitName(Suit suit) {
        return SUIT_NAMES[suit];
    };
    
}