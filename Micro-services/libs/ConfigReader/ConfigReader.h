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
 *      int variable_name1 = reader.read_value_by_key<int>("variable_name1")
 *      float variable_name2 = reader.read_value_by_key<float>("variable_name2")
 *      double variable_name3 = reader.read_value_by_key<double>("variable_name3")
 *      bool variable_name4 = reader.read_value_by_key<bool>("variable_name4")
 *      std::string variable_name5 = reader.read_value_by_key<std::string>("variable_name5")
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

#pragma once

#include <stdexcept>
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

    template <> int TypeParser<int>::operator()(const std::string &s);

    template <>
    float TypeParser<float >::operator()(const std::string &s);

    template <>
    bool TypeParser<bool >::operator()(const std::string &s);

    template <> std::string TypeParser<std::string>::operator()(const std::string &s);

    class ConfigReader{
    private:
        std::string _file_name;
        std::map<std::string,std::string> variables;
    public:

        ConfigReader(std::string file_name){
            this->_file_name = file_name;
        }

        void set_file_name(std::string file_name){
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
            }
            else{
                input.exceptions(std::ifstream::failbit);
            }
        }

        template < typename T>
        T read_value_by_key(std::string key){
            std::map<std::string,std::string>::iterator it = this->variables.find(key);
            if(it != variables.end()){
                TypeParser<T> typeParser;
                return typeParser((*it).second);
            } else {
                throw std::invalid_argument("argument not found: " + key);
            }

        }
    };

    extern ConfigReader reader;
}


