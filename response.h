#ifndef _SERVER_RESPONSE_
#define _SERVER_RESPONSE_

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>

#include "responseTypes.h"

constexpr static int maxServerResponseSize = 1500;

class response {
public:
    char* getAcess() const {
        return (char*)state;
    }

    responseType checkResponseType() const {
        if(state[0] == 'P')
            return responseType::Positive;
        return responseType::Negative;
    }

    void setResponseType(responseType rt) {
        if(rt == responseType::Positive)
            state[0] = 'P';
        else
         state[0] = 'N';
    }

    void clearState(){
        memset(state, 0, maxServerResponseSize);
    }
    
private:
    char state[maxServerResponseSize];
};

#endif

