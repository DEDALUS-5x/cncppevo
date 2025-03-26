#include <string>
#include <iostream>

// REMINDER
// members and functions are private by default in classes
// for struct instaed members and functions are public by default

class Vehicle{
  
  private:
    std::string brand;
    std::string name;
    int wheels_number;
  
  public:

    Vehicle(){
      brand = "none";
      name = "none";
      wheels_number = 0;
    }

    Vehicle(std::string n, std::string b){
      this -> name = n;
      this -> brand = b;
      this -> wheels_number = 0;
    }

    Vehicle(std::string n, std::string b, int w){
      this -> name = n;
      this -> brand = b;
      this -> wheels_number = w;
    }

    void print(void){
      std::cout << "Car: " << name << " " << brand << std::endl;
    }
};

// inheritance, Car has all the properties of Vehicle
class Car : public Vehicle{
  public:
    Car(std::string n, std::string b) : Vehicle(b, n, 4) {}   // fast inizialization by using the Vehicle constructor -> in this way we call the father class constructor -> check the diamond problem
};

int main(){

  Vehicle car("bmw", "123");
  car.print();

  Car my_car("my", "panda");
  my_car.print();


  return 0;
}