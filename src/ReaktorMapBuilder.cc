#include "ReaktorMapBuilder.h"

using namespace std;

int ReaktorMapBuilder::parsePrefix(const char* s) const
{
  string str(s);
  string pfx;
  pfx.reserve(str.size());

  for (auto c : str) {
    if (c == '_') break;
    if (!isdigit(c)) return -1;
    pfx += c;
  }

  return stoi(pfx, nullptr, 10);
}

bool ReaktorMapBuilder::checkIfAudioFile(const char* s) const
{
  string str(s);
  string fmt;
  fmt.reserve(5);
  bool has_dot = false;
  
  int i;
  for (i = str.size() - 1; i >= 0; --i) {
    if (str[i] == '.') {
      ++i;
      has_dot = true;
      break;
    }   
  }

  if (!has_dot) return -1; 

  for (; i < str.size(); ++i) {
    fmt += str[i];
  }
  
  for (auto& f : _formats) {
    if (fmt == f) return true;
  }

  return false; 
}

int ReaktorMapBuilder::loadReaktorMapData()
{
    DIR* d1;
    DIR* d2;
    struct dirent* entry1;
    struct dirent* entry2;
    int prefix;
    char path_t[PATH_MAX + 1] = "";
    int vel = 1;

    _number_of_refs = 0;
    _path_length_sum = 0;

    for (int i = 0; i < 128; i++)
      _key_lengths[i] = 0;


    if ((d1 = opendir(".")) == NULL) {
      cerr << "Unable to read current directory" << endl;
      return -1;
    }

    
    while ((entry1 = readdir(d1)) != NULL) {
      if (IsDirectory(entry1->d_name) &&
          (prefix = parsePrefix(entry1->d_name)) != -1) {
        if ((d2 = opendir(entry1->d_name)) == NULL) {
          cerr << "Unable to open directory" << endl;
          return -1;
        }
        while ((entry2 = readdir(d2)) != NULL) {
          if (vel > 127) {
            cerr << "NOTE: more than 127 files in " << entry1->d_name << endl;
            break;
          }
          if ((strcmp(entry2->d_name, ".") != 0) &&
              (strcmp(entry2->d_name, "..") != 0) &&
              checkIfAudioFile(entry2->d_name) &&
              (entry2->d_name[0] != '.')) {
            
            _key_lengths[prefix]++;
            _number_of_refs++;
            realpath(entry1->d_name, path_t);
            string path(path_t);
            path += "/";
            path += entry2->d_name;
            int path_size = path.size();
            _path_length_sum += path_size;
            _refs.push_back(Ref { path_size,
                                 path,
                                 prefix,
                                 prefix,
                                 vel,
                                 vel,
                                 prefix });
            ++vel;
          }
        }
      }
      vel = 1;
    }

    closedir(d1);
    closedir(d2);

    return 0;
}

void ReaktorMapBuilder::createSampleMapFile()
{
  ofstream f;

  f.open("map.map", ios::binary);
  f.write(_buffer, _buf_size);

  f.close();
}

void ReaktorMapBuilder::createTextFile()
{
  ofstream f;

  f.open("table.txt");
  for (int i = 0; i < 127; ++i)
    f << _key_lengths[i] << ",";
  f << _key_lengths[127];

  f.close();
}

void ReaktorMapBuilder::buildReaktorMap()
{
  loadReaktorMapData();
  createBuffer();
  createSampleMapFile();
  createTextFile();
}
