#ifndef JSONTEXTEDIT_H
#define JSONTEXTEDIT_H

#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>

class JsonTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    JsonTextEdit();
    JsonTextEdit(const QJsonDocument& doc);

    void jsonLoad(const QJsonDocument& doc);

public slots:
    void jsonSave  ();
    void jsonReload();

signals:
    void jsonChanged(const QJsonDocument& doc);

private:

    QJsonDocument json_doc_;
};

#endif // JSONTEXTEDIT_H
