#include <iostream>
#include <fstream>
#include <json_spirit.h>

using namespace std;
using namespace json_spirit;

int main()
{
    Array age_arr;
    age_arr.push_back( 4 );
    age_arr.push_back( 7 );
    age_arr.push_back( 12 );

    Object addr_obj;

    addr_obj.push_back( Pair( "house_number", 42 ) );
    addr_obj.push_back( Pair( "road",         "East Street" ) );
    addr_obj.push_back( Pair( "town",         "Newtown" ) );
    addr_obj.push_back( Pair( "childrens_ages", age_arr ) );

    ofstream os( "address.txt" );

    write( addr_obj, os, pretty_print );
    
    os.close();
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k json_spirit_test"
** End:
*/
