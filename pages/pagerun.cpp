#include "pagerun.h"

#include <QProcess>
#include <QStack>

PageRun::PageRun(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);
    treeWidgetDetectedFeatures->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    tabWidget->setCurrentIndex(0);
}

void PageRun::initializePage()
{
    configure();
}

bool PageRun::isComplete() const
{
    return false;
}

void PageRun::initProcess(QProcess *p)
{
    labelStatus->setText("<b>Running: </b>" + p->program());
    connect(p, &QProcess::readyReadStandardError, [this, p](){
        auto b = p->readAllStandardError();
        plainTextEditBuildOutput->appendPlainText(b);
    });
    connect(p, &QProcess::readyReadStandardOutput, [this, p](){
        auto b = p->readAllStandardOutput();
        plainTextEditBuildOutput->appendPlainText(b);
    });
}

void PageRun::configure()
{
    auto p = _config->createProcess();
    initProcess(p);
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this](int code, QProcess::ExitStatus status){
        if (status == QProcess::NormalExit) {
            plainTextEditBuildOutput->appendPlainText("Configure successful");
            metaObject()->invokeMethod(this, "make", Qt::QueuedConnection);
        }
        else
            plainTextEditBuildOutput->appendPlainText(QString("Configure failed with code: %1").arg(code));
    });
    p->start();
}

void PageRun::detectFeatures() {
    treeWidgetDetectedFeatures->clear();

    auto lines = plainTextEditBuildOutput->toPlainText().split("\n");
    bool found = false;
    QRegularExpression rSection("^(\\s*)([^:]+):$");
    QRegularExpression rWithVal("^(\\s+)([^.]+)\\s\\.+\\s(.*)$");
    QStack<QTreeWidgetItem*> itemsStack;
    int lastIndent = 0;
    QTreeWidgetItem *lastItem = nullptr;

    foreach (QString line, lines) {
        if (!found) {
            if (line == "Build options:")
                found = true;
        }
        if (!found)
            continue;

        QTreeWidgetItem *item = nullptr;
        int indent = -1;
        auto m = rSection.match(line);
        if (m.hasMatch()) {
            item = new QTreeWidgetItem;
            item->setText(0, m.captured(2));
            indent = m.captured(1).length() / 2;
        }

        m = rWithVal.match(line);
        if (m.hasMatch()) {
            item = new QTreeWidgetItem;
            item->setText(0, m.captured(2));
            item->setText(1, m.captured(3));
            indent = m.captured(1).length() / 2;

            if (m.captured(3) == "no")
                item->setData(1, Qt::DecorationRole, QColor(Qt::red));
        }

        if (item) {
            for (int i = 0; i < lastIndent - indent; i++)
                itemsStack.pop();

            if (indent > lastIndent)
                itemsStack.push(lastItem);

            if (itemsStack.size())
                itemsStack.top()->addChild(item);
            else
                treeWidgetDetectedFeatures->addTopLevelItem(item);


            lastIndent = indent;
            lastItem = item;
        }
    }
}
void PageRun::make()
{
    detectFeatures();
    auto p = new QProcess(this);
    initProcess(p);
    p->setWorkingDirectory(_config->buildPath());
    p->setProgram("gmake");
    p->start();
}
