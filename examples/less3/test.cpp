#include <iostream> 
#include <vector>

class Animal{

  //default private
  std::string _sound{"Undefined"};

  public:
    ~Animal(){
      std::cout << "oh no" << std::endl;
    }

    Animal(){}

    Animal(std::string s) : _sound(s){}

    const std::string &sound() const{
      return this -> _sound;
    }

    void make_sound(){
      std::cout << "Animal says: " << this -> _sound << std::endl:
    }

    static void make_sound(std::string s){
      std::cout << "Animal also says: " << s << std::endl;
    }

};

