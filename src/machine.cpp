/*

  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/

class implementation                                                        
*/

#include "machine.hpp"
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <iostream>
#include <rang.hpp>

using namespace rang;

namespace cncpp{

  Machine::Machine(const string &settings_file) : _settings_file(settings_file){ 

    load(settings_file);
    mosqpp::lib_init();
  }

  Machine::~Machine(){

    if(disconnect() != MOSQ_ERR_SUCCESS){
      cerr << fg::red << "Cannot disconnect from MQTT broker " << fg::reset << endl;
    }
    mosqpp::lib_cleanup();
  }

  void Machine::load(const string &s){
    _settings_file = s;
    
    auto data = YAML::LoadFile(s);      // data now is an object which has all the methosds and attributes that can be used in order to access to all yaml fields

    auto machine = data["machine"];
    _A         = machine["A"].as<data_t>();
    _tq        = machine["tq"].as<data_t>();
    _max_error = machine["max_error"].as<data_t>();
    _fmax      = machine["fmax"].as<data_t>();
    _zero      = Point(machine["zero"][0].as<data_t>(), machine["zero"][1].as<data_t>(), machine["zero"][2].as<data_t>());
    _offset    = Point(machine["offset"][0].as<data_t>(), machine["offset"][1].as<data_t>(), machine["offset"][2].as<data_t>());

    //MQTT parameters from yml file
    _mqtt_host = data["mqtt"]["host"].as<string>("localhost");    // inside () there is the default
    _mqtt_port = data["mqtt"]["port"].as<int>(1883);
    _mqtt_keepalive = data["mqtt"]["keepalive"].as<int>(60);
    _pub_topic = data["mqtt"]["topics"]["pub"].as<string>("cnc/setpoint");
    _sub_topic = data["mqtt"]["topics"]["sub"].as<string>("cnc/status/#");  // # is a wildcard

  }

  string Machine::desc(bool colored) const{

    stringstream ss;

    ss << "A = " << _A << ", ";
    ss << "tq = " << _tq << ", ";
    ss << "max_error = " << _max_error << ", ";
    ss << "fmax = " << _fmax << ", " << endl;

    ss << "zero = " << _zero.desc(colored) << endl;
    ss << "offset = " << _offset.desc(colored) << endl;
    ss << "MQTT host = " << mqtt_host() << endl;

    return ss.str();
  }


  data_t Machine::quantize(data_t t, data_t &dq) const{

      data_t q;
      q = static_cast<size_t>((t / _tq) + 1) * _tq;
      dq = q - t;

      return q;
  }

  int Machine::connect(){
    // call the mosquittopp method
    int rc = mosquittopp::connect(_mqtt_host.c_str(), _mqtt_port, _mqtt_keepalive);
    if(rc != MOSQ_ERR_SUCCESS){
      
      throw CNCError("Cannot connect to MQTT broker ", this);
    }

    return rc;
  }

  void Machine::listen_start(){

    if(subscribe(NULL, _sub_topic.c_str()) != MOSQ_ERR_SUCCESS){
      
      throw CNCError("Cannot subscribe to topic " + _sub_topic, this);
    }
  }

  void Machine::listen_stop(){

    if(unsubscribe(NULL, _sub_topic.c_str()) != MOSQ_ERR_SUCCESS){
      
      throw CNCError("Cannot unsubscribe to topic " + _sub_topic, this);
    }
  }

  void Machine::on_connect(int rc){

    if(_debug){
      cerr << fg::yellow << style::italic << "Connected to broker " << mqtt_host() << fg::reset << endl;
    }
  }

  void Machine::on_disconnect(int rc){

      if(_debug){
        cerr << fg::yellow << style::italic << "Disconnected to broker " << mqtt_host() << fg::reset << endl;
    }
  }

  void Machine::on_subscribe(int mid, int qos_count, const int *qos){

    if(_debug){
        cerr << fg::yellow << style::italic << "Subscribed to topic " << mqtt_host() << fg::reset << endl;
    }
  }

  void Machine::on_unsubscribe(int mid){

    if(_debug){
        cerr << fg::yellow << style::italic << "Unsubscribed from topic " << mqtt_host() << fg::reset << endl;
    }

  }

  void Machine::on_message(const struct mosquitto_message *message){

    string payload((char *)message -> payload, message -> payloadlen);
    json j;

    try{
      j = json::parse(payload);

    } catch(json::parse_error &e){
      cerr << fg::red << "Canont parse JSON payload: " << e.what() << endl;
      cerr << "Payload was: " << style::bold << payload << style::reset << fg::reset << endl;

      return;
    }

    _position = Point(j.value("x", 0) * 1000,
                      j.value("y", 0) * 1000,
                      j.value("z", 0) * 1000);
    _error = j.value("error", 0) * 1000;          // * 1000 because the cnc simulator works in meters

  }

  void Machine::sync(bool rapid){ // synchronize the machine with the current values
    Point pos = (_setpoint + _offset);
    json j;
    j["x"] = pos.x();
    j["y"] = pos.y();
    j["z"] = pos.z();
    j["rapid"] = rapid;         // flag in order to tell if the movement is rapid or not
    string payload = j.dump();  // dump method convert a json structure into a string
    publish(NULL, _pub_topic.c_str(), payload.length(), payload.c_str(), 0, false);

    loop();                     // every sync call the mqtt communication updates thanks to loop() function

  }



}




/*
  _____         _                     _       
 |_   _|__  ___| |_   _ __ ___   __ _(_)_ __  
   | |/ _ \/ __| __| | '_ ` _ \ / _` | | '_ \ 
   | |  __/\__ \ |_  | | | | | | (_| | | | | |
   |_|\___||___/\__| |_| |_| |_|\__,_|_|_| |_|
                                              
*/

#ifdef MACHINE_MAIN

#include <iostream>
using namespace cncpp;

int main(int argc, const char *argv[]){

  if(argc < 2){ //we need at least 2 arguments, so 

    // argv[0] is the name of the exucatble
    // argv[1] is the first argument of the executable

    cerr << "Usage: " << argv[0] << " <settings_file" << endl;
    return 1;

  }

  cncpp::Machine machine(argv[1]);
  cout << "Inizialized machine: " << endl << machine.desc() << endl;

  cncpp::Machine default_machine = Machine();
  cout << "Default machine: " << endl << default_machine.desc() << endl;

  default_machine.load(argv[1]);
  cout << "Default machine after loading: " << endl << default_machine.desc() << endl;

  return 0;


}


#endif