#ifndef _DB_
#define _DB_

#include <string>
#include <fstream>
#include <iostream>

class DB{
public:
    DB(const char* name_in) : name(name_in), file(name_in) { }

    void write(std::string&& entry){
        try{
            file.open(name, std::ios_base::app);
            if(file.is_open())
                file.write(entry.c_str(), entry.size()); 
            std::cout << "writed " << entry << " into " << name << "\n";
        }catch(...){
            std::cout << "couldnt write into file\n" << name << "\n";
        }

    }

    ~DB(){
        file.close();
    }

private:
    std::fstream file;
    std::string name;
};

#endif

