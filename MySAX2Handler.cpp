
#include "MySAX2Handler.h"
#include "track.h"

#include <xercesc/sax2/Attributes.hpp>

#include <string>
#include <iostream>

#include <stdlib.h>
#include <time.h>


void MySAX2Handler::startElement(const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname,
	const   Attributes&     attrs)
{
    // char* message = XMLString::transcode(localname);
    mState->startElement(uri, localname, qname, attrs);
    // std::cout << "I saw element: "<< message << std::endl;
    // XMLString::release(&message);
}

void MySAX2Handler::fatalError(const SAXParseException& exception)
{
    char* message = XMLString::transcode(exception.getMessage());
    std::cout << "Fatal Error: " << message
	<< " at line: " << exception.getLineNumber()
	<< std::endl;
    XMLString::release(&message);
}
void MySAX2Handler::characters(
	const XMLCh *const chars,
	const unsigned int length
	)
{
    // char* message = XMLString::transcode(chars);
    mState->characters(chars, length);
    // std::cout << "I got data: "<< message << std::endl;
    // XMLString::release(&message);
}

void MySAX2Handler::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
    mState->endElement(uri, localname, qname);
    // char* message = XMLString::transcode(localname);
    // std::cout << "End of element: "<< message << std::endl;
    // XMLString::release(&message);
}



/**
 * Gpx class
 */
void MySAX2Handler::gpx::startElement(const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname,
	const   Attributes&     attrs)
{
    char* message = XMLString::transcode(localname);
    // char* qn = XMLString::transcode(qname);
    // std::cout << "I saw element: "<< message << ":" << qn << std::endl;
    if(!strcmp(message, "trk")) {
	// std::cout << "Starting Track" << std::endl;
	Track::Track *trk = new Track::Track();
	mTracks.push_back(trk);
	setState(new inTrack(getHandler(), this, *trk));
    }
    XMLString::release(&message);
    // XMLString::release(&qn);
}

void MySAX2Handler::gpx::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
#if 0
    char* message = XMLString::transcode(localname);
    std::cout << "End of element: "<< message << std::endl;
    XMLString::release(&message);
#endif
}

/**
 * inTrack Stuff
 */
void MySAX2Handler::inTrack::startElement(const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname,
	const   Attributes&     attrs)
{
    char* message = XMLString::transcode(localname);
    char* qn = XMLString::transcode(qname);
    if(!strcmp(message, "name")) {
	setState(new inTrackName(getHandler(), this, mTrack));
    } else if(!strcmp(message, "trkseg")) {
	setState(new inTrackSegment(getHandler(), this, mTrack));
    } else {
	std::cout << "Unknown tag:"<< message << ":" << qn <<": in track"<< std::endl;
    }
    XMLString::release(&message);
    XMLString::release(&qn);
}

void MySAX2Handler::inTrack::characters(
	const XMLCh *const chars,
	const unsigned int length
	)
{
}

void MySAX2Handler::inTrack::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "trk")) {
	// std::cout << "End of track: "<< mTrack << std::endl;
	setState(pState);
    } else {
	std::cout << "End of unhandeled track: "<< message << std::endl;
    }
    XMLString::release(&message);
}

void MySAX2Handler::inTrackName::characters(
	const XMLCh *const chars,
	const unsigned int length
	)
{
    char* message = XMLString::transcode(chars);
    mTrack.setName(message);
    XMLString::release(&message);
}

void MySAX2Handler::inTrackName::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "name")) {
	setState(pState);
    } else {
	std::cout << "End of unhandeled in name: "<< message << std::endl;
    }
    XMLString::release(&message);
}

void MySAX2Handler::inTrackSegment::startElement(const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname,
	const   Attributes&     attrs)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "trkpt")) {
	XMLCh* latStr = XMLString::transcode("lat");
	XMLCh* lonStr = XMLString::transcode("lon");
	const   XMLCh* xmlLat = attrs.getValue(latStr);
	const   XMLCh* xmlLon = attrs.getValue(lonStr);
	XMLString::release(&latStr);
	XMLString::release(&lonStr);
	if(xmlLat && xmlLon) {
	    char* slat = XMLString::transcode(xmlLat);
	    char* slon = XMLString::transcode(xmlLon);
	    double lat = strtod(slat, (char **)NULL);
	    double lon = strtod(slon, (char **)NULL);
	    Track::TrackPoint *tp = new Track::TrackPoint(lat, lon);
	    mSegment->addPoint(tp);
	    //std::cout << "New point: (" << lat << ", " << lon << ") : " << *tp << std::endl;
	    setState(new inTrackPoint(getHandler(), this, mTrack,  *tp));
	    XMLString::release(&slat);
	    XMLString::release(&slon);
	} else {
	    std::cout << "Can't find lat " << xmlLat << " and " << xmlLon
		<< std::endl;
	}
    } else {
	std::cout << "Unknown tag:"<< message << ":" << ": in track segment"<< std::endl;
    }
    XMLString::release(&message);
}

void MySAX2Handler::inTrackSegment::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "trkseg")) {
	// std::cout << "End of track segment." << *mSegment << std::endl;
	//mTrack.addSegment(mSegment);
	setState(pState);
    } else {
	std::cout << "End of unhandels track segment: "<< message << std::endl;
    }
    XMLString::release(&message);
}

void MySAX2Handler::inTrackPoint::startElement(const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname,
	const   Attributes&     attrs)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "time")) {
	setState(new inTrackPointTime(getHandler(), this, mTrack,  mPoint));
    } else if(!strcmp(message, "ele")) {
	setState(new inTrackPointEle(getHandler(), this, mTrack,  mPoint));
    } else {
	std::cout << "Unknown tag:"<< message << ":" << ": in track point"<< std::endl;
    }
    XMLString::release(&message);
}

void MySAX2Handler::inTrackPoint::characters(
	const XMLCh *const chars,
	const unsigned int length
	)
{
	std::cout << "Unhandeled track data: " << std::endl;
}

void MySAX2Handler::inTrackPoint::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "trkpt")) {
	//std::cout << "End of track point:"<< mPoint <<  std::endl;
	setState(pState);
    } else {
	std::cout << "End of unhandels track point part: "<< message << std::endl;
    }
    XMLString::release(&message);
}

void MySAX2Handler::inTrackPointTime::characters(
	const XMLCh *const chars,
	const unsigned int length
	)
{
    char* message = XMLString::transcode(chars);
    struct tm myTime;
    if(strptime(message, "%Y-%m-%dT%TZ", &myTime)) {
	//mPoint.setTime(&myTime);
	mPoint.setTime(Track::Time(message));
    } else {
	std::cout << "Track point time not understood: " << message
	    << std::endl;
    }
    XMLString::release(&message);
}

void MySAX2Handler::inTrackPointTime::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "time")) {
	setState(pState);
    } else {
	std::cout << "Unknown element in Track Point time: "
	    << message << std::endl;
    }
    XMLString::release(&message);
}
void MySAX2Handler::inTrackPointEle::characters(
	const XMLCh *const chars,
	const unsigned int length
	)
{
    char* message = XMLString::transcode(chars);
    mPoint.setElevation(strtod(message, (char **)NULL));
    XMLString::release(&message);
}

void MySAX2Handler::inTrackPointEle::endElement(
	const XMLCh *const uri,
	const XMLCh *const localname,
	const XMLCh *const qname
	)
{
    char* message = XMLString::transcode(localname);
    if(!strcmp(message, "ele")) {
	setState(pState);
    } else {
	std::cout << "Unknown element in TracpPoint elevation: "<< message << std::endl;
    }
    XMLString::release(&message);
}
