
#define _XOPEN_SOURCE
#include <time.h>

#include "track.h"


std::ostream& operator<<(std::ostream &os, const Track::Time& t)
{
    os << t.asString() ;
    return os;
}
std::ostream& operator<<(std::ostream& os, const Track::TrackPoint& t)
{
    os << (t.lat()) << ":" << t.lon() << ":" << t.ele() << ":" << t.time() ;
    return os;
}

std::ostream& operator<<(std::ostream &os, const Track::TrackSegment& t)
{
    os << "Points: " << t.size() << ": ..." ;
    return os;
}
std::ostream& operator<<(std::ostream &os, const Track::Track& t)
{
    os << "Name: " << t.name() << " Segments: " << t.size() << ": ..." ;
    return os;
}

void Track::Time::convert_iso8601(const char *time_string,
	int ts_len, struct tm *tm_data)
{
    tzset();

    char temp[64];
    memset(temp, 0, sizeof(temp));
    strncpy(temp, time_string, ts_len);

    struct tm ctime;
    memset(&ctime, 0, sizeof(struct tm));
    strptime(temp, "%FT%T%z", &ctime);

    ts = mktime(&ctime) - timezone;
    localtime_r(&ts, tm_data);
}

Track::Time::Time(const std::string &when)
{
    memset(&mTime, 0, sizeof(mTime));
    convert_iso8601(when.c_str(), when.size(), &mTime); 
}
