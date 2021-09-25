//
// Created by christian on 8/24/21.
//

#include "problem.h"
#include <iostream>
#include <cstring>
#include "christian_utils.h"

using namespace std;

void Problem::pureRandomScramble(U8 * data)
{
    unsigned char * to_change = data + fast_rand()%(data_len);
    *to_change = (U8)fast_rand();
}

void Problem::textPrettyPrint(U8 * data)
{
    cout << data << endl;
}

void Problem::hexPrettyPrint(U8 * data) const
{
    print_data_as_hex(data, data_len);
}

void Problem::textDataInit(U8 * data) const
{
    memset(data, ' ', data_len);
}

void Problem::prettyPrintData(U8 *data) {
    hexPrettyPrint(data);
}

double Problem::scalarTrial(U8 *data) {
    return 0;
}

void Problem::scrambler(U8 *data) {
    pureRandomScramble(data);
}

void Problem::dataInit(U8 *data) {
    memset(data, 0, data_len);
}
