
#include "track.h"

#include <cxxtest/TestSuite.h>

class MyTestSuite : public CxxTest::TestSuite 
{
    public:
	void testTime( void )
	{
	    Track::Time t = Track::Time("2009-10-05T17:35:12Z");
	    TS_ASSERT_EQUALS( t.year(), 2009);
	    TS_ASSERT_EQUALS( t.month(), 10);
	    TS_ASSERT_EQUALS( t.day(), 5);
	    TS_ASSERT_EQUALS( (unsigned long int)t.unixtime(), (unsigned long int)1254764112);
	}
};

