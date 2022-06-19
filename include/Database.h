#ifndef TwinPeaks_Database_h
#define TwinPeaks_Database_h

#include "sqlite_modern_cpp/hdr/sqlite_modern_cpp.h"
#include <string>

namespace TwinPeaks
{

class Database
{
    private:
        sqlite::database db;

        void reset_tables();

    public:
        Database();

        void save_particle(const std::string& s,
                           const std::vector<double>& scalars);


};




} // namespace


#endif
