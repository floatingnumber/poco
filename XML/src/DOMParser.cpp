//
// DOMParser.cpp
//
// $Id: //poco/1.3/XML/src/DOMParser.cpp#4 $
//
// Library: XML
// Package: DOM
// Module:  DOMParser
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/DOMBuilder.h"
#include "Poco/SAX/WhitespaceFilter.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/XML/NamePool.h"
#include <sstream>


namespace Poco {
namespace XML {


const XMLString DOMParser::FEATURE_FILTER_WHITESPACE = toXMLString("http://www.appinf.com/features/no-whitespace-in-element-content");


DOMParser::DOMParser(NamePool* pNamePool):
	_pNamePool(pNamePool),
	_filterWhitespace(false)
{
	if (_pNamePool) _pNamePool->duplicate();
	_saxParser.setFeature(XMLReader::FEATURE_NAMESPACES, true);
	_saxParser.setFeature(XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
}


DOMParser::~DOMParser()
{
	if (_pNamePool) _pNamePool->release();
}


void DOMParser::setEncoding(const XMLString& encoding)
{
	_saxParser.setEncoding(encoding);
}


const XMLString& DOMParser::getEncoding() const
{
	return _saxParser.getEncoding();
}


void DOMParser::addEncoding(const XMLString& name, Poco::TextEncoding* pEncoding)
{
	_saxParser.addEncoding(name, pEncoding);
}


void DOMParser::setFeature(const XMLString& name, bool state)
{
	if (name == FEATURE_FILTER_WHITESPACE)
		_filterWhitespace = state;
	else
		_saxParser.setFeature(name, state);
}


bool DOMParser::getFeature(const XMLString& name) const
{
	if (name == FEATURE_FILTER_WHITESPACE)
		return _filterWhitespace;
	else
		return _saxParser.getFeature(name);
}


Document* DOMParser::parse(const XMLString& uri)
{
	if (_filterWhitespace)
	{
		WhitespaceFilter filter(&_saxParser);
		DOMBuilder builder(filter, _pNamePool);
		return builder.parse(uri);
	}
	else
	{
		DOMBuilder builder(_saxParser, _pNamePool);
		return builder.parse(uri);
	}
}


Document* DOMParser::parse(InputSource* pInputSource)
{
	if (_filterWhitespace)
	{
		WhitespaceFilter filter(&_saxParser);
		DOMBuilder builder(filter, _pNamePool);
		return builder.parse(pInputSource);
	}
	else
	{
		DOMBuilder builder(_saxParser, _pNamePool);
		return builder.parse(pInputSource);
	}
}


Document* DOMParser::parseString(const std::string& xml)
{
	return parseMemory(xml.data(), xml.size());
}


Document* DOMParser::parseMemory(const char* xml, std::size_t size)
{
	if (_filterWhitespace)
	{
		WhitespaceFilter filter(&_saxParser);
		DOMBuilder builder(filter, _pNamePool);
		return builder.parseMemoryNP(xml, size);
	}
	else
	{
		DOMBuilder builder(_saxParser, _pNamePool);
		return builder.parseMemoryNP(xml, size);
	}
}


EntityResolver* DOMParser::getEntityResolver() const
{
	return _saxParser.getEntityResolver();
}


void DOMParser::setEntityResolver(EntityResolver* pEntityResolver)
{
	_saxParser.setEntityResolver(pEntityResolver);
}


} } // namespace Poco::XML
