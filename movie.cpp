#include "movie.h"

#include <QDebug>
#include <QDomElement>
#include <QDomDocument>

#include "xmpp_xmlcommon.h"
#include "httprequest.h"
#include "afishahelpers.h"

Movie::Movie()
	: QObject()
{
}

Movie::~Movie()
{
}

void Movie::initFromData(const QString& xml)
{
}

void Movie::initFromXml(const QDomElement& e)
{
	Q_ASSERT(e.tagName() == "movie");
	clear();

	id_ = XMLHelper::subTagText(e, "id");
	name_ = XMLHelper::subTagText(e, "name");

	// if (!e.attribute("detailed").isEmpty()) {
		// if (!details_.isEmpty())
			qWarning() << id_ << name_;
	// }
}

void Movie::updateFromWeb()
{
}

void Movie::clear()
{
}

void Movie::requestFinished()
{
}
