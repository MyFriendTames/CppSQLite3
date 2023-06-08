#ifndef CPP_SQQLITE3_DB_HPP
#define CPP_SQQLITE3_DB_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>

#include "sqlite3.h"

class SQLite3{
  public:
    enum class Datatype{
      Integer = SQLITE_INTEGER,
      Float = SQLITE_FLOAT,
      Blob = SQLITE_BLOB,
      Null = SQLITE_NULL,
      Text = SQLITE3_TEXT
    };

    class Statement{
      friend SQLite3;
      private:
        sqlite3* connection;
        sqlite3_stmt* statement;
        std::unordered_map< std::string, int > keys;
        int lastInsertId;

      public:
        Statement();
        Statement( const Statement& ) = delete;
        Statement( Statement&& );
        ~Statement();

        void operator = ( const Statement& ) = delete;
        void operator = ( Statement&& );

        int next();
        int reset();
        int finalize();
        int getLastInsertId();

        // Column methods
        int columnCount();

        std::string getName( int );
        Datatype getType( int );
        Datatype getType( const std::string& );
        bool isNull( int );
        bool isNull( const std::string& );
        int getStringLength( int );
        int getStringLength( const std::string& );

        int getInteger( int );
        int getInteger( const std::string& );
        double getDouble( int );
        double getDouble( const std::string& );
        std::string getString( int );
        std::string getString( const std::string& );

        // Parameter binding methods
        int clearBindings();
        int bindInteger( int, int );
        int bindDouble( int, double );
        int bindString( int, const std::string& );
        int bindNull( int );
    };

  private:
    sqlite3* connection;
  
  public:
    SQLite3( const std::string& );
    SQLite3( const SQLite3& ) = delete;
    ~SQLite3();

    int close();

    Statement statement( const std::string& );
};

#endif