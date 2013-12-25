#include "infodialog.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QAction>

InfoDialog::InfoDialog(QWidget *parent) : QDialog(parent),
headerLabel("Header:",this),
bodyLabel("Body:",this),
footerLabel("Footer:",this),
typeLabel("type:",this),
xLabel("X:",this),
yLabel("Y:",this),
hLabel("Height:",this),
wLabel("Width:",this),
headerEdit(this),
bodyEdit(this),
footerEdit(this),
typeEdit(this),
xEdit(this),
yEdit(this),
hEdit(this),
wEdit(this),
headerCompleter(this), bodyCompleter(this), footerCompleter(this),
save(QString("Save"),this)
{	
	setWindowFlags((windowFlags() | Qt::WindowStaysOnTopHint) & ~Qt::WindowContextHelpButtonHint);
	setModal(false);
	setMaximumSize(300, 550);
	setWindowTitle("Item Info");
	headerLabel.setBuddy(&headerEdit);
	bodyLabel.setBuddy(&bodyEdit);
	footerLabel.setBuddy(&footerEdit);
	typeLabel.setBuddy(&typeEdit);
	xLabel.setBuddy(&xEdit);
	yLabel.setBuddy(&yEdit);
	hLabel.setBuddy(&hEdit);
	wLabel.setBuddy(&wEdit);
	typeEdit.setReadOnly(true);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(&typeLabel);
	layout->addWidget(&typeEdit);
	layout->addWidget(&headerLabel);
	layout->addWidget(&headerEdit);
	layout->addWidget(&bodyLabel);
	layout->addWidget(&bodyEdit);
	layout->addWidget(&footerLabel);
	layout->addWidget(&footerEdit);
	layout->addWidget(&xLabel);
	layout->addWidget(&xEdit);
	layout->addWidget(&yLabel);
	layout->addWidget(&yEdit);
	layout->addWidget(&hLabel);
	layout->addWidget(&hEdit);
	layout->addWidget(&wLabel);
	layout->addWidget(&wEdit);
	layout->addWidget(&save);
	this->setLayout(layout);

	headerCompleter.setModel(&headers);
	//headerCompleter.setModelSorting(Qt::Ascending);
	headerCompleter.setCaseSensitivity(Qt::CaseInsensitive);
	
	bodyCompleter.setModel(&bodies);
	bodyCompleter.setCaseSensitivity(Qt::CaseInsensitive);
	
	footerCompleter.setModel(&footers);
	//footerCompleter.setModelSorting(Qt::Ascending);
	footerCompleter.setCaseSensitivity(Qt::CaseInsensitive);

	headerEdit.setCompleter(&headerCompleter);
	bodyEdit.setCompleter(&bodyCompleter);
	footerEdit.setCompleter(&footerCompleter);

	saveJSONDataToItem = new QAction(tr("Save Object Info"), this);
	saveJSONDataToItem->setShortcut(tr("Ctrl+M"));
	connect(saveJSONDataToItem, SIGNAL(triggered()),
		this, SLOT(saveDataForSelectedItem()));

	connect(&save, SIGNAL(clicked()),
		this, SLOT(saveDataForSelectedItem()));

	connect(&headerEdit, SIGNAL(returnPressed()),
		this, SLOT(loadBodyForHeader()));
}

void InfoDialog::setInfo(DiagramItem *item){
	this->item = item;
	reloadInfo();
}

void InfoDialog::reloadInfo(){
	typeEdit.setText(QString::number(item->iosDiagramType));
	headerEdit.setText(item->header);
	bodyEdit.setText(item->body);
	footerEdit.setText(item->footer);
	QRectF rect =item->mapRectToScene(item->boundingRect());
	xEdit.setText(QString::number((int)rect.x()));
	yEdit.setText(QString::number((int)rect.y()));
	hEdit.setText(QString::number((int)rect.height()));
	wEdit.setText(QString::number((int)rect.width()));
}

void InfoDialog::saveDataForSelectedItem(){
	if(item==NULL)
		return;
	//do smth
	item->header = headerEdit.text();
	item->body = bodyEdit.text();
	item->footer = footerEdit.text();
	item->setSize(QSize(wEdit.text().toInt(),hEdit.text().toInt()));
	item->setX(xEdit.text().toInt());
	item->setY(yEdit.text().toInt());

	QSize sz(item->boundingRect().width(),item->boundingRect().height());
	int tp = typeEdit.text().toInt();
	if(DiagramScene::getDefaultSize(tp)!=sz){
		DiagramScene::setDefaultSize(tp, sz);
	}
}

void InfoDialog::loadBodyForHeader(){
	QString header(headerEdit.text()), body;
	body = db[header];
	if(!body.isEmpty()){
		bodyEdit.setText(body);
	}
}