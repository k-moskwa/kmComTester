/** @file
* @brief Main application routines.
* kmComTester.cpp
*
*  Created on: Feb 28, 2020
*      Author: Krzysztof Moskwa
*      License: GPL-3.0-or-later
*
*  kmComTester application
*  Copyright (C) 2020  Krzysztof Moskwa
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "KMComTester.h"
#include "ui_KMComTester.h"

#include <QTimer>
#include <QSettings>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define CMD_A 'a'
#define CMD_B 'b'
#define CMD_C 'c'
#define CMD_D 'd'
#define CMD_E 'e'
#define CMD_F 'f'
#define CMD_G 'g'
#define CMD_H 'h'
#define CMD_I 'i'
#define CMD_J 'j'
#define CMD_K 'k'
#define CMD_L 'l'
#define CMD_R 'r'
#define CMD_U 'u'
#define CMD_T 't'
#define CMD_X 'x'
#define SEQ_1 '1'
#define SEQ_2 '2'
#define SEQ_3 '3'
#define SEQ_4 '4'
#define SEQ_5 '5'
#define SEQ_6 '6'
#define SEQ_7 '7'
#define SEQ_8 '8'
#define SEQ_9 '9'

#define KM_COM_BUFFER_LEN 100
#define KM_APP_NAME "kmComTester"
#define KM_APP_MAJOR 1
#define KM_APP_MINOR 0
#define KM_APP_PATCH 0
#define KM_APP_BUILD 10
#define KM_APP_VERSION KM_APP_MAJOR "." KM_APP_MINOR "." KM_APP_PATCH "." KM_APP_BUILD
#define KM_APP_LICENSE "GNU General Public License v3.0"
#define KM_APP_SETTINGS_INI_FILE_NAME "./" KM_APP_NAME ".ini"

kmComTester::kmComTester(QWidget *parent)
	: QMainWindow(parent)
	, m_sequencePos(0)
	, m_serialPortName("COM14")
	, m_serialBaudRate(QSerialPort::Baud115200)
	, m_serialDataBits(QSerialPort::Data8)
	, m_serialParity(QSerialPort::NoParity)
	, m_serialStopBits(QSerialPort::OneStop)
	, m_serialFlowControl(QSerialPort::NoFlowControl)
	, m_serialAutoConnect(false)
	, m_ui(new Ui::kmComTester)
{
	m_ui->setupUi(this);

	rescanComPorts();
	slotsConnect();
	createCommands();
	loadSettings();
}

kmComTester::~kmComTester()
{
	delete m_ui;
}

void kmComTester::rescanComPorts(void) {
	// get list of serial ports available in the system
	QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
	// traverse the retrieved list by simple operator
	QListIterator<QSerialPortInfo> i(serialList);
	while (i.hasNext()) {
		QSerialPortInfo portInfo = i.next();
		// get port names from the SerialPortInfo object
		QString portInfoStr = portInfo.portName();
		// add the names to related combo box
		m_ui->comboSerialPortList->addItem(portInfoStr);
	}
	// using standard QComboBox model - sort the list
	// please not this standard model has limited functionalities
	// so e.g. COM10 will be before COM2 after this sort operation
	m_ui->comboSerialPortList->model()->sort(0);
}

void kmComTester::slotsConnect(void) {
	connect(m_ui->btnConnect, SIGNAL(clicked(void)), this, SLOT(slotBtnConnect(void)));
	connect(m_ui->btnDisconnect, SIGNAL(clicked(void)), this, SLOT(slotBtnDisconnect(void)));
	connect(m_ui->btnInfo, SIGNAL(clicked(void)), this, SLOT(slotBtnInfo(void)));
	connect(m_ui->btnHide, SIGNAL(clicked(void)), this, SLOT(slotBtnHide(void)));
	connect(m_ui->btnExit, SIGNAL(clicked(void)), this, SLOT(slotBtnExit(void)));
	connect(m_ui->btnU, SIGNAL(clicked(void)), this, SLOT(slotBtnUp(void)));
	connect(m_ui->btnD, SIGNAL(clicked(void)), this, SLOT(slotBtnDown(void)));
	connect(m_ui->btnL, SIGNAL(clicked(void)), this, SLOT(slotBtnLeft(void)));
	connect(m_ui->btnR, SIGNAL(clicked(void)), this, SLOT(slotBtnRight(void)));
	connect(m_ui->btnReset, SIGNAL(clicked(void)), this, SLOT(slotBtnReset(void)));
	connect(m_ui->btnCmd1, SIGNAL(clicked(void)), this, SLOT(slotBtnCmd1(void)));
	connect(m_ui->btnCmd2, SIGNAL(clicked(void)), this, SLOT(slotBtnCmd2(void)));
	connect(m_ui->btnCmd3, SIGNAL(clicked(void)), this, SLOT(slotBtnCmd3(void)));
	connect(m_ui->btnCmd4, SIGNAL(clicked(void)), this, SLOT(slotBtnCmd4(void)));

	connect(m_ui->actionConnect, SIGNAL(triggered(void)), this, SLOT(slotBtnConnect(void)));
	connect(m_ui->actionDisconnect, SIGNAL(triggered(void)), this, SLOT(slotBtnDisconnect(void)));
	connect(m_ui->aCmdI, SIGNAL(triggered(void)), this, SLOT(slotBtnInfo(void)));
	connect(m_ui->aCmdT, SIGNAL(triggered(void)), this, SLOT(slotBtnExit(void)));
	connect(m_ui->aCmdU, SIGNAL(triggered(void)), this, SLOT(slotBtnUp(void)));
	connect(m_ui->aCmdD, SIGNAL(triggered(void)), this, SLOT(slotBtnDown(void)));
	connect(m_ui->aCmdL, SIGNAL(triggered(void)), this, SLOT(slotBtnLeft(void)));
	connect(m_ui->aCmdH, SIGNAL(triggered(void)), this, SLOT(slotBtnHide(void)));

	connect(m_ui->aCmdR, SIGNAL(triggered(void)), this, SLOT(slotBtnRight(void)));
	connect(m_ui->aCmd1, SIGNAL(triggered(void)), this, SLOT(slotBtnCmd1(void)));
	connect(m_ui->aCmd2, SIGNAL(triggered(void)), this, SLOT(slotBtnCmd2(void)));
	connect(m_ui->aCmd3, SIGNAL(triggered(void)), this, SLOT(slotBtnCmd3(void)));
	connect(m_ui->aCmd4, SIGNAL(triggered(void)), this, SLOT(slotBtnCmd4(void)));
	connect(m_ui->aCmdX, SIGNAL(triggered(void)), this, SLOT(slotBtnReset(void)));

	connect(m_ui->aAbout, SIGNAL(triggered(void)), this, SLOT(slotAbout(void)));
	connect(m_ui->actionExit, SIGNAL(triggered(void)), this, SLOT(slotActionExit(void)));
	m_ui->actionExit->setShortcut(tr("Alt+F4")); // fake shortcut that cannot be set by QtDesigner

	connect(&m_serialPort, SIGNAL(readyRead(void)), this, SLOT(slotReadData(void)));
}

void kmComTester::slotAbout(void) {
	QMessageBox::information(this, KM_APP_NAME, QString(tr("%1 %2.%3.%4 build %5\n%6"))
		.arg(KM_APP_NAME)
		.arg(KM_APP_MAJOR)
		.arg(KM_APP_MINOR)
		.arg(KM_APP_PATCH)
		.arg(KM_APP_BUILD)
		.arg(KM_APP_LICENSE));
}

void kmComTester::slotBtnConnect(void) {
	m_serialPort.setPortName(m_ui->comboSerialPortList->currentText());
	m_serialPort.setBaudRate(QSerialPort::Baud115200);
	m_serialPort.setDataBits(QSerialPort::Data8);
	m_serialPort.setParity(QSerialPort::NoParity);
	m_serialPort.setStopBits(QSerialPort::OneStop);
	m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
	if (m_serialPort.open(QIODevice::ReadWrite)) {
		m_ui->btnConnect->setEnabled(false);
		m_ui->btnDisconnect->setEnabled(true);
		m_serialAutoConnect = true;
	} else {
		QMessageBox::critical(this, tr("Error"), m_serialPort.errorString());
	}
}

void kmComTester::slotBtnDisconnect(void) {
	if (m_serialPort.isOpen()) {
		m_serialPort.close();
		m_ui->btnConnect->setEnabled(true);
		m_ui->btnDisconnect->setEnabled(false);
		m_serialAutoConnect = false;
	}
}

void kmComTester::slotBtnUp(void) {
	cmd(getCommand(CMD_U));
}

void kmComTester::slotBtnDown(void) {
	cmd(getCommand(CMD_D));
}

void kmComTester::slotBtnLeft(void) {
	cmd(getCommand(CMD_L));
}

void kmComTester::slotBtnRight(void) {
	cmd(getCommand(CMD_R));
}

void kmComTester::slotBtnInfo(void) {
	cmd(getCommand(CMD_I));
}

void kmComTester::slotBtnHide(void) {
	cmd(getCommand(CMD_H));
}

void kmComTester::slotBtnExit(void) {
	cmd(getCommand(CMD_T));
}

void kmComTester::slotBtnReset(void) {
	cmd(getCommand(CMD_X));
}

void kmComTester::slotBtnCmd1(void) {
	processSequence(m_ui->leCmd1->text());
}

void kmComTester::slotBtnCmd2(void) {
	processSequence(m_ui->leCmd2->text());
}

void kmComTester::slotBtnCmd3(void) {
	processSequence(m_ui->leCmd3->text());
}

void kmComTester::slotBtnCmd4(void) {
	processSequence(m_ui->leCmd4->text());
}

void kmComTester::processSequence(QString complexCommand) {
	m_currentSequence = complexCommand;
	m_sequencePos = 0;
	QTimer::singleShot(0, this, SLOT(slotProcessSequenceTimer(void)));
}

void kmComTester::slotProcessSequenceTimer(void) {
	int delay = 0;
	int len = m_currentSequence.length();
	while (m_sequencePos < len) {
		QChar currentChar = m_currentSequence[m_sequencePos++];
		if (!currentChar.isDigit()) {
			cmd(getCommand(currentChar));
			QTimer::singleShot(0, this, SLOT(slotProcessSequenceTimer(void)));
			break;
		} else {
			delay *= 10;
			delay += currentChar.digitValue();
			if (m_sequencePos < len && !m_currentSequence[m_sequencePos].isDigit()) {
				QTimer::singleShot(delay, this, SLOT(slotProcessSequenceTimer(void)));
				m_ui->txtConsole->append(QString(tr("Delay:%1")).arg(delay));
				delay = 0;
				break;
			}
		}
	}
}

void kmComTester::createCommands(void) {
	m_commands[CMD_A] = m_ui->leCmdA;
	m_commands[CMD_B] = m_ui->leCmdB;
	m_commands[CMD_C] = m_ui->leCmdC;
	m_commands[CMD_D] = m_ui->leCmdD;
	m_commands[CMD_E] = m_ui->leCmdE;
	m_commands[CMD_F] = m_ui->leCmdF;
	m_commands[CMD_G] = m_ui->leCmdG;
	m_commands[CMD_H] = m_ui->leCmdH;
	m_commands[CMD_I] = m_ui->leCmdI;
	m_commands[CMD_J] = m_ui->leCmdJ;
	m_commands[CMD_K] = m_ui->leCmdK;
	m_commands[CMD_L] = m_ui->leCmdL;
	m_commands[CMD_R] = m_ui->leCmdR;
	m_commands[CMD_U] = m_ui->leCmdU;
	m_commands[CMD_T] = m_ui->leCmdT;
	m_commands[CMD_X] = m_ui->leCmdX;

	m_sequences[SEQ_1] = m_ui->leCmd1;
	m_sequences[SEQ_2] = m_ui->leCmd2;
	m_sequences[SEQ_3] = m_ui->leCmd3;
	m_sequences[SEQ_4] = m_ui->leCmd4;
}

QString kmComTester::getCommand(QChar cmdLetter) {
	cmdLetter = cmdLetter.toLower();
	if (m_commands.contains(cmdLetter)) {
		return m_commands[cmdLetter]->text();
	} // else {
	return tr("Unknown : ") + QString(cmdLetter);
}

void kmComTester::slotReadData(void) {
	while (!m_serialPort.atEnd()) {
		QByteArray data = m_serialPort.read(KM_COM_BUFFER_LEN);
		m_ui->txtConsole->append(data);
	}
}

void kmComTester::slotActionExit(void) {
	saveSettings();
	close();
}

void kmComTester::loadSettings(void) {
	QSettings settings(KM_APP_SETTINGS_INI_FILE_NAME, QSettings::IniFormat);

	settings.beginGroup("/MainWindow");
	resize(settings.value("/size", QSize(400, 400)).toSize());
	move(settings.value("/pos", QPoint(200, 200)).toPoint());
	settings.endGroup();
	settings.beginGroup("/Serial");
	QString portTmp = settings.value("/port", "COM1").toString();
	m_serialBaudRate = QSerialPort::BaudRate(settings.value("/baudRate").toInt());
	m_serialDataBits = QSerialPort::DataBits(settings.value("/dataBits").toInt());
	m_serialParity = QSerialPort::Parity(settings.value("/baudParity").toInt());
	m_serialStopBits = QSerialPort::StopBits(settings.value("/baudStopBits").toInt());
	m_serialFlowControl = QSerialPort::FlowControl(settings.value("/baudFlowControl").toInt());
	m_serialAutoConnect = settings.value("/autoConnect", false).toBool();
	int portIdx = m_ui->comboSerialPortList->findText(portTmp);
	if (portIdx >= 0) {
		m_ui->comboSerialPortList->setCurrentIndex(portIdx);
		if (true == m_serialAutoConnect) {
			slotBtnConnect();
		}
	}
	settings.endGroup();
	if (settings.childGroups().contains("Commands")) {
		settings.beginGroup("/Commands");
		QHash<QChar, QLineEdit *>::iterator it;
		for (it = m_commands.begin(); it != m_commands.end(); it++) {
			it.value()->setText(settings.value(QString("/Cmd") + it.key().toUpper(), "").toString());
		}
		settings.endGroup();
	}
	if (settings.childGroups().contains("Sequences")) {
		settings.beginGroup("/Sequences");
		QHash<QChar, QLineEdit *>::iterator itSeq;
		for (itSeq = m_sequences.begin(); itSeq != m_sequences.end(); itSeq++) {
			itSeq.value()->setText(settings.value(QString("/Seq") + itSeq.key().toUpper(), "").toString());
		}
		settings.endGroup();
	}
}

void kmComTester::saveSettings(void) {
	QSettings settings(KM_APP_SETTINGS_INI_FILE_NAME, QSettings::IniFormat);
	// Window geometry
	settings.beginGroup("/MainWindow");
	settings.setValue("/size", size());
	settings.setValue("/pos", pos());
	settings.endGroup();
	// Communication over serial port
	settings.beginGroup("/Serial");
	settings.setValue("/port", m_ui->comboSerialPortList->currentText());
	settings.setValue("/baudRate", m_serialBaudRate);
	settings.setValue("/dataBits", m_serialDataBits);
	settings.setValue("/baudParity", m_serialParity);
	settings.setValue("/baudStopBits", m_serialStopBits);
	settings.setValue("/baudFlowControl", m_serialFlowControl);
	settings.setValue("/autoConnect", m_serialAutoConnect);
	settings.endGroup();
	// Commands
	settings.beginGroup("/Commands");
	QHash<QChar, QLineEdit *>::iterator it;
	for (it = m_commands.begin(); it != m_commands.end(); it++) {
		settings.setValue(QString("/Cmd") + it.key().toUpper(), it.value()->text());
	}
	settings.endGroup();
	// Sequences
	settings.beginGroup("/Sequences");
	QHash<QChar, QLineEdit *>::iterator itSeq;
	for (itSeq = m_sequences.begin(); itSeq != m_sequences.end(); itSeq++) {
		settings.setValue(QString("/Seq") + itSeq.key().toUpper(), itSeq.value()->text());
	}
	settings.endGroup();
	settings.sync();
}

void kmComTester::cmd(QString cmd) {
	cmd += "\r\n";
	m_serialPort.write(cmd.toUtf8());
	m_ui->txtConsole->append(tr(">") + cmd);
}
