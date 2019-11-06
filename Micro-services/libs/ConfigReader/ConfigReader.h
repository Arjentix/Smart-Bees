//
// Created by k3l on 01.11.2019.
//

/*
 *  Usage:
 *  try{
 *
 *      ConfigReader::ConfigReader reader("config_file.txt")
 *      reader.read_config() // throws std::ios_base::failure
 *
 *      int variable_name1 = reader.ReadValueByKey<int>("variable_name1")
 *      float variable_name2 = reader.ReadValueByKey<float>("variable_name2")
 *      double variable_name3 = reader.ReadValueByKey<double>("variable_name3")
 *      bool variable_name4 = reader.ReadValueByKey<bool>("variable_name4")
 *      std::string variable_name5 = reader.ReadValueByKey<std::string>("variable_name5")
 *
 *  } catch (std::invalid_argument e) {
 *      std::cout << e.what()
 *      exit()
 *  } catch (std::ios_base::failure e) {
 *      std::cout << e.what()
 *      exit()
 *  }
 *
 *  Usage for user types
 *
 *
 *  template<>
 *  MyType TypeParser<MyType>::operator()( const std::string &s) {
 *      // your
 *  }
 *
 *  MyType my_type_var_name = reader.ReadValueByKey<MyType>("my_type_var_name")
 *
 */

#include <stdexcept>


#pragma once

#include <string>
#include <map>
#include <fstream>
#include <regex>

namespace ConfigReader{

    template <typename T>
    class TypeParser{
    public:
        T operator()(const std::string &s){
            return T(s);
        };

    };

    template<> int TypeParser<int>::operator()( const std::string &s){
        std::regex r("\\s*-?[0-9]+\\s*");
        if(std::regex_match(s,r)){
            return atoi(s.c_str());
        }
        throw std::invalid_argument(s);
    }

    template <> float TypeParser<float>::operator()( const std::string &s){
        std::regex r("\\s*[-]?[0-9]+\\.[0-9]*\\s*");
        if(std::regex_match(s,r)){
            return atof(s.c_str());
        }
        throw std::invalid_argument(s);
    }

    template <> double TypeParser<double>::operator()(const std::string &s){
        std::regex r("\\s*-?[0-9]+\\.[0-9]*\\s*");
        if(std::regex_match(s,r)){
            return atof(s.c_str());
        }
        throw std::invalid_argument(s);
    }

    template <> bool TypeParser<bool>::operator()(const std::string &s){
        std::regex r_true("\\s*true\\s*");
        std::regex r_false("\\s*false\\s*");
        if(std::regex_match(s,r_true)){
            return true;
        } else if(std::regex_match(s,r_false)){
            return false;
        }
        throw std::invalid_argument(s);
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
        throw std::invalid_argument(s);
    }

    class ConfigReader{
    private:
        std::string _file_name;
        std::map<std::string,std::string> variables;
    public:

        ConfigReader(std::string file_name){
            this->_file_name = file_name;
        }

        void read_config(){
            std::ifstream input(this->_file_name);
            std::string line;
            if(input.is_open()){
                while (getline(input,line)){
                    if(line.find("#") != std::string::npos){
                        line.erase(line.find("#"));
                    }
                    if(line.find("=") != std::string::npos){
                        std::string s1 = line.substr(0,line.find("="));
                        std::regex r("\\s*");
                        s1 = std::regex_replace(s1,r,"");
                        std::string s2 = line.substr(line.find("=") + 1,line.length());
                        variables[s1] = s2;
                    }

                }
            } input.exceptions(std::ifstream::failbit);
        }

        template < typename T>
        T ReadValueByKey(std::string key){
            std::map<std::string,std::string>::iterator it = this->variables.find(key);
            if(it != variables.end()){
                TypeParser<T> typeParser;
                return typeParser((*it).second);
            } else {
                throw std::invalid_argument(key);
            }

        }
    };
}

