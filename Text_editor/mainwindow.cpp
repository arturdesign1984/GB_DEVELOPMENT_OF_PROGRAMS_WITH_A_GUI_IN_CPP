#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QTranslator translator;
    if(translator.load(":/locales/Text_editor_ru_RU.qm"))
    {
        QCoreApplication::removeTranslator(&translator);
        QCoreApplication::installTranslator(&translator);
    }
    ui->setupUi(this);
    createConnactions();
    Shortcuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createConnactions()
{
    connect(ui->actionNew, &QAction::toggled, this, &MainWindow::on_actionNew_triggered);
    connect(ui->actionOpen, &QAction::toggled, this, &MainWindow::on_actionOpen_triggered);
    connect(ui->actionOpen_readonly, &QAction::toggled, this, &MainWindow::on_actionOpen_readonly_triggered);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::on_actionSave_as_triggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::on_actionSave_triggered);
    connect(ui->actionExit, &QAction::toggled, this, &MainWindow::on_actionExit_triggered);
    connect(ui->actionAbout, &QAction::toggled, this, &MainWindow::on_actionInfo_triggered);
    connect(ui->actionen_EN, &QAction::triggered, this, &MainWindow::change_lang_to_en);
    connect(ui->actionru_RU, &QAction::triggered, this, &MainWindow::change_lang_to_ru);

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F1)
    {
        on_actionInfo_triggered();
    }
}

void MainWindow::change_lang_to_en()
{
    if(translator.load(":/locales/Text_editor_en_EN.qm"))
    {
        QCoreApplication::removeTranslator(&translator);
        QCoreApplication::installTranslator(&translator);
    }
    ui->retranslateUi(this);
    Shortcuts();
}

void MainWindow::change_lang_to_ru()
{
    QTranslator translator;
    if(translator.load(":/locales/Text_editor_ru_RU.qm"))
    {
        QCoreApplication::removeTranslator(&translator);
        QCoreApplication::installTranslator(&translator);
    }
    ui->retranslateUi(this);
    Shortcuts();
}

void MainWindow::on_actionInfo_triggered()
{
    ui->textEdit->setReadOnly(true);

    file.setFileName(":/src/info.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        ui->textEdit->setText(file.readAll());
        file.close();
    } else
    {
        qDebug() << tr("File info.txt lost") << ".\n";
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save file"),
                                                    QDir::currentPath(),
                                                    "Text (*.txt);;All files (*.*)");

    file.setFileName(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        QString text = ui->textEdit->toPlainText();
        QByteArray ba = text.toUtf8();
        file.write(ba, ba.length());
        file.close();
    } else
    {
        qDebug() << tr("Error when saving") << ".\n";
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(!file.isOpen() || file.fileName() != "")
    {
        if(file.open(QIODevice::WriteOnly))
        {
            QString text = ui->textEdit->toPlainText();
            QByteArray ba = text.toUtf8();
            file.write(ba, ba.length());
            file.close();
        } else
        {
            on_actionSave_as_triggered();
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    ui->textEdit->setReadOnly(false);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"),
                                                    QDir::currentPath(),
                                                    "Text (*.txt);;All files (*.*)");

    file.setFileName(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        ui->textEdit->setText(file.readAll());
        file.close();
    } else
    {
        qDebug() << tr("Failed to open file ") << fileName << ".\n";
    }
}

void MainWindow::on_actionNew_triggered()
{
    ui->textEdit->setReadOnly(false);
    file.setFileName("");
    ui->textEdit->setText("");
}

void MainWindow::on_actionExit_triggered()
{
    if(ui->textEdit->toPlainText() != "")
    {
        on_actionSave_triggered();
    }
    this->close();
}

void MainWindow::on_actionOpen_readonly_triggered()
{
    ui->textEdit->setReadOnly(true);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"),
                                                    QDir::currentPath(),
                                                    "Text (*.txt);;All files (*.*)");

    file.setFileName(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        ui->textEdit->setText(file.readAll());
        file.close();
    } else
    {
        qDebug() << tr("Failed to open file ") << fileName << ".\n";
    }
}

void MainWindow::Shortcuts()
{
    shortcuts.clear();
    for(auto action : ui->menuFile->actions())
    {
        shortcuts.insert(action->text(), action);
    }
    for(auto action : ui->menuHelp->actions())
    {
        shortcuts.insert(action->text(), action);
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(shortcuts.keys());
}

void MainWindow::on_comboBox_activated(int index)
{
    ui->lineEdit->setText(shortcuts.value(ui->comboBox->itemText(index))->shortcut().toString());
}

void MainWindow::on_pushButton_clicked()
{
    auto index = ui->comboBox->currentIndex();

    if(index != 0)
    {
        shortcuts[ui->comboBox->itemText(index)]->setShortcut(QKeySequence(ui->lineEdit->text()));
    }
}

