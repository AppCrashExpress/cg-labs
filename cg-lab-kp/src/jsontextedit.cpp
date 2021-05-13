#include "jsontextedit.h"

JsonTextEdit::JsonTextEdit()
{
    setTabStopDistance(30);
}

JsonTextEdit::JsonTextEdit(const QJsonDocument& doc)
{
    jsonLoad(doc);
}

void JsonTextEdit::jsonSave()
{
    QJsonDocument temp_doc = QJsonDocument::fromJson(toPlainText().toUtf8());
    if (!temp_doc.isNull())
    {
        json_doc_.swap(temp_doc);
    }
    emit jsonChanged(json_doc_);
}

void JsonTextEdit::jsonLoad(const QJsonDocument& doc)
{
    json_doc_ = doc;
    jsonReload();
    emit jsonChanged(json_doc_);
}

void JsonTextEdit::jsonReload()
{
    setPlainText(json_doc_.toJson());
}
