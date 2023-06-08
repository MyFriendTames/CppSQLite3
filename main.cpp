#include <iostream>
#include <cwchar>

#include "SQLite3.hpp"

int main(){
  SQLite3 db( "mydb.db" );

  db.statement( "CREATE TABLE IF NOT EXISTS Persons ("
    "id INTEGER PRIMARY KEY,"
    "name TEXT,"
    "lastName TEXT"
  ")" ).next();

  db.statement( "DELETE FROM Persons;" ).next();

  //db.statement( "INSERT INTO Persons ( name, lastName ) VALUES"
  //"( 'Diego', 'Tames Vargas' ),"
  //"( 'Eduardo', 'Tames Vargas' ),"
  //"( 'Daniela', 'Quiros Nunez' )" ).next();

  SQLite3::Statement stmt;

  stmt = db.statement( "INSERT INTO Persons ( id, name, lastName ) VALUES ( ?, ?, ? )" );
  stmt.bindInteger( 0, 5 );
  stmt.bindString( 1, "Diego" );
  stmt.bindString( 2, "Tames Vargas" );
  stmt.next();
  std::cout << "id: " << stmt.getLastInsertId() << std::endl;
  stmt.reset();
  stmt.bindInteger( 0, 10 );
  stmt.bindString( 1, "Eduardo" );
  stmt.bindString( 2, "Tames Vargas" );
  stmt.next();
  std::cout << "id: " << stmt.getLastInsertId() << std::endl;

  stmt = db.statement( "SELECT id, name, lastName FROM Persons" );
  stmt.bindInteger( 0, 10 );

  std::cout << "Persons: " << std::endl;
  std::cout << "id\t\tname\t\tlastName" << std::endl;
  while ( stmt.next() == SQLITE_ROW ){
    std::cout << stmt.getInteger( "id" ) << "\t\t" << stmt.getString( "name" ) << "\t\t" << stmt.getString( "lastName" ) << std::endl;
  }

  return 0;
}