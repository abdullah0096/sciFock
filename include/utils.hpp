#ifndef __UTILS_CPP__
#define __UTILS_CPP__

#include <openvdb/openvdb.h>
// #include <string>

class utils
{
public:
    openvdb::DoubleGrid::Ptr readMatrixMarketFileWithCutoff(const std::string &, double);
};

#endif