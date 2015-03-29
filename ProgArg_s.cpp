/* 
 * File:   ProgArg_s.cpp
 * Author: Thor
 * 
 * Created on 26. marts 2015, 21:51
 */

#include <stdint.h>
#include <sstream>
#include <iostream>
#include <vector>
#include "ProgArg_s.h"

using namespace std;

ProgArg_s::ProgArg_s() {
}

/**
 * Copy Constructor (auto generated) 
 * @param orig
 */
ProgArg_s::ProgArg_s(const ProgArg_s& orig) {
}

/**
 * Class to hold argument to this program.
 * Holds the argument option an parameter. 
 * @param uint8_t no
 * @param const string literal
 */
//ProgArg_s::ProgArg_s(uint8_t number, const string literal, uint8_t parameter, uint8_t length/* = 0*/)){
//    
//}

/**
 * Setup the argument. Should be called for this object to make sense
 * 
 * @param uint8_t number of this arg in arg list 
 * @param string literal of this arg (what user should type)
 * @param uint8_t parameter type can be NOTHING, STRING, NUMBER 
 * @param uint8_t length of parameter value string (default 0)
 */
void ProgArg_s::setArgumet(uint8_t number, const string literal, uint8_t parameter, uint8_t length/* = 0*/) {
    this->expectedParam = parameter;
    this->paramLength = length;
    this->number = number;
    this->literal = literal;
}

/**
 * Check if the parameter is valid based on the setArgumet 
 * called when this arg was created
 * @param string parameterValue
 * @return true if value passed all test
 */
bool ProgArg_s::isValid(string parameterValue) {
    if ((expectedParam == STRING)) {
        if (posibleParamValus.empty()) {
            if ((paramLength > 0)&&(parameterValue.length() == paramLength)) {
                hasValue = true;
                this->paramVal = parameterValue;
                return true;
            }
        } else {
            for (uint8_t i = 0; i < posibleParamValus.size(); i++) {
                if (posibleParamValus.at(i) == parameterValue) {
                    this->paramVal = parameterValue;
                    hasValue = true;
                    return true;
                }
            }
            hasValue = false;
            return false;
        }
    } else if (expectedParam == NUMBER) {
        std::istringstream convert(parameterValue);
        if (!(convert >> paramValNo)) {
            hasValue = false;
            return false;
        } else {
            this->paramVal = parameterValue;
            hasValue = true;
            return true;
        }
    }
    hasValue = false;
    return false;
}

/**
 * Is this arg literal equal to the compStr
 * @param string compStr
 * @return true if so 
 */
bool ProgArg_s::equals(string compStr) {
    return (literal == compStr);
}

/**
 * Print an error telling what was expected for this arg to be satisfied
 */
void ProgArg_s::printError() {
    cout << "ERROR: Expected '" << literal << "'";
    if (expectedParam == STRING)
        cout << " followed by a string";
    if (paramLength > 0)
        cout << " of length " << paramLength+0 ; //WHAT !!!? will not print without a +0???
    if (!posibleParamValus.empty()) {
        cout << " with content: ";
        for (uint8_t i = 0; i < posibleParamValus.size(); i++) {
            cout << "\""<<posibleParamValus.at(i) << "\" ";
        }
    }
    if (expectedParam == NUMBER)
        cout << " followed by a number.";
    cout << "\n\r";
}

void ProgArg_s::setParamVal(string paramVal) {
    this->paramVal = paramVal;
}

string ProgArg_s::getParamVal() {
    return paramVal;
}

void ProgArg_s::setParamValNo(uint32_t paramValNo) {
    this->paramValNo = paramValNo;
}

uint32_t ProgArg_s::getParamValNo() {
    return paramValNo;
}

ProgArg_s::~ProgArg_s() {
}

