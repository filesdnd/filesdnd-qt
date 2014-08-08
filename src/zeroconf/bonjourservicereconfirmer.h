#ifndef BONJOURSERVICERECONFIRMER_H
#define BONJOURSERVICERECONFIRMER_H

#include <QtCore/QObject>
#include <bonjour/dns_sd.h>
#include <QTimer>

#include "../helpers/logmanager.h"

class BonjourRecord;

class BonjourServiceReconfirmer : public QObject
{
    Q_OBJECT
public:
    BonjourServiceReconfirmer(QObject *parent);
    ~BonjourServiceReconfirmer();

    void reconfirmBonjourRecord (const BonjourRecord &record);
    char* getFullName(const BonjourRecord &record);

    DNSServiceRef dnssref;
signals:
    void error(DNSServiceErrorType error);

private slots:
    void cleanupResolve();

private:
    static void DNSSD_API bonjourConfirmReply(DNSServiceRef , const DNSServiceFlags , uint32_t ifIndex,
                                              DNSServiceErrorType errorCode, const char *fullname, uint16_t rrtype,
                                              uint16_t rrclass, uint16_t rdlen, const void *rdata, uint32_t ttl, void *context);
    QTimer _timer;
};
#endif // BONJOURSERVICERECONFIRMER_H
