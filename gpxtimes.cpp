
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

#include "MySAX2Handler.h"

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <getopt.h>
#include <stdio.h>

XERCES_CPP_NAMESPACE_USE

void usage(char *name)
{
    printf("%s: [-h] <files>\n",  name);
}

int processXmlFile(const char* xmlFile)
{
    std::string xmlFileName = xmlFile;

    try {
	XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	std::cout << "Error during initialization! :\n";
	std::cout << "Exception message is: \n"
	    << message << "\n";
	XMLString::release(&message);
	return 1;
    }

    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
#if 1
    //SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    XMLCh* propertyValue = XMLString::transcode("http://www.topografix.com/GPX/1/1 gpx.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 GpxExtensionsv3.xsd");
    ArrayJanitor<XMLCh> janValue(propertyValue);

    parser->setProperty( XMLUni::fgXercesSchemaExternalSchemaLocation,
		          propertyValue);

#endif
    //parser->setFeature(XMLUni::fgXercesLoadExternalDTD, false);
    //parser->setFeature(XMLUni::fgXercesScannerName, XMLUni::fgWFXMLScanner);
    //parser->setLoadExternalDTD(false);

    //parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
    //parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);   // optional

    MySAX2Handler* defaultHandler = new MySAX2Handler();
    //DefaultHandler* defaultHandler = new DefaultHandler();
    parser->setContentHandler(defaultHandler);
    parser->setErrorHandler(defaultHandler);

    std::cout << "Reading file " << xmlFile << std::endl;
    try {
	parser->parse(xmlFileName.c_str());
    }
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	std::cout << "1 Exception message is: \n"
	    << message << "\n";
	XMLString::release(&message);
	return -1;
    }
    catch (const SAXParseException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	std::cout << "2 Exception message is: \n"
	    << message << "\n";
	XMLString::release(&message);
	return -1;
    }
    catch (...) {
	std::cout << "Unexpected Exception \n" ;
	return -1;
    }
    std::cout << "Read ... " << std::endl;
    std::list<Track::Track*>&trks = defaultHandler->getTracks();
    std::cout << "Read " << trks.size() << " tracks." << std::endl;
    for(std::list<Track::Track*>::iterator it = trks.begin();
	    it != trks.end() ;
	    ++it) {
#if 0
	std::cout << **it << std::endl;
	std::list<Track::TrackSegment*>::const_iterator b = (*it)->begin();
	std::list<Track::TrackSegment*>::const_reverse_iterator e = (*it)->rbegin();

	std::cout << "First segment starts at : " <<
	    (*b)->startTime() << " ends at " <<
	    (*b)->endTime() << std::endl;
	std::cout << "Last segment starts at : " <<
	    (*e)->startTime() << " ends at " <<
	    (*e)->endTime() << std::endl;
#endif
	Track::Time start = (*it)->startTime();
	Track::Time end = (*it)->endTime();
	double duration = ((double)(end.unixtime() - start.unixtime())) / 60.0;
	std::cout << "Track: " << start.date()
	    << " start time: " << start.timeOfDay()
	    <<  " ends time: " << end.timeOfDay()
	    <<  " duration: " << duration
	    << std::endl;

    }

    delete parser;
    delete defaultHandler;
    return 0;
}
int main (int argc, char* argv[])
{
    int c;
    int digit_optind = 0;

    while (1) {
	int this_option_optind = optind ? optind : 1;
	int option_index = 0;
	static struct option long_options[] = {
	    {"help", 0, 0, 'h'},
	    {0, 0, 0, 0}
	};
	c = getopt_long(argc, argv, "h", long_options, &option_index);
	if(c == -1) {
	    break;
	}
	switch(c) {
	    case 'h':
	    default:
		usage(argv[0]);
		break;
	}
    }



    if (optind < argc) {
	while (optind < argc) {
	    processXmlFile(argv[optind++]);
	}
    }
    exit(EXIT_SUCCESS);
}

