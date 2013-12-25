#ifndef IOJSON_H_DEFINED
#define IOJSON_H_DEFINED

class IoJson {
public:
    virtual QJsonObject toJson() = 0;
    virtual void fromJson(QJsonObject&) = 0;
};

#endif
