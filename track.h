#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <list>
#include <time.h>
#include <iostream>

#include <string.h>
#include <time.h>

namespace Track {
    class Time {
	public:
	    Time() {
		ts = time(NULL);
		gmtime_r(&ts, &mTime);
	    }
	    Time(const std::string &when);
	    int year() const {
		return 1900 + mTime.tm_year;
	    };
	    int month() const {
		return 1 + mTime.tm_mon;
	    }
	    int day() const {
		return mTime.tm_mday;
	    }
	    time_t unixtime() const {
		return ts;
	    }
	    const struct tm *getTM() const {
		return &mTime;
	    }
	    std::string date() const {
		char buff[50];
		strftime(buff, sizeof(buff), "%x", getTM() );
		return std::string(buff);
	    }

	    std::string timeOfDay() const {
		char buff[50];
		strftime(buff, sizeof(buff), "%X", getTM() );
		return std::string(buff);
	    }

	    std::string asString(void) const {
		char buff[50];
		strftime(buff, sizeof(buff), "%FT%T%z", getTM() );
		return std::string(buff);
	    }

	private:
	    struct tm mTime;
	    time_t ts;
	    void convert_iso8601(const char *time_string,                                    int ts_len, struct tm *tm_data);

    };

    class TrackPoint {
	public:
	    TrackPoint(double lat, double lon) : mlat(lat), mlon(lon) {};
	    void setTime(const Time& time) {
		mTime = time;
	    }
	    void setElevation(double elevation) {
		mElevation = elevation;
	    }
	    double lat(void) const {
		return mlat;
	    }
	    double lon(void) const {
		return mlon;
	    }
	    double ele(void) const {
		return mElevation;
	    }
	    const Time& time() const {
		return mTime;
	    }
	private:
	    double mlat;
	    double mlon;
	    //struct tm myTime;
	    double mElevation;
	    Time mTime;
    };
    double distance(const TrackPoint& from, const TrackPoint& to);
    /*
     * Haversine formula: Distance
     * R = earth’s radius (mean radius = 6,371km)
     * Δlat = lat2− lat1
     * Δlong = long2− long1
     * a = sin²(Δlat/2) + cos(lat1).cos(lat2).sin²(Δlong/2)
     * c = 2.atan2(√a, √(1−a))
     * d = R.c 
     *
     * var R = 6371; // km
     * var dLat = (lat2-lat1).toRad();
     * var dLon = (lon2-lon1).toRad(); 
     * var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
     *         Math.cos(lat1.toRad()) * Math.cos(lat2.toRad()) * 
     *         Math.sin(dLon/2) * Math.sin(dLon/2); 
     * var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a)); 
     * var d = R * c;
     */

    class TrackSegment {
	public:
	    TrackSegment() {};
	    void addPoint(TrackPoint* point) {
		mPoints.push_back(point);
	    }
	    size_t size(void) const {
		return mPoints.size();
	    }

	    Time startTime() const {
		TrackPoint* first = *(mPoints.begin());
		if(first) {
		    return first->time();
		}
		return Time();
	    }

	    Time endTime() const {
		TrackPoint* last = *(mPoints.rbegin());
		if(last) {
		    return last->time();
		}
		return Time();
	    }

	    std::list<TrackPoint*>::const_iterator begin() const {
		return mPoints.begin();
	    }
	    std::list<TrackPoint*>::const_iterator end() const {
		return mPoints.end();
	    }
	    std::list<TrackPoint*>::const_reverse_iterator rbegin() const {
		return mPoints.rbegin();
	    }
	    std::list<TrackPoint*>::const_reverse_iterator rend() const {
		return mPoints.rend();
	    }
	private:
	    std::list<TrackPoint*> mPoints;
    };

    class Track {
	public:
	    Track() : mName("unkown") {};
	    void addSegment(TrackSegment* seg) {
		mSegments.push_back(seg);
	    }
	    void setName(const char* name) {
		mName = std::string(name);
	    }
	    const std::string& name() const{
		return mName;
	    }
	    size_t size() const {
		return mSegments.size();
	    }
	    std::list<TrackSegment*>::const_iterator begin() const {
		return mSegments.begin();
	    }
	    std::list<TrackSegment*>::const_iterator end() const {
		return mSegments.end();
	    }
	    std::list<TrackSegment*>::const_reverse_iterator rbegin() const {
		return mSegments.rbegin();
	    }
	    std::list<TrackSegment*>::const_reverse_iterator rend() const {
		return mSegments.rend();
	    }

	    Time startTime() const {
		TrackSegment* first = *(mSegments.begin());
		if(first) {
		    return first->startTime();
		}
		return Time();
	    }

	    Time endTime() const {
		TrackSegment* last = *(mSegments.rbegin());
		if(last) {
		    return last->endTime();
		}
		return Time();
	    }
	private:
	    std::string mName;
	    std::list<TrackSegment*> mSegments;

    };
};
std::ostream& operator<<(std::ostream &os, const Track::Time& t);
std::ostream& operator<<(std::ostream &os, const Track::Track& t);
std::ostream& operator<<(std::ostream &os, const Track::TrackPoint& t);
std::ostream& operator<<(std::ostream &os, const Track::TrackSegment& t);
#endif // TRACK_H
