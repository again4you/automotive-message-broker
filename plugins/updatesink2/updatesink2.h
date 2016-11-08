

#ifndef UPDATESINK2_H
#define UPDATESINK2_H

#include "ambpluginimpl.h"

class UpdateSink2: public AmbPluginImpl
{
public:
	UpdateSink2(AbstractRoutingEngine* re,
		const std::map<std::string, std::string>& config,
		AbstractSource& parent);
	~UpdateSink2() {};
	const string uuid() const;
	virtual void propertyChanged(AbstractPropertyType *value);
};



#endif /* UPDATESINK2_H */
