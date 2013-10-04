//
//  main.cpp
//  PokerRanks
//
//  Created by Christopher Tarquini on 10/2/13.
//  Copyright (c) 2013 Christopher Tarquini. All rights reserved.
//

#include <iostream>
#include <bitset>
#include "poker.h"

using std::string;
using std::bitset;

#define console std::cout

int main(int argc, const char * argv[])
{
    string str("3s 4s 5s 6s 7s");
    string str2("Ts Js Qs Ks As");
    
    Poker::Result result = Poker::Evaluate(str);
    Poker::Result result2 = Poker::Evaluate(str2);
    
    console << "Hand #1 ( " << Poker::HandName(result.type) << "): " << result.score << "\n";
    console << "Hand #2 ( "<<  Poker::HandName(result2.type) << "): " << result2.score << "\n";
    console << "Winner: Hand #" << (result.score > result2.score ? "1": "2") << "\n";
}

