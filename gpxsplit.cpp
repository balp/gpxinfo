
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

XERCES_CPP_NAMESPACE_USE

int main (int argc, char* args[]) {

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


    const char* xmlFile = "x1.xml";
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    // parser->setFeature(XMLUni::fgXercesLoadExternalDTD, false);
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);   // optional

    MySAX2Handler* defaultHandler = new MySAX2Handler();
    //DefaultHandler* defaultHandler = new DefaultHandler();
    parser->setContentHandler(defaultHandler);
    parser->setErrorHandler(defaultHandler);

    try {
	parser->parse(xmlFile);
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
    std::list<Track::Track*>&trks = defaultHandler->getTracks();
    std::cout << "Read " << trks.size() << " tracks." << std::endl;
    for(std::list<Track::Track*>::iterator it = trks.begin();
	    it != trks.end() ;
	    ++it) {
	std::cout << **it << std::endl;
    }

    delete parser;
    delete defaultHandler;
    return 0;
}

