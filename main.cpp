#include "data_store/config.hpp"
#include "data_store/data_store.hpp"
#include "game_engine.hpp"

using std::ios;

int main() {
  GameEngine game;
  data_store::DataStore ds;
  if (ds.open("release/game_data_en.dat", ios::in | ios::binary, data_store::Smaster)) {
    cout << "Loading..." << std::endl;
    ds.deserialize(data_store::Smaster, game);
  }
  else {
    cout << "Data file not found, run the 'process' executable.\n";
    exit(0);
  }
    cout <<"Starting game..." <<std::endl;
  game.init();

/*
  data_store::DataStore ds;
  ds.open(DS_DATA_DIRECTORY "test.dat", ios::in | ios::binary, data_store::Smaster);
  ds.open(DS_DATA_DIRECTORY "save.dat", ios::in | ios::binary, data_store::Ssave);
  vector<Animal> animals;
  ds.deserialize(data_store::Smaster, animals);

  cout << "Animals after deserialization: \n";
  for (uint i = 0; i < animals.size(); i++) {
    cout << "\"" << animals[i].name << "\"\n";
    cout << "\"" << animals[i].weight << "\"\n";
    cout << "\"" << animals[i].max_age << "\"\n";
    cout << "\"" << animals[i].prey << "\"\n";
  }
//  cout << "plat prop" << animals[2].properties[0] <<"\n";
  data_store::DataStore ds2;
  ds2.open(DS_DATA_DIRECTORY "save.dat", ios::out | ios::binary, data_store::Ssave);
  animals[0].max_age++;
  ds2.serialize(data_store::Smaster, animals);
  cout << "Finished saving.\n";
*/
}
