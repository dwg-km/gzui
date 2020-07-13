
#include "touchkey.h"
#include <QMessageBox>
TouchKey* TouchKey::touchkey;

TouchKey::TouchKey() : 
	QDialog(),
	str("")
{
	label = NULL;

	m_edit = new QLineEdit(this);
	
	if (m_edit != NULL){
		m_edit->move(10, 10);
		m_edit->resize(240, 30);
		m_edit->setReadOnly(true);
	}
	Construct();

	killTimer = new QTimer(this);
	connect(killTimer, SIGNAL(timeout()), this,  SLOT(KillTouchkey()));
}

TouchKey* TouchKey::newInstance()
{
	if(touchkey == NULL){
		touchkey = new TouchKey;
	}
	
	return touchkey;
}

TouchKey::~TouchKey()
{
}

void TouchKey::setRet(QLineEdit * line){
	if(line){
		label = line;
		str = "";
		str = label->text();
		m_edit->setText(str);
		
		timer = 0;
		killTimer->start(1000);
	}
}

void TouchKey::add()
{
	timer = 0;
	if(label){
		if(str == "")
			str = "0";
	//	if(str != ""){
	//		label->setText(str);
	//	}
	//	else{
	//		label->setText("0");
	//	}
	//	label->clearFocus();
		//label->repaint();
	//	label = NULL;
	
		bool ret = 1;
        if(typefloat){
			float num = 0.0;
	 		num = str.toFloat(&ret);
            if(!ret && IsNotString){
				QMessageBox  box;
				box.setText("Please input a float number!");
				box.exec();
				str = "0";
				label->setText("0");
				m_edit->setText("0");
			}
			else{
                if(IsNotString){
                    QString data = QString::number(num,'g', 2);
			       // QString data = QString("%1").arg(num);
                    label->setText(data);
               			
                    label = NULL;
                    close();
                }
                else {
                    label->setText(str);
                    label = NULL;
                    close();
                }
			}
		}
		else{
			int num = 0;
        		num = str.toInt(&ret , 10);
		      
            if(!ret && IsNotString){
				QMessageBox  box;
				box.setText("Please input a int number!");
				box.exec();
				label->setText("0");
			}
			else{

				
			        QString data = QString("%1").arg(num);
		 		label->setText(data);
				label = NULL;
                 	 	close();
			}
		}
	}
}
	
	

/*
void TouchKey::show()
{
	QWidget::show();
	setFixedSize(width(), height());
}
*/
bool TouchKey::Construct()
{
	timer = 0;
	const char *buttontext[15] = {
		"7", "8", "9",
		"4", "5", "6",
		"1", "2", "3",
		"0", ".", "-",
		"C",  "OK",
	};
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_buttons[3 * i + j] = new QPushButton(this);
			if (m_buttons[3 * i + j] != NULL){
				m_buttons[3 * i + j]->move(10 + (10 + 40)*j, 50 + (10 + 40)*i);
				m_buttons[3 * i + j]->resize(40, 40);
				m_buttons[3 * i + j]->setText(buttontext[3 * i + j]);
				connect(m_buttons[3 * i + j], SIGNAL(clicked()), this, SLOT( onCalculate()));
			}
		}
	}
	m_buttons[11] = new QPushButton(this);
	m_buttons[11]->setText("C");
	m_buttons[11]->move(160,50);
	m_buttons[11]->resize(80,80);
	connect(m_buttons[11], SIGNAL(clicked()), this, SLOT( onCalculate()));
	
	m_buttons[12] = new QPushButton(this);
	m_buttons[12]->setText("OK");
	m_buttons[12]->move(160,160);
	m_buttons[12]->resize(80,80);
	connect(m_buttons[12], SIGNAL(clicked()), this, SLOT(add()));
	
	return 0;
}

void TouchKey::onCalculate()
{
	timer = 0;
	QPushButton* button = dynamic_cast<QPushButton*>(sender());//判断有哪一个按钮按下
	if (button != NULL){
		QString buttontext = button->text();
		//if (buttontext == "<-"){
		//	if (str.length() > 0){
		//		str.remove(str.length() - 1, 1);
		//	}
		//}
		//else 
		if(buttontext == "C"){
			str="";
			m_edit->setText("");
		}else if(buttontext == "."){
			//setPointEnable(false);
			//m_buttons[10]->setEnabled(false);
			str += buttontext;
		}else{
			str += buttontext;
		}
		
		m_edit->setText(str);
	}
}

