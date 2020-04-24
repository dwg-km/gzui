#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#include <QWidget>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QObject>

#include "iconbutton.h"

struct NetworkType
{
    bool IsStatic = false;
    QString Address;
    QString NetMask;
    QString GateWay;
};

class NetworkWidget :public QWidget
{
    Q_OBJECT

public:

    void getNetworkPort()
    {
        m_NetPort->addItem("eth0");
        m_NetPort->addItem("eth1");
    }

    bool getNetworkConfig();
    string getLocalIp(QString Gport);

    NetworkWidget()
    {
        m_LabNetPort = new QLabel(tr("port:"));
        m_NetPort = new QComboBox;
        m_dhcp = new QRadioButton(tr("dhcp"));
        m_static = new QRadioButton(tr("static"));
        m_RestarNet = new QPushButton(tr("OK"));

        m_portButtonGroup = new QButtonGroup(this);
        m_portButtonGroup->addButton(m_dhcp);
        m_portButtonGroup->addButton(m_static);
        m_dhcp->setChecked(true);

        connect(this->m_RestarNet, SIGNAL(clicked()), this, SLOT(RestartNetwork()));
        connect(m_portButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(SetPortState(int)));

        getNetworkPort();
        connect(m_NetPort, SIGNAL(currentIndexChanged(int)), this, SLOT(NetPortChange(int)));

        m_LabIp = new QLabel(tr("IP:"));
        m_LineIp = new DoubleLineEdit;
        m_LineIp->getTouch()->setIsNotString(false);
        m_LabNetmask = new QLabel(tr("NETMASK:"));
        m_LineNetmask = new DoubleLineEdit;
        m_LineNetmask->getTouch()->setIsNotString(false);
        m_LabGateway = new QLabel(tr("GATEWAY:"));
        m_LineGateway = new DoubleLineEdit;
        m_LineGateway->getTouch()->setIsNotString(false);

        NetworkLayout = new QGridLayout();
        NetworkLayout->addWidget(m_LabNetPort,0,0,1,1);
        NetworkLayout->addWidget(m_NetPort,0,1,1,1);
        NetworkLayout->addWidget(m_dhcp,1,0,1,2);
        NetworkLayout->addWidget(m_static,2,0,1,2);
        NetworkLayout->addWidget(m_LabIp,3,0,1,1);
        NetworkLayout->addWidget(m_LineIp,3,1,1,1);
        NetworkLayout->addWidget(m_LabNetmask,4,0,1,1);
        NetworkLayout->addWidget(m_LineNetmask,4,1,1,1);
        NetworkLayout->addWidget(m_LabGateway,5,0,1,1);
        NetworkLayout->addWidget(m_LineGateway,5,1,1,1);
        NetworkLayout->addWidget(m_RestarNet,6,2,1,1);
        m_AllWidget = new QWidget;
        m_AllWidget->setLayout(NetworkLayout);
        m_AllWidget->setFixedWidth(400);

        NetAllLayout = new QHBoxLayout();
        NetAllLayout->addWidget(m_AllWidget);
        setLayout(NetAllLayout);


    }

public slots:
    void TabOpen();
    void setButtonDisable(bool IsDisabel);
    void SetPortState(int change);
    void NetPortChange(int PortIndex);
    void SaveNetWorkConfig(QString port);
    void RestartNetwork();

private:
    QString m_path = "/etc/network/interfaces";

    QLabel * m_LabNetPort;
    QComboBox * m_NetPort;
    QRadioButton * m_dhcp;
    QRadioButton * m_static;
    QPushButton * m_RestarNet;
    QButtonGroup * m_portButtonGroup;

    QWidget * m_AllWidget;
    QHBoxLayout * NetAllLayout;

    QLabel * m_LabIp;
    DoubleLineEdit * m_LineIp;
    QLabel * m_LabNetmask;
    DoubleLineEdit * m_LineNetmask;
    QLabel * m_LabGateway;
    DoubleLineEdit * m_LineGateway;

    NetworkType m_eth0;
    NetworkType m_eth1;

    QGridLayout * NetworkLayout;
};
