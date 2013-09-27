//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#ifndef WIKIEDIT_H
#define WIKIEDIT_H

#include <QString>
#include <QThread>
#include <QMutex>
#include <QtXml>
#include "apiquery.h"
#include "wikiuser.h"
#include "wikipage.h"

enum WarningLevel
{
    WarningLevelNone,
    WarningLevel1,
    WarningLevel2,
    WarningLevel3,
    WarningLevel4
};

enum WEStatus
{
    StatusNone,
    StatusProcessed,
    StatusPostProcessed
};

class WikiEdit;

class ProcessorThread :  public QThread
{
    Q_OBJECT
public:
    static QList<WikiEdit *> PendingEdits;
    static QMutex EditLock;
    void Process(WikiEdit *edit);
protected:
    void run();
};

class Query;
class ApiQuery;

class WikiEdit
{
public:
    WikiEdit();
    WikiEdit(const WikiEdit& edit);
    WikiEdit(WikiEdit *edit);
    ~WikiEdit();
    bool FinalizePostProcessing();
    void PostProcess();
    //! Return a full url to edit
    QString GetFullUrl();
    //! Return true in case this edit was post processed already
    bool IsPostProcessed();
    //! Page that was changed by edit
    WikiPage *Page;
    //! User who changed the page
    WikiUser *User;
    //! Edit is a minor edit
    bool Minor;
    //! Edit is a bot edit
    bool Bot;
    bool NewPage;
    int Size;
    int Diff;
    int Priority;
    int OldID;
    int RevID;
    WEStatus Status;
    WarningLevel CurrentUserWarningLevel;
    QString Summary;
    QString RollbackToken;
    QString DiffText;
    //! If this is true the edit was made by huggle
    bool EditMadeByHuggle;
    //! If this is true the edit was made by some other
    //! tool for vandalism reverting
    bool TrustworthEdit;
    bool Whitelisted;
    //! Edit was made by you
    bool OwnEdit;
    WikiEdit *Previous;
    WikiEdit *Next;
    int Score;
    QStringList ScoreWords;
    void ProcessWords();
    bool PostProcessing;
    bool ProcessingByWorkerThread;
    bool ProcessedByWorkerThread;
private:
    bool ProcessingRevs;
    bool ProcessingDiff;
    ApiQuery* ProcessingQuery;
    ApiQuery* DifferenceQuery;
};

#endif // WIKIEDIT_H
