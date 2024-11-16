#pragma once

#include <iostream>
#include <assert.h>

#define ERROR( MSG ) do { std::cerr << MSG << std::endl; assert( 0 ); } while ( 0 );
