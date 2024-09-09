/********************************************************************
CSCI 480 - Assignment 2 - Semester (Spring) 2022

Progammer: Samuel Haffner
Section:   001
TA:        Jake Rogers
Date Due:  02/19/2022

Purpose:   Just a helper class configured for dealing 
*                 with conversions of strings
*********************************************************************/
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>


class strHelper{
public:
    void toLower(std::string &v){
        std::for_each(v.begin(), v.end(), [](char & c) {c = ::tolower(c); });
    } 
    // Simple helper to turn 
    char *convert(const std::string & s)
    {
       char *pc = new char[s.size()+1];
       std::strcpy(pc, s.c_str());
       return pc; 
    }

    std::vector<std::string> split(std::string input){
        std::vector<std::string> output;
        
        size_t pos = 0;
        std::string token = "";
        // while there is a space in the string, we loop through getting each section of the string
        while ((pos = input.find(" ")) != std::string::npos) {
            token = input.substr(0, pos);
            
            bool hasLetter = false;
            for (char c:token){
                if(c != ' ') {
                    hasLetter = true;
                    break;
                }
            }
            
            if(hasLetter){
                //then we save to our output vector
                output.push_back(token);
            }
            //remove that token from the users input
            input.erase(0, pos + 1);
        }
        if(input != " ")
            output.push_back(input);
        
        return output;
    }
};