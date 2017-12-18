#include "ReaktorMapBuilder_6_0_1_R1526.h"

void ReaktorMapBuilder_6_0_1_R1526::createBuffer()
{
  _buf_size = 50 + _number_of_refs * 108 + _path_length_sum;

  int buf_idx;
  _buffer = new char[_buf_size];

  _buffer[0] = 0x00;
  _buffer[1] = 0x00;
  _buffer[2] = 0x00;
  _buffer[3] = 0x00;
  _buffer[4] = 0xD8;
  _buffer[5] = 0x02;
  _buffer[6] = 0x00;
  _buffer[7] = 0x00;
  _buffer[8] = 0x4E;
  _buffer[9] = 0x49;
  _buffer[10] = 0x4D;
  _buffer[11] = 0x61;
  _buffer[12] = 0x70;
  _buffer[13] = 0x46;
  _buffer[14] = 0x69;
  _buffer[15] = 0x6C;
  _buffer[16] = 0x65;
  _buffer[17] = 0x02;
  _buffer[18] = 0x00;
  _buffer[19] = 0x00;
  _buffer[20] = 0x00;
  _buffer[21] = 0x01;
  _buffer[22] = 0x6D;
  _buffer[23] = 0x61;
  _buffer[24] = 0x70;
  _buffer[25] = 0x70;
  _buffer[26] = 0x0C;
  _buffer[27] = 0x00;
  _buffer[28] = 0x00;
  _buffer[29] = 0x00;
  _buffer[30] = 0x01;
  _buffer[31] = 0x00;
  _buffer[32] = 0x00;
  _buffer[33] = 0x00;
  _buffer[34] = 0x01;
  _buffer[35] = 0x00;
  _buffer[36] = 0x00;
  _buffer[37] = 0x00;
  _buffer[38] = 0x00;
  _buffer[39] = 0x00;
  _buffer[40] = 0x00;
  _buffer[41] = 0x00;
  _buffer[42] = 0x00;
  _buffer[43] = 0x00;
  _buffer[44] = 0x00;
  _buffer[45] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
  _buffer[46] = (char)_number_of_refs;
  _buffer[47] = (char)(_number_of_refs >> 8);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
  _buffer[48] = 0x00;
  _buffer[49] = 0x00;

  buf_idx = 50;

  for (auto ref = _refs.begin(); ref != _refs.end(); ++ref) {
    _buffer[buf_idx++] = 0x01;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = (char)ref->path_size;
    _buffer[buf_idx++] = (char)(ref->path_size >> 8);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    for (int i = 0; i < ref->path.size(); ++i)
      _buffer[buf_idx++] = ref->path[i];
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x65;
    _buffer[buf_idx++] = 0x6E;
    _buffer[buf_idx++] = 0x74;
    _buffer[buf_idx++] = 0x72;
    _buffer[buf_idx++] = 0x54;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x02;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = (char)ref->low;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = (char)ref->high;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = (char)ref->velocity_low;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = (char)ref->velocity_high;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    _buffer[buf_idx++] = (char)ref->root;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    for (int i = 0; i < 23; ++i)
      _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x04;
    for (int i = 0; i < 31; ++i)
      _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x55;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x01;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
    _buffer[buf_idx++] = 0x00;
  }
}
