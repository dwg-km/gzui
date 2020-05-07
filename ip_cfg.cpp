#include "ip_cfg.h"
#include "cali.h"
#include "motion.h"

#include <QMessageBox>



bool NetworkWidget::getNetworkConfig()
{
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open the file!" << endl;
        return false;
    }

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString Strline(line);
        if(Strline.contains("iface eth0 inet dhcp",Qt::CaseInsensitive))
        {
            m_eth0.IsStatic = false;
            m_LineIp->setText(QString::fromStdString(getLocalIp("eth0")));
        }
        else if (Strline.contains("iface eth0 inet static",Qt::CaseInsensitive))
        {
            m_eth0.IsStatic = true;
            QByteArray AddLine = file.readLine();
            QString SaddLine = QString(AddLine);
            while (!SaddLine.contains("address",Qt::CaseInsensitive)) {
                if(file.atEnd()){
                    qDebug() << "error:Don't find Address!";
                    return false;
                }
                AddLine = file.readLine();
                SaddLine = QString(AddLine);
            }
            m_eth0.Address = AddLine.mid(8);
            QByteArray NetLine = file.readLine();
            QString SnetLine = QString(NetLine);
            while (!SnetLine.contains("netmask",Qt::CaseSensitive)) {
                if(file.atEnd()){
                    qDebug() << "error:Don't find Netmask!";
                    return false;
                }
                NetLine = file.readLine();
                SnetLine = QString(NetLine);
            }
            m_eth0.NetMask = NetLine.mid(8);
            QByteArray GateLine = file.readLine();
            QString SgateLine = QString(GateLine);
            while (!SgateLine.contains("gateway",Qt::CaseSensitive)) {
                if(file.atEnd()){
                    qDebug() << "error:Don't find Gateway!";
                    return false;
                }
                GateLine = file.readLine();
                SgateLine = QString(GateLine);
            }
            m_eth0.GateWay = GateLine.mid(8);
        }
        else if (Strline.contains("iface eth1 inet dhcp",Qt::CaseInsensitive))
        {
            m_eth1.IsStatic = false;
            m_LineIp->setText(QString::fromStdString(getLocalIp("eth1")));
        }
        else if (Strline.contains("iface eth1 inet static",Qt::CaseInsensitive))
        {
            m_eth1.IsStatic = true;
            QByteArray AddLine = file.readLine();
            QString SaddLine = QString(AddLine);
            while (!SaddLine.contains("address",Qt::CaseInsensitive)) {
                if(file.atEnd()){
                    qDebug() << "error:Don't find Address!";
                    return false;
                }
                AddLine = file.readLine();
                SaddLine = QString(AddLine);
            }
            m_eth1.Address = AddLine.mid(8);
            QByteArray NetLine = file.readLine();
            QString SnetLine = QString(NetLine);
            while (!SnetLine.contains("netmask",Qt::CaseSensitive)) {
                if(file.atEnd()){
                    qDebug() << "error:Don't find Netmask!";
                    return false;
                }
                NetLine = file.readLine();
                SnetLine = QString(NetLine);
            }
            m_eth1.NetMask = NetLine.mid(8);
            QByteArray GateLine = file.readLine();
            QString SgateLine = QString(GateLine);
            while (!SgateLine.contains("gateway",Qt::CaseSensitive)) {
                if(file.atEnd()){
                    qDebug() << "error:Don't find Gateway!";
                    return false;
                }
                GateLine = file.readLine();
                SgateLine = QString(GateLine);
            }
            m_eth1.GateWay = GateLine.mid(8);
        }
        else
        {
            continue;
        }

    }
    return true;
}

string NetworkWidget::getLocalIp(QString Gport)
{
    int inet_sock;
    struct ifreq ifr;
    char ip[32] = {};

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, Gport.toStdString().c_str());
    ioctl(inet_sock, SIOCGIFADDR, &ifr);
    strcpy(ip,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    return string(ip);
}

void NetworkWidget::TabOpen()
{
    getNetworkConfig();
    if(m_eth0.IsStatic)
    {
        m_static->setChecked(true);
        setButtonDisable(false);

        m_LineIp->setText(m_eth0.Address);
        m_LineNetmask->setText(m_eth0.NetMask);
        m_LineGateway->setText(m_eth0.GateWay);
    }
    else
    {
        m_dhcp->setChecked(true);
        setButtonDisable(true);
        m_LineIp->setText(QString::fromStdString(getLocalIp("eth0")));
    }

    m_NetPort->setCurrentIndex(0);

}

void NetworkWidget::setButtonDisable(bool IsDisabel)
{
    m_LineIp->setDisabled(IsDisabel);
    m_LineNetmask->setDisabled(IsDisabel);
    m_LineGateway->setDisabled(IsDisabel);
}

void  NetworkWidget::SetPortState(int change)
{
   if(change == -2)
   {
       setButtonDisable(true);
   }
   else if(change == -3)
   {
       setButtonDisable(false);
   }
   else
   {
       qDebug() << "Have an error!";
       return;
   }

}


void NetworkWidget::NetPortChange(int PortIndex)
{
    if(PortIndex == 0)
    {
        //qDebug() << "in eth0";

        m_LineIp->setText(m_eth0.Address);
        m_LineNetmask->setText(m_eth0.NetMask);
        m_LineGateway->setText(m_eth0.GateWay);
        if(m_eth0.IsStatic == false)
        {
            setButtonDisable(true);
            m_dhcp->setChecked(true);
            m_LineIp->setText(QString::fromStdString(getLocalIp("eth0")));
        }
        else
        {
            setButtonDisable(false);
            m_static->setChecked(true);
        }
    }
    else
    {
        //qDebug() << "in eth1";

        m_LineIp->setText(m_eth1.Address);
        m_LineNetmask->setText(m_eth1.NetMask);
        m_LineGateway->setText(m_eth1.GateWay);
        if(m_eth1.IsStatic == false)
        {
            setButtonDisable(true);
            m_dhcp->setChecked(true);
            m_LineIp->setText(QString::fromStdString(getLocalIp("eth1")));
        }
        else
        {
            setButtonDisable(false);
            m_static->setChecked(true);
        }
    }
}

void NetworkWidget::SaveNetWorkConfig(QString port)
{
    if(m_static->isChecked())
    {
        if(port == QString::fromLocal8Bit("eth0"))
        {
             m_eth0.IsStatic = true;
             m_eth0.Address.clear();
             m_eth0.Address = m_LineIp->text();
             m_eth0.NetMask.clear();
             m_eth0.NetMask = m_LineNetmask->text();
             m_eth0.GateWay.clear();
            m_eth0.GateWay = m_LineGateway->text();
        }
         else
         {
            m_eth1.IsStatic = true;
            m_eth1.Address.clear();
            m_eth1.Address = m_LineIp->text();
            m_eth1.NetMask.clear();
            m_eth1.NetMask = m_LineNetmask->text();
            m_eth1.GateWay.clear();
            m_eth1.GateWay = m_LineGateway->text();
        }
    }
    else
    {
        if(port == QString::fromLocal8Bit("eth0"))
        {
            m_eth0.IsStatic = false;
            m_eth0.Address.clear();
            m_LineIp->setText("");
            m_eth0.NetMask.clear();
            m_LineNetmask->setText("");
            m_eth0.GateWay.clear();
            m_LineGateway->setText("");
            m_LineIp->setText(QString::fromStdString(getLocalIp("eth0")));
        }
        else
        {
           m_eth1.IsStatic = false;
           m_eth1.Address.clear();
           m_LineIp->setText("");
           m_eth1.NetMask.clear();
           m_LineNetmask->setText("");
           m_eth1.GateWay.clear();
           m_LineGateway->setText("");
           m_LineIp->setText(QString::fromStdString(getLocalIp("eth1")));
        }
        setButtonDisable(true);
    }
}

void NetworkWidget::RestartNetwork()
{
    int r = QMessageBox::warning(NULL,
        tr("Update"), "Are you sure to restart network?", QMessageBox::Cancel | QMessageBox::Yes);
    if (r == QMessageBox::Yes){



   SaveNetWorkConfig(m_NetPort->currentText());
   QString list = "auto eth0\nauto eth1\n";

   if(m_eth0.IsStatic)
   {
       list = list
               + "iface eth0 inet static\n"
               + "address " + m_eth0.Address + "\n"
               + "netmask " + m_eth0.NetMask + "\n"
               + "gateway " + m_eth0.GateWay + "\n";
   }
   else
   {
       list = list + "iface eth0 inet dhcp\n";
   }

   if(m_eth1.IsStatic)
   {
       list = list
               + "iface eth1 inet static\n"
               + "address " + m_eth1.Address + "\n"
               + "netmask " + m_eth1.NetMask + "\n"
               + "gateway " + m_eth1.GateWay + "\n"
               + "auto lo\niface lo inet loopback";
   }
   else
   {
       list = list + "iface eth1 inet dhcp\n"
       + "auto lo\niface lo inet loopback";
   }

   QString rmFile = "rm " + m_path;
   system(rmFile.toStdString().c_str());
   QString touchFile = "touch " + m_path;
   system(touchFile.toStdString().c_str());


   QFile file(m_path);
   file.open(QIODevice::WriteOnly | QIODevice::Text);
   file.write(list.toUtf8());
   file.close();
   //system("/etc/init.d/networking restart");
   /*
   system("ifdown eth0 && ifup eth0");
   system("ifdown eth1 && ifup eth1");
   */
   /*
   system("ifconfig eth0 down");
   system("ifconfig eth1 down");
   system("ifconfig eth0 up");
   system("ifconfig eth1 up");
   */

   system("reboot");
   qDebug() << "Creat Success!";

   getNetworkConfig();

    }
}

LineEditGroup::LineEditGroup(QString name, int x, int y, QStringList * color, QWidget *parent)
    : QGroupBox(name),
    colnum(x),
    rownum(y),
    Value(x * y, -1000)
{
    QGridLayout * horLayout = new QGridLayout;

    printButton = new QPushButton(this);
    printButton->setText(QObject::tr("PRINT"));
    //printButton->resize(72, 28);
    //printButton->setStyleSheet("background-color: rgb(9, 148, 220)");
    horLayout->addWidget(printButton, 0, 0, 1, 1);

    for(int j = 0; j < rownum; j++){
        QLabel *label = new QLabel(this);
        label->setText(QString::number(j));
        label->setAlignment(Qt::AlignCenter);
        horLayout->addWidget(label, j + 1, 0);
    }

    for(int i = 0; i < colnum; i++){
        QLabel *label = new QLabel(this);
        //label->setFixedHeight(40);
        if(color){
            label->setText(color->at(i));
        }else{
            label->setText(QString::number(i));
        }

        label->setAlignment(Qt::AlignCenter);
        horLayout->addWidget(label, 0, i + 2, 1, 1);
    }

    for(int j = 0; j < rownum; j++){
        for(int i = 0; i < colnum; i++){
            IntLineEdit  *lineEdit = new IntLineEdit(this);
            //lineEdit->resize(40, 28);
            horLayout->addWidget(lineEdit, j + 1, i + 2);
            matrix.push_back(lineEdit);
        }
    }

    setLayout(horLayout);
}

MoterParamGroupBox::MoterParamGroupBox(QString name, QWidget *parent) :
    QGroupBox(name, parent)
{
    QLabel * highLabel = new QLabel(QObject::tr("High Speed"));
    QLabel * midLabel = new QLabel(QObject::tr("Medium Speed"));
    QLabel * lowLabel = new QLabel(QObject::tr("Low Speed"));

    QLabel * speedLabel = new QLabel(QObject::tr("Speed(mm/s)"));
    QLabel * accLabel = new QLabel(QObject::tr("Acc Distance(mm)"));

    highspeedLineEdit = new IntLineEdit;
    highaccLineEdit = new IntLineEdit;
    midspeedLineEdit = new IntLineEdit;
    midaccLineEdit = new IntLineEdit;
    lowspeedLineEdit = new IntLineEdit;
    lowaccLineEdit = new IntLineEdit;

    QGridLayout * Layout = new QGridLayout;

    int y = 0;
    Layout->addWidget(speedLabel, 		y, 1);
    Layout->addWidget(accLabel,	 	y, 2);

    y++;
    Layout->addWidget(highLabel, 		y, 0);
    Layout->addWidget(highspeedLineEdit, 	y, 1);
    Layout->addWidget(highaccLineEdit, 	y, 2);

    y++;
    Layout->addWidget(midLabel, 		y, 0);
    Layout->addWidget(midspeedLineEdit, 	y, 1);
    Layout->addWidget(midaccLineEdit, 	y, 2);

    y++;
    Layout->addWidget(lowLabel, 		y, 0);
    Layout->addWidget(lowspeedLineEdit, 	y, 1);
    Layout->addWidget(lowaccLineEdit, 	y, 2);

    //Layout->addWidget(accLabel, 		3, 0);
    //Layout->addWidget(accLineEdit, 		3, 1);
    //Layout->addWidget(subdivLabel, 		4, 0);
    //Layout->addWidget(subdivLineEdit, 	4, 1);
    setLayout(Layout);
}
