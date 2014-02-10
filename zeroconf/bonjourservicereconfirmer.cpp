#include <QtCore/QSocketNotifier>
#include <QtNetwork/QHostInfo>

#include "bonjourrecord.h"
#include "bonjourservicereconfirmer.h"

BonjourServiceReconfirmer::BonjourServiceReconfirmer(QObject *parent)
    : QObject(parent), dnssref(0)
{
    _timer.setSingleShot(true);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(deleteLater()));
}

BonjourServiceReconfirmer::~BonjourServiceReconfirmer()
{
    cleanupResolve();
}

void BonjourServiceReconfirmer::cleanupResolve()
{
    if (dnssref) {
        DNSServiceRefDeallocate(dnssref);
        dnssref = 0;
    }
}

void BonjourServiceReconfirmer::reconfirmBonjourRecord (const BonjourRecord &record)
{
    char *fullname = getFullName(record);

    cleanupResolve();
    LogManager::appendLine("[BonjourReconfirmer] Try reconfirm device");
    DNSServiceErrorType err = DNSServiceQueryRecord(&dnssref, kDNSServiceFlagsForce
                                                    ,0 , fullname, kDNSServiceType_PTR & kDNSServiceType_SRV,
                                                    kDNSServiceClass_IN, (DNSServiceQueryRecordReply) bonjourConfirmReply, this);
    delete[] fullname;
    if (err != kDNSServiceErr_NoError) {
        emit error(err);
    }

    _timer.start(BONJOUR_TIMEOUT);
}

void BonjourServiceReconfirmer::bonjourConfirmReply(DNSServiceRef , const DNSServiceFlags , uint32_t ifIndex, DNSServiceErrorType , const char *fullname, uint16_t rrtype, uint16_t rrclass, uint16_t rdlen, const void *rdata, uint32_t , void *)
{
    LogManager::appendLine("[BonjourReconfirmer] Device reconfirmed");
    DNSServiceReconfirmRecord(kDNSServiceFlagsForce, ifIndex, fullname, rrtype, rrclass, rdlen, rdata);
}

char* BonjourServiceReconfirmer::getFullName(const BonjourRecord &record)
{
    char *fullname = new char[kDNSServiceMaxDomainName];
    DNSServiceErrorType err = DNSServiceConstructFullName(fullname,
       record.serviceName.toUtf8().constData(),
       record.registeredType.toUtf8().constData(),
       record.replyDomain.toUtf8().constData());

    if (err != kDNSServiceErr_NoError) {
        emit error(err);
    }

    return fullname;
}
