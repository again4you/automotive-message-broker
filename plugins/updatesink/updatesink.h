
#ifndef UPATESINK_H
#define UPATESINK_H

#include <abstractsink.h>
#include <string>

class UpdateSink: public AbstractSink
{
public:
	UpdateSink(AbstractRoutingEngine* re, map<string, string> config);
	~UpdateSink();

	virtual const std::string uuid();
	virtual void supportedChanged(const PropertyList & supportedProperties);
	virtual void propertyChanged(AbstractPropertyType* value);
	virtual PropertyList subscriptions();
};

#endif /* UPATESINK_H */
