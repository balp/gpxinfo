
#include "track.h"

#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE ;

class MySAX2Handler : public DefaultHandler {
    private:
	class state {
	    public:
		state(MySAX2Handler *master) : handler(master) {
		}
		void setState(state *s) {
		    handler->setState(s);
		}
		MySAX2Handler *getHandler() {
		    return(handler);
		}

		virtual ~state() {};
		virtual void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   Attributes&     attrs
			) = 0;
		virtual void characters(
			const XMLCh *const chars,
			const unsigned int length
			) = 0;
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			) = 0;
	    private:
		MySAX2Handler *handler;
		state();
		state(const state &s);
	};
	class gpx : public state {
	    public:
		gpx(MySAX2Handler *master, std::list<Track::Track*> &tracks)
		    : state(master), mTracks(tracks)
		{}
		virtual ~gpx() {};
		virtual void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   Attributes&     attrs
			);
		virtual void characters(
			const XMLCh *const chars,
			const unsigned int length
			) {};
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			);
	    private:
		std::list<Track::Track*> &mTracks;
	};
	class inTrack : public state {
	    public:
		inTrack(MySAX2Handler *master, state *prevState, Track::Track &track)
		    : state(master), pState(prevState), mTrack(track)
		{}
		virtual ~inTrack() {};
		virtual void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   Attributes&     attrs
			);
		virtual void characters(
			const XMLCh *const chars,
			const unsigned int length
			);
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			);
	    protected:
		Track::Track &mTrack;
		state *pState;
	};

	class inTrackName : public inTrack {
	    public:
		inTrackName(MySAX2Handler *master, state *prevState,
			Track::Track &track)
		    : inTrack(master, prevState, track)
		{}
		virtual ~inTrackName() {};
		virtual void characters(
			const XMLCh *const chars,
			const unsigned int length
			);
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			);
	    private:
	};

	class inTrackSegment : public inTrack {
	    private:
		Track::TrackSegment *mSegment;
	    public:
		inTrackSegment(MySAX2Handler *master, state *prevState,
			Track::Track &track)
		    : inTrack(master, prevState, track), mSegment(NULL)
		{ 
		    mSegment = new Track::TrackSegment();
		    track.addSegment(mSegment);
		}
		virtual ~inTrackSegment() {};
		virtual void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   Attributes&     attrs
			);
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			);
	    private:
	};

	class inTrackPoint : public inTrack {
	    private:
		Track::TrackPoint &mPoint;
	    public:
		inTrackPoint(MySAX2Handler *master, state *prevState,
			Track::Track &track, 
			Track::TrackPoint &trackPoint)
		    : inTrack(master, prevState, track), mPoint(trackPoint)
		{}
		virtual ~inTrackPoint() {};
		virtual void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   Attributes&     attrs
			);
		virtual void characters(
			const XMLCh *const chars,
			const unsigned int length
			);
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			);
	    private:
	};

	class inTrackPointTime : public inTrack {
	    private:
		Track::TrackPoint &mPoint;
	    public:
		inTrackPointTime(MySAX2Handler *master, state *prevState,
			Track::Track &track, 
			Track::TrackPoint &trackPoint)
		    : inTrack(master, prevState, track), mPoint(trackPoint)
		{}
		virtual ~inTrackPointTime() {};
		virtual void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   Attributes&     attrs
			) {};
		virtual void characters(
			const XMLCh *const chars,
			const unsigned int length
			);
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			);
	    private:
	};

	class inTrackPointEle : public inTrack {
	    private:
		Track::TrackPoint &mPoint;
	    public:
		inTrackPointEle(MySAX2Handler *master, state *prevState,
			Track::Track &track, 
			Track::TrackPoint &trackPoint)
		    : inTrack(master, prevState, track), mPoint(trackPoint)
		{}
		virtual ~inTrackPointEle() {};
		virtual void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   Attributes&     attrs
			) {};
		virtual void characters(
			const XMLCh *const chars,
			const unsigned int length
			);
		virtual void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname
			);
	    private:
	};

	state *mState;
	std::list<Track::Track*> mTracks;

    public:
	MySAX2Handler() : mState(NULL) {
	    mState = new gpx(this, mTracks);
	};

	std::list<Track::Track*>& getTracks() {
	    return mTracks;
	}

	void setState(state *newState) {
	    mState = newState;
	}

	void startElement(
		const   XMLCh* const    uri,
		const   XMLCh* const    localname,
		const   XMLCh* const    qname,
		const   Attributes&     attrs
		);
	virtual void fatalError(const SAXParseException&);
	virtual void characters(
		const XMLCh *const chars,
		const unsigned int length
		);
	virtual void endElement(
		const XMLCh *const uri,
		const XMLCh *const localname,
		const XMLCh *const qname
		);
};
