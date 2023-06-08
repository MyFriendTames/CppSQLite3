#include "CppSQLite3.hpp"

SQLite3::SQLite3( const std::string& file ){
  if ( sqlite3_open( file.c_str(), &connection ) != SQLITE_OK ){
    throw std::runtime_error( "Error while opening SQLite3 Database" );
  }
}

SQLite3::~SQLite3(){
  close();
}

int SQLite3::close(){
  int status = SQLITE_OK;
  if ( connection ) status = sqlite3_close( connection );
  connection = nullptr;
  return status;
}

SQLite3::Statement SQLite3::statement( const std::string& sql ){
  SQLite3::Statement statement;
  statement.connection = connection;
  if ( sqlite3_prepare_v2( connection, sql.c_str(), sql.length(), &statement.statement, nullptr ) != SQLITE_OK ){
    throw std::runtime_error( "Error while preparing SQLite3 Statement" );
  }
  return statement;
}

SQLite3::Statement::Statement() :
  connection( nullptr ),
  statement( nullptr ),
  lastInsertId( 0 ){
}

SQLite3::Statement::Statement( SQLite3::Statement&& other ) :
  connection( other.connection ),
  statement( other.statement ),
  keys( std::move( other.keys ) ),
  lastInsertId( other.lastInsertId ){
  other.statement = nullptr;
}

SQLite3::Statement::~Statement(){
  finalize();
}

void SQLite3::Statement::operator = ( SQLite3::Statement&& other ){
  connection = other.connection;
  statement = other.statement;
  keys = std::move( other.keys );
  lastInsertId = other.lastInsertId;
  other.statement = nullptr;
}

int SQLite3::Statement::next(){
  int status = sqlite3_step( statement );
  lastInsertId = static_cast< int >( sqlite3_last_insert_rowid( connection ) );
  for ( int i = columnCount() - 1; i >= 0; --i ){
    keys[ getName( i ) ] = i;
  }
  return status;
}

int SQLite3::Statement::reset(){
  return sqlite3_reset( statement );
}

int SQLite3::Statement::finalize(){
  int status = SQLITE_OK;
  if ( statement ) status = sqlite3_finalize( statement );
  connection = nullptr;
  statement = nullptr;
  lastInsertId = 0;
  return status;
}

int SQLite3::Statement::getLastInsertId(){
  return lastInsertId;
}

int SQLite3::Statement::columnCount(){
  return sqlite3_column_count( statement );
}

std::string SQLite3::Statement::getName( int column ){
  return sqlite3_column_name( statement, column );
}

SQLite3::Datatype SQLite3::Statement::getType( int column ){
  return SQLite3::Datatype( sqlite3_column_type( statement, column ) );
}

SQLite3::Datatype SQLite3::Statement::getType( const std::string& column ){
  return SQLite3::Datatype( sqlite3_column_type( statement, keys[ column ] ) );
}

bool SQLite3::Statement::isNull( int column ){
  return sqlite3_column_type( statement, column ) == SQLITE_NULL;
}

bool SQLite3::Statement::isNull( const std::string& column ){
  return sqlite3_column_type( statement, keys[ column ] ) == SQLITE_NULL;
}

int SQLite3::Statement::getStringLength( int column ){
  return sqlite3_column_bytes( statement, column );
}

int SQLite3::Statement::getStringLength( const std::string& column ){
  return sqlite3_column_bytes( statement, keys[ column ] );
}

int SQLite3::Statement::getInteger( int column ){
  return sqlite3_column_int( statement, column );
}

int SQLite3::Statement::getInteger( const std::string& column ){
  return sqlite3_column_int( statement, keys[ column ] );
}

double SQLite3::Statement::getDouble( int column ){
  return sqlite3_column_double( statement, column );
}

double SQLite3::Statement::getDouble( const std::string& column ){
  return sqlite3_column_double( statement, keys[ column ] );
}

std::string SQLite3::Statement::getString( int column ){
  return reinterpret_cast< const char* >( sqlite3_column_text( statement, column ) );
}

std::string SQLite3::Statement::getString( const std::string& column ){
  return reinterpret_cast< const char* >( sqlite3_column_text( statement, keys[ column ] ) );
}


// Parameter binding mehods
int SQLite3::Statement::clearBindings(){
  return sqlite3_clear_bindings( statement );
}

int SQLite3::Statement::bindInteger( int parameter, int value ){
  return sqlite3_bind_int( statement, parameter + 1, value );
}

int SQLite3::Statement::bindDouble( int parameter, double value ){
  return sqlite3_bind_double( statement, parameter + 1, value );
}

int SQLite3::Statement::bindString( int parameter, const std::string& value ){
  return sqlite3_bind_text( statement, parameter + 1, value.c_str(), value.length(), SQLITE_STATIC );
}

int SQLite3::Statement::bindNull( int parameter ){
  return sqlite3_bind_null( statement, parameter + 1 );
}