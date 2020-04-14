#include <QProcess>
#include <QApplication>
#include <QMessageBox>

#include "menu.h"




void UiSetting::Backup()
{
	system("./backup.sh");
}
void UiSetting::Update()
{
	QFileDialog *fileDialog = new QFileDialog();
	fileDialog->setWindowTitle("选择文件");
	fileDialog->setDirectory(".");
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	
	QDir dir("/media");
	dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	QFileInfoList list = dir.entryInfoList();
	if(list.size() == 0){
		fileDialog->setDirectory("/home");
	}else if(list.size() == 1){
		QString path = list.at(0).absoluteFilePath();
		qDebug() << "file=" << path;
		fileDialog->setDirectory(path);
	}else{
		fileDialog->setDirectory("/media");
	}

	QString filename;
	if(fileDialog->exec()){
		filename = fileDialog->selectedFiles()[0];
		qDebug() << "selected file is" << filename;
	
		QString cmd = "./update.sh ";
		cmd += filename;
		qDebug() << cmd;
		std::string str = cmd.toStdString();
		system(str.c_str());

		int r = QMessageBox::warning(NULL, 
			tr("Update"), "Update finished. click yes to restart", QMessageBox::Cancel | QMessageBox::Yes);
		if (r == QMessageBox::Yes){
			DestroyPrinter(0);
			qApp->exit(773);
		}
	}
}



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
            m_ip->setLine(QString::fromStdString(getLocalIp("eth0")));
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
            m_ip->setLine(QString::fromStdString(getLocalIp("eth1")));
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

        m_ip->setLine(m_eth0.Address);
        m_netmask->setLine(m_eth0.NetMask);
        m_gateway->setLine(m_eth0.GateWay);
    }
    else
    {
        m_dhcp->setChecked(true);
        setButtonDisable(true);
        m_ip->setLine(QString::fromStdString(getLocalIp("eth0")));
    }


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

void NetworkWidget::setButtonDisable(bool IsDisabel)
{
    m_ip->setLineDisabel(IsDisabel);
    m_netmask->setLineDisabel(IsDisabel);
    m_gateway->setLineDisabel(IsDisabel);
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
        qDebug() << "in eth0";

        m_ip->setLine(m_eth0.Address);
        m_netmask->setLine(m_eth0.NetMask);
        m_gateway->setLine(m_eth0.GateWay);
        if(m_eth0.IsStatic == false)
        {
            setButtonDisable(true);
            m_dhcp->setChecked(true);
            m_ip->setLine(QString::fromStdString(getLocalIp("eth0")));
        }
        else
        {
            setButtonDisable(false);
            m_static->setChecked(true);
        }
    }
    else
    {
        qDebug() << "in eth1";

        m_ip->setLine(m_eth1.Address);
        m_netmask->setLine(m_eth1.NetMask);
        m_gateway->setLine(m_eth1.GateWay);
        if(m_eth1.IsStatic == false)
        {
            setButtonDisable(true);
            m_dhcp->setChecked(true);
            m_ip->setLine(QString::fromStdString(getLocalIp("eth1")));
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
             m_eth0.Address = m_ip->getLine();
             m_eth0.NetMask.clear();
             m_eth0.NetMask = m_netmask->getLine();
             m_eth0.GateWay.clear();
             m_eth0.GateWay = m_gateway->getLine();
         }
         else
         {
            m_eth1.IsStatic = true;
            m_eth1.Address.clear();
            m_eth1.Address = m_ip->getLine();
            m_eth1.NetMask.clear();
            m_eth1.NetMask = m_netmask->getLine();
            m_eth1.GateWay.clear();
            m_eth1.GateWay = m_gateway->getLine();
        }
    }
    else
    {
        if(port == QString::fromLocal8Bit("eth0"))
        {
            m_eth0.IsStatic = false;
            m_eth0.Address.clear();
            m_ip->setLine("");
            m_eth0.NetMask.clear();
            m_netmask->setLine("");
            m_eth0.GateWay.clear();
            m_gateway->setLine("");
            m_ip->setLine(QString::fromStdString("eth0"));
        }
        else
        {
           m_eth1.IsStatic = false;
           m_eth1.Address.clear();
           m_ip->setLine("");
           m_eth1.NetMask.clear();
           m_netmask->setLine("");
           m_eth1.GateWay.clear();
           m_gateway->setLine("");
           m_ip->setLine(QString::fromStdString("eth1"));
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
   system("/etc/init.d/networking restart");
   qDebug() << "Creat Success!";

   getNetworkConfig();

    }
}
