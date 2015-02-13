// Copyright Hugh Perkins 2015 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License, 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <stdexcept>

#include "stringhelper.h"
#include "GtestGlobals.h"

#include "TestArgsParser.h"

using namespace std;

TestArgsParser::TestArgsParser() :
    argc(GtestGlobals::instance()->argc),
    argv(GtestGlobals::instance()->argv) {
}
void TestArgsParser::_arg( std::string key, int *p_value ) {
    args.push_back( new ArgInt( key, p_value ) );
}
void TestArgsParser::_arg( std::string key, std::string *p_value ) {
    args.push_back( new ArgString( key, p_value ) );
}
void TestArgsParser::_printAvailableKeys() {
    cout << "Available keys:" << endl;
    for( vector< Arg * >::iterator it = args.begin(); it != args.end(); it++ ) {
        cout << "   " << (*it)->key << endl;
    }
}
void TestArgsParser::_go() {
    if( argc == 2 && string( argv[1] ) == "--help" ) {
        _printAvailableKeys();
        return;
    }
    for( int i = 1; i < argc; i++ ) {
        bool found = false;
        string thisKeyValue = string( argv[i] );
        vector<string> splitKeyValue = split( thisKeyValue, "=" );
        if( splitKeyValue.size() != 2 ) {
            throw runtime_error( "argument [" + thisKeyValue + "] not in format [key]=[value]" );
        }
        string key = splitKeyValue[0];
        string valueString = splitKeyValue[1];
        for( vector< Arg * >::iterator it = args.begin(); it != args.end(); it++ ) {
            if( (*it)->key == key ) {
                found = true;
                (*it)->apply( valueString );
            }
        }
        if( !found ) {
            _printAvailableKeys();
            cout << endl;
            throw runtime_error("key [" + key + "] not found");
        }
    }
}
void ArgInt::apply( std::string stringValue ) {
    *p_int = atoi( stringValue );
}
void ArgString::apply( std::string stringValue ) {
    *p_value = stringValue;
}
TestArgsParser *TestArgsParser::instance() {
    static TestArgsParser *thisInstance = new TestArgsParser();
    return thisInstance;
}
void TestArgsParser::arg( std::string key, int *p_value ) {
    instance()->_arg( key, p_value );
}
void TestArgsParser::arg( std::string key, std::string *p_value ) {
    instance()->_arg( key, p_value );
}
void TestArgsParser::go() {
    instance()->_go();
    instance()->args.clear();
}


