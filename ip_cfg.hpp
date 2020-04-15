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
        m_NetPort = new QComboBox;
        m_ip = new TipLineEdit;
        m_netmask = new TipLineEdit;
        m_gateway = new TipLineEdit;
        m_dhcp = new QRadioButton(tr("dhcp"));
        m_static = new QRadioButton(tr("static"));
        staticWidget = new QWidget;
        m_RestarNet = new QPushButton(tr("OK"));

        m_portButtonGroup = new QButtonGroup(this);
        m_portButtonGroup->addButton(m_dhcp);
        m_portButtonGroup->addButton(m_static);
        m_dhcp->setChecked(true);

        connect(this->m_RestarNet, SIGNAL(clicked()), this, SLOT(RestartNetwork()));
        connect(m_portButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(SetPortState(int)));

        getNetworkPort();
        connect(m_NetPort, SIGNAL(currentIndexChanged(int)), this, SLOT(NetPortChange(int)));

        ConfigLayout = new QVBoxLayout();
        m_ip->setLab("IP:");
        ConfigLayout->addWidget(m_ip);
        m_netmask->setLab("NETMASK:");
        ConfigLayout->addWidget(m_netmask);
        m_gateway->setLab("GATEWAY:");
        ConfigLayout->addWidget(m_gateway);
        staticWidget->setLayout(ConfigLayout);

        NetworkLayout = new QGridLayout();
        NetworkLayout->addWidget(m_NetPort,0,0,1,1);
        NetworkLayout->addWidget(m_dhcp,1,0,1,1);
        NetworkLayout->addWidget(m_static,2,0,1,1);
        NetworkLayout->addWidget(staticWidget,3,0,1,1);
        NetworkLayout->addWidget(m_RestarNet,4,1,1,1);

        setLayout(NetworkLayout);

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

    QComboBox * m_NetPort;
    QRadioButton * m_dhcp;
    QRadioButton * m_static;
    QWidget * staticWidget;
    QPushButton * m_RestarNet;
    QButtonGroup * m_portButtonGroup;

    TipLineEdit * m_ip;
    TipLineEdit * m_netmask;
    TipLineEdit * m_gateway;

    NetworkType m_eth0;
    NetworkType m_eth1;

    QGridLayout * NetworkLayout;
    QVBoxLayout * ConfigLayout;
};
