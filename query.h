#ifndef _CLIENT_QUERY_
#define _CLIENT_QUERY_

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>

#include "queryTypes.h"

constexpr int maxClientMessageSize = 10000;

class query {
public:

    query(){
        text.resize(maxClientMessageSize);
    } 

    query(std::string query_text) {
        clearText();
        text = query_text;
    }

    std::vector<std::string> getTokens() const {
        std::vector<std::string> sets;
        return  boost::split(sets, text, boost::is_any_of(" "));
    }

    char* getAcess() const{
        return (char*)text.c_str();
    }

    int getSize() const {
        return text.size();
    }

    std::string& getText(){
        return text;
    }

    void clearText(){
        text.clear();
    }

    queryType getQueryType() const {
        if(text[0] == 'R')
            return queryType::Registration;
        if(text[0] == 'L')
            return queryType::Login;
        if(text[0] == 'S')
            return queryType::SendMessage;
        if(text[0] == 'E')
            return queryType::Exit;

        return queryType::NotQuery;
    }

    operator std::string&(){
        return text;
    }

private:
    std::string text;

private:
};

#endif

