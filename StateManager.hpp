#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

//
// State Manager 
// Manages the state of all game objects loaded through runtime load enviroment
// Can either write to file (for easy debugging) or save them in hashmap in memory for faster loading
//

class StateManager{
private:
    bool writeToFile;
    std::map<std::string, std::string> map;
public:

    //
    // Print function to debug state of the map
    //
    void printMap(){
        std::map<std::string, std::string>::iterator it;
        for ( it = map.begin(); it != map.end(); it++ )
        {
            std::cout << it->first << "->" << it->second << std::endl;
        }
        std::cout << std::endl;
    }

    //
    // Map iterator
    //
    std::string find(std::string className){
        std::string ret = "\0";
        std::map<std::string, std::string>::iterator i = map.find(className);
        if(i != map.end()){
            ret = i->second;
        }
        return ret;
    }

    //
    // Adding value to map 
    //
    void add(std::string first,std::string second){
        this->map[first] = second;
    }
    
    //
    // Based on flags, store the relevant string
    // Game Objects are resposible for handling class serialization to string objects
    //
    void save(std::string classname,std::string ss){
        if(writeToFile){
            std::ofstream file;
            file.open (classname, std::ofstream::out | std::ofstream::trunc);
            file << ss;
            file.close();
        }
        else{
            add(classname,ss);
        }
    }


    //
    // Based on flags, loads the relevant string
    // Game Objects are resposible for handling class serialization to string objects
    //
    std::string load(std::string classname){
        std::stringstream ss;
        if(writeToFile){
            std::ifstream file;
            file.open (typeid(this).name());
            ss << file.rdbuf();
            file.close();
        }
        else{
            ss << find(classname);
        }
        return ss.str();
    }

   StateManager(bool writeToFile = false){
       this->writeToFile = writeToFile;
   }
};

#endif