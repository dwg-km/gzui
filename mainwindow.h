#ifndef FINDFILEDIALOG_H
#define FINDFILEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

#include <QTableWidget>
#include <QDialog>

#include "iconbutton.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class FindFileDialog : public QDialog
{
    Q_OBJECT

public:
    FindFileDialog(QWidget *parent = 0);

private:
    QLabel *logoLabel;
    QLabel *namedLabel;
    QLabel *lookInLabel;
    QLineEdit *lookInLineEdit;
    QLineEdit *namedLineEdit;
    QCheckBox *subfoldersCheckBox;
    QTableWidget *tableWidget;
    QLabel *messageLabel;
    QPushButton *findButton;
    QPushButton *stopButton;
    QPushButton *closeButton;
    QPushButton *helpButton;

    iconButton * leftButton;
    iconButton * rightButton;
    iconButton * upButton;
    iconButton * downButton;
    iconButton * homeButton;
};

#endif
