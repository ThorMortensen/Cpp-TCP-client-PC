/* 
 * File:   ProgArg_s.h
 * Author: Thor
 *
 * Created on 26. marts 2015, 21:51
 */

#ifndef PROGARG_S_H
#define	PROGARG_S_H

/**
 * Class to hold argument to this program.
 * Holds the argument option an parameter. 
 * @param uint8_t number of this arg in arg list 
 * @param string literal of this arg (what user should type)
 * @param uint8_t parameter type can be NOTHING, STRING, NUMBER 
 * @param uint8_t length of parameter value string (default 0)
 */
class ProgArg_s {
public:

#define NOTHING 0
#define STRING  1
#define NUMBER  2

    ProgArg_s(); //Default constructor for arrays
    ProgArg_s(uint8_t number, const std::string literal, uint8_t parameter, uint8_t length = 0)  : //ASK FOR POINTER INSTEAD OF COPY STRING????
    number(number), literal(literal), expectedParam(parameter){
        hasValue = false;
        paramValNo = 0;
        this->paramLength = length;};
    
    ProgArg_s(const ProgArg_s& orig);
    
    bool hasValue;
    
    //Posible parameter string values for this argument
    std::vector<std::string> posibleParamValus;

    void setArgumet(uint8_t number, const std::string literal, uint8_t parameter, uint8_t length = 0);
    bool isValid(std::string parameterValue);
    bool equals(std::string compStr);
    void printError();
    void setParamVal(std::string paramVal);
    std::string getParamVal();
    void setParamValNo(uint32_t paramValNo);
    uint32_t getParamValNo();
    virtual ~ProgArg_s();

private:

    uint8_t paramLength;
    uint8_t expectedParam;
    uint8_t number;
    std::string literal;
    std::string paramVal;
    
    uint32_t paramValNo;


};

#endif	/* PROGARG_S_H */

