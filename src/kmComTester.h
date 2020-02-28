/** @file
* @brief Main application routines.
* kmComTester.h
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

#ifndef KMCOMTESTER_H
#define KMCOMTESTER_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class kmComTester; }
QT_END_NAMESPACE

class kmComTester : public QMainWindow
{
	// Obligatory for objects inherited from QObject
	Q_OBJECT

public:
	/**
	Default constructor / destructor.
	@param parent parent widget
	*/
	kmComTester(QWidget *parent = nullptr);
	~kmComTester();

	/*
	Public slots are for internal purposes of UI component,
	mainly connecting UI widgets to the application
	*/
public slots:
	void slotBtnConnect(void);
	void slotBtnDisconnect(void);
	void slotReadData(void);
	void slotBtnUp(void);
	void slotBtnDown(void);
	void slotBtnLeft(void);
	void slotBtnRight(void);
	void slotBtnInfo(void);
	void slotBtnHide(void);
	void slotBtnExit(void);
	void slotBtnReset(void);

	void slotBtnCmd1(void);
	void slotBtnCmd2(void);
	void slotBtnCmd3(void);
	void slotBtnCmd4(void);
	void slotActionExit(void);
	void slotAbout(void);

	void slotProcessSequenceTimer(void);

private:
	// Send 'cmd' command over serial port.
	void cmd(QString cmd);
	// Connect all slots.
	void slotsConnect(void);
	// Load application settings from .ini file.
	void loadSettings(void);
	// Save application settings to .ini file.
	void saveSettings(void);
	// Create internal structures for used commands.
	void createCommands(void);
	// Checks available COM ports and shows them in UI (combo box)
	void rescanComPorts(void);
	/*
	Process sequence of commands where letter is a index of a command and number
	is delay in milliseconds to wait before next command is issued
	Examples: sequence like "ABCD" will issue A, then B, then C, then D commands defined in settings
	A500B500C500D will issue A command than wait 0.5s (500ms) than B command will be issued etc.
	*/
	void processSequence(QString complexCommand);

	/*
	Returns string of the Serial COM command by single specific letter (case insensitive)
	(e.g. command 'a' will be changed into 'AT+DK' if defined like that in the settings)
	*/
	QString getCommand(QChar cmdLetter);

	// Internal variables and structure
	QHash<QChar, QLineEdit *> m_commands;
	QHash<QChar, QLineEdit *> m_sequences;
	QString m_currentSequence;
	int m_sequencePos;
	QSerialPort m_serialPort;
	QString m_serialPortName;
	QSerialPort::BaudRate m_serialBaudRate;
	QSerialPort::DataBits m_serialDataBits;
	QSerialPort::Parity m_serialParity;
	QSerialPort::StopBits m_serialStopBits;
	QSerialPort::FlowControl m_serialFlowControl;
	bool m_serialAutoConnect;

	Ui::kmComTester *m_ui;
};
#endif // KMCOMTESTER_H
