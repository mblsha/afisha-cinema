#include "cinema.h"

#include <QDebug>
#include <QDomElement>
#include <QDomDocument>

#include "xmpp_xmlcommon.h"

Cinema::Cinema()
	: QObject()
{
}

Cinema::~Cinema()
{
}

void Cinema::updateFromWeb()
{
}

void Cinema::initFromXml(const QDomElement& e)
{
	Q_ASSERT(e.tagName() == "cinema");
	clear();

	id_ = XMLHelper::subTagText(e, "id");
	name_ = XMLHelper::subTagText(e, "name");
}

void Cinema::clear()
{
	id_ = QString();
	name_ = QString();
	address_ = QString();
	mapUrl_ = QString();
	phones_ = QStringList();
	lastUpdatedAt_ = QDateTime::currentDateTime();
}
