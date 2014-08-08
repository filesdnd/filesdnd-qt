/*
Copyright (c) 2007, Trenton Schulz

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. The name of the author may not be used to endorse or promote products
    derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QtCore/QSocketNotifier>
#include <QtNetwork/QHostInfo>
#include <QTextCodec>

#include "bonjourrecord.h"
#include "bonjourserviceresolver.h"
#include "../helpers/settingsmanager.h"

BonjourServiceResolver::BonjourServiceResolver(QObject *parent)
    : QObject(parent), dnssref(0), bonjourSocket(0), _bonjourPort(-1)
{
    _timeout.setSingleShot(true);
    connect(&_timeout, SIGNAL(timeout()), this, SLOT(cleanupResolve()));
}

BonjourServiceResolver::~BonjourServiceResolver()
{
    cleanupResolve();
}

void BonjourServiceResolver::cleanupResolve()
{
    if (dnssref) {
        DNSServiceRefDeallocate(dnssref);
        dnssref = 0;
        delete bonjourSocket;
        _bonjourPort = -1;

        _timeout.stop();
        emit finish(this);
    }
}

void BonjourServiceResolver::resolveBonjourRecord(const BonjourRecord &record)
{
    if (dnssref) {
        qWarning("Resolve in process, aborting");
        return;
    }

    _currentBonjourRecord = record;
    _timeout.start(BONJOUR_TIMEOUT);
    LogManager::appendLine(QString("[BonjourResolver] Start resolve on : ").append(record.serviceName));

    DNSServiceErrorType err = DNSServiceResolve(&dnssref, 0, 0,
                                                record.serviceName.toUtf8().constData(),
                                                record.registeredType.toUtf8().constData(),
                                                record.replyDomain.toUtf8().constData(),
                                                (DNSServiceResolveReply)bonjourResolveReply, this);
    if (err != kDNSServiceErr_NoError) {
        emit error(err);
    } else {
        sockfd = DNSServiceRefSockFD(dnssref);
        if (sockfd == -1) {
            emit error(kDNSServiceErr_Invalid);
        } else {
            _fullName = record.serviceName; // We keep the name
            bonjourSocket = new QSocketNotifier(sockfd, QSocketNotifier::Read, this);
            connect(bonjourSocket, SIGNAL(activated(int)), this, SLOT(bonjourSocketReadyRead(int)));
        }
    }
}

void BonjourServiceResolver::bonjourSocketReadyRead(int sock)
{
    if(sock == sockfd) {
    DNSServiceErrorType err = DNSServiceProcessResult(dnssref);
    if (err != kDNSServiceErr_NoError)
        emit error(err);
    }
}

void BonjourServiceResolver::bonjourResolveReply(DNSServiceRef, DNSServiceFlags ,
                                    quint32 , DNSServiceErrorType errorCode,
                                    const char *, const char *hosttarget, quint16 port,
                                    quint16 txtLen, const char *txtRecord, void *context)
{
    BonjourServiceResolver *serviceResolver = static_cast<BonjourServiceResolver *>(context);
    if (errorCode != kDNSServiceErr_NoError) {
        emit serviceResolver->error(errorCode);
        return;
    }
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        {
            port =  0 | ((port & 0x00ff) << 8) | ((port & 0xff00) >> 8);
        }
#endif
    serviceResolver->_txtRecordParsed = TxtRecord::parse(txtLen, txtRecord);

    serviceResolver->_bonjourPort = port;
    QHostInfo::lookupHost(QString::fromUtf8(hosttarget),
                          serviceResolver, SLOT(finishConnect(const QHostInfo &)));
}

void BonjourServiceResolver::finishConnect(const QHostInfo &hostInfo)
{
    Device *device = parseDevice(_fullName, hostInfo, _bonjourPort);
    if (device)
    {
        device->setBonjourRecord(_currentBonjourRecord);
        device->setDetectedBy(DETECTED_BY_BONJOUR);
        emit bonjourRecordResolved(device);
    }
    QMetaObject::invokeMethod(this, "cleanupResolve", Qt::QueuedConnection);
}

Device* BonjourServiceResolver::parseDevice(QString name, const QHostInfo &hostInfo, int bonjourPort)
{
    QString uid, stype, version;

    uid = _txtRecordParsed.value("uid");
    stype = _txtRecordParsed.value("type");
    version = _txtRecordParsed.value("version");

    Device *device = NULL;

    if (uid != SettingsManager::getDeviceUID())
        device = new Device(name, stype, uid, hostInfo, bonjourPort, version);

    return device;
}
