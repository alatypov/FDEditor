#ifndef HELPER_H_DEFINED
#define HELPER_H_DEFINED

#include <QMap>
#include <QString>
#include <QDir>
#include <QList>
#include <QSet>
#include <vector>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QFileInfo>

#include <string>

class MainWindow;

class Helper {
	friend class MainWindow;
	friend class InfoDialog;

	QMap<QString,QString> db;
	QSet<QString> footers;
	QSet<QString> bodies;
	QSet<QString> headers;
	QMap<QString,QDateTime> helperFileInfos;
	QString dirPath;
	
public:

	void parseFile(QString fileName){
        /*
//        if(helperFileInfos.contains(fileName)){
//			lastRead=helperFileInfos[fileName];
//			fi=QFileInfo(fileName);
//			if(fi.lastModified()==lastRead){
//				return;
//			}else{
//					helperFileInfos[fileName]=fi.lastModified();
//			}
//		}else{
//			fi=QFileInfo(fileName);
//			helperFileInfos[fileName]=fi.lastModified();
//		}
		
        QJsonDocument reader;
        QFileInfo fi;
        QDateTime lastRead;
        QJsonParseError error;
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream inf(&file);
		QString content = inf.readAll();
        reader = QJsonDocument::fromJson(content.toUtf8().data(), &error);
        if( !reader.isNull() ) {
            QMessageBox::critical(NULL, QString("JSON Import Error"),
                QString("Failed to parse JSON file ''%1'', due to ''%2''.").arg(fileName).arg(error.errorString()));
			return;
		}

        if( !reader.isArray() ){
            QMessageBox::critical(NULL, QString("JSON Import Error"),
                QString("Document should contain an Array.").arg(fileName).arg(error.errorString()));
            return;
        }

        QJsonArray items = reader.array();
        QJsonValue item;
		int type, x, y, h, w;
		QString header, body, footer, tmp;
        for(int i=0; i<items.size(); i++){
			item = items[i];
            type = item.get["type"].toInt();
			Json::Value::Members m = item.getMemberNames();
			Json::Value::Members::iterator it;
			header.clear();
			body.clear();
			footer.clear();
			for(it=m.begin(); it!=m.end(); it++){
				tmp = QString::fromStdString((*it));
				if( tmp=="header" ) {
					header=QString::fromStdString(item.get("header","").asString());
				}	else if( tmp=="body" ) {
					body=QString::fromStdString(item.get("body","").asString());
				}	else if( tmp=="footer" ) {
					footer=QString::fromStdString(item.get("footer","").asString());
				}
			}
			if(!header.isEmpty() && !body.isEmpty()){
				db[header] = body;
				bodies << body;
			}
			if(!header.isEmpty()){
				headers << header;
			}
			if(!body.isEmpty()){
				bodies << body;
			}
			if(!footer.isEmpty()){
				footers << footer;
			}
		}
        */
	}

	void readDB(QString dirPath){
		QDir dir(dirPath);
		dir.setFilter(QDir::Files);
		QStringList nameFilters;
		nameFilters << "*.json";
		dir.setNameFilters(nameFilters);
		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			parseFile(fileInfo.absoluteFilePath());
		}
		headers=db.keys().toSet();
	}
};

#endif //HELPER_H_DEFINED
