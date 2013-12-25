#ifndef INFODIALOG_H_DEFINED
#define INFODIALOG_H_DEFINED

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QLabel>
#include <QCompleter>
#include <QStringListModel>
#include "diagramitem.h"

class InfoDialog : public QDialog {
	Q_OBJECT
	friend class MainWindow;
	QMap<QString,QString> db;
private:
	DiagramItem *item;
	QLineEdit headerEdit, bodyEdit, typeEdit, footerEdit, xEdit, yEdit, hEdit, wEdit;
	QLabel headerLabel, bodyLabel, footerLabel, typeLabel, xLabel, yLabel, hLabel, wLabel;
	QAction *saveJSONDataToItem;
	QPushButton save;
	QCompleter headerCompleter, bodyCompleter, footerCompleter;

private slots:
		void saveDataForSelectedItem();

public:
	QStringListModel headers, bodies, footers;
	InfoDialog(QWidget *parent = 0);
	Q_SLOT void setInfo(DiagramItem *item);
	Q_SLOT void reloadInfo();
	Q_SLOT void loadBodyForHeader();
};

#endif