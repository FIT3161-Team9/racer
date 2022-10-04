#pragma once
#include <string>

struct Vehicles
  {
    float speed;
    float acc;
    float fuel;
    std::string name;
  
  };

namespace vehicles{
   
    inline Vehicles car1() {return Vehicles{.speed = 300.f, .acc = 600.f, .fuel = 200.f, .name = "car 1"};};
    inline Vehicles car2() {return Vehicles{.speed = 500.f, .acc = 400.f, .fuel = 300.f, .name = "car 2"};};
    inline Vehicles car3() {return Vehicles{.speed = 600.f, .acc = 500.f, .fuel = 500.f, .name = "car 3"};};

};
