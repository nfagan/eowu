#include "play-serialize.hpp"
#include "play-store.hpp"

int main(int argc, char** argv) {
  
  eowu::serialize::play_serialize();
  eowu::data::play_store();
  
  return 0;
}
