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
    db << "BEGIN;";

    db << "DROP TABLE IF EXISTS particles;";
    db << "DROP TABLE IF EXISTS scalars;";

    db << "CREATE TABLE particles\
(id     INTEGER NOT NULL PRIMARY KEY,\
 logp   REAL,\
 params TEXT NOT NULL);";

    db << "CREATE TABLE scalars\
(particle_id INTEGER NOT NULL,\
 scalar      INTEGER NOT NULL,\
 value       REAL NOT NULL,\
 FOREIGN KEY (particle_id) REFERENCES particles (id),\
 PRIMARY KEY (particle_id, scalar))\
WITHOUT ROWID;";

    db << "COMMIT;";
}

void Database::save_particle(const std::string& s,
                             const std::vector<double>& scalars)
{
    int particle_id;

    db << "BEGIN;";

    db << "INSERT INTO particles (params) VALUES (?);" << s;
    db << "SELECT LAST_INSERT_ROWID();" >> particle_id;

    for(size_t i=0; i<scalars.size(); ++i)
    {
        db << "INSERT INTO scalars VALUES (?, ?, ?);"
           << particle_id << i << scalars[i];
    }

    db << "COMMIT;";
}


} // namespace
