//
// Created by k3l on 06.11.2019.
//

#include "ConfigReader.h"

namespace  ConfigReader {

    template<> int TypeParser<int>::operator()( const std::string &s){
        std::regex r("\\s*-?[0-9]+\\s*");
        if(std::regex_match(s,r)){
            return atoi(s.c_str());
        }
        throw std::invalid_argument("invalid argument " + s);
    }

    template <> float TypeParser<float>::operator()( const std::string &s){
        std::regex r("\\s*[-]?[0-9]+\\.[0-9]*\\s*");
        if(std::regex_match(s,r)){
            return atof(s.c_str());
        }
        throw std::invalid_argument("invalid argument " + s);
    }

    template <> double TypeParser<double>::operator()(const std::string &s){
        std::regex r("\\s*-?[0-9]+\\.[0-9]*\\s*");
        if(std::regex_match(s,r)){
            return atof(s.c_str());
        }
        throw std::invalid_argument("invalid argument " + s);
    }

    template <> bool TypeParser<bool>::operator()(const std::string &s){
        std::regex r_true("\\s*true\\s*");
        std::regex r_false("\\s*false\\s*");
        if(std::regex_match(s,r_true)){
            return true;
        } else if(std::regex_match(s,r_false)){
            return false;
        }
        throw std::invalid_argument("invalid argument " + s);
    }

    template <> std::string TypeParser<std::string>::operator()(const std::string &s){
        std::regex r1("\\s*\\w+\\s*");
        std::regex r2("\\s*\"[^\"]*\"\\s*");
        if(std::regex_match(s,r1)){
            std::regex r("\\s*");
            return std::regex_replace(s,r,"");
        } else if(std::regex_match(s,r2)){
            return s.substr(s.find("\"") + 1,s.rfind("\"") - s.find("\"") - 1);
        }
        throw std::invalid_argument("invalid argument " + s);
    }

    ConfigReader reader("../config.txt");
}



