#include "src.hpp"
#include <iostream>
using namespace final;
int main(){
    shell sh;
    sh.run(1, "ls -al /usr/bin", [](int argc, char** argv){
        std::cout << argc << "\n";
        for(int i=0;i<argc;++i){
            std::cout << '[' << argv[i] << "]\n";
        }
    });
    sh.run(2, "single", [](int argc, char** argv){
        std::cout << argc << "\n";
        for(int i=0;i<argc;++i){
            std::cout << '[' << argv[i] << "]\n";
        }
    });
    return 0;
}
