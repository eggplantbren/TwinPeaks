#include "Database.h"

namespace TwinPeaks
{

Database::Database()
:db("output/twinpeaks.db")
{
    db << "PRAGMA JOURNAL_MODE = WAL;";
    db << "PRAGMA SYNCHRONOUS = 0;";

    reset_tables();
}


void Database::reset_tables()
{
    db << "DROP TABLE IF EXISTS particles;";

    db << "CREATE TABLE particles\
(id     INTEGER NOT NULL PRIMARY KEY,\
 params TEXT NOT NULL);";

}



} // namespace
