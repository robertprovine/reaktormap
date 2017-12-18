#ifndef REAKTOR_MAP_BUILDER__H
#define REAKTOR_MAP_BUILDER__H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <array>

#include "posix.h"

class ReaktorMapBuilder {
 public:
  void buildReaktorMap();
 protected:
  struct Ref {
    int path_size;
    std::string path; 
    int low;
    int high;
    int velocity_low;
    int velocity_high;
    int root;
  };

  std::list<Ref> _refs;
  int _path_length_sum;
  int _number_of_refs;
  char* _buffer;
  int _buf_size;
  std::array<int, 128> _key_lengths;
 private:
  int parsePrefix(const char* s) const;
  bool checkIfAudioFile(const char* s) const;
  int loadReaktorMapData();
  void createSampleMapFile();
  void createTextFile();
  virtual void createBuffer() = 0;

  const std::vector<std::string> _formats {
    "wav", "aif", "aiff", "aifc", "mp3", "wma", "ogg", "au", "ea"
  };
};

#endif
