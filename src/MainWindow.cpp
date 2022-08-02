#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "EntriesModel.h"

#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent},
      ui{new Ui::MainWindow},
      _file_dialog{nullptr},
      _entries_model{nullptr}
{
    ui->setupUi(this);

    _file_dialog = new QFileDialog(this);
    _file_dialog->setWindowTitle(tr("Укажите директорию"));
    _file_dialog->setFileMode(QFileDialog::ExistingFile);

    connect(ui->pushButtonFileDialog, &QAbstractButton::clicked,
                this, &MainWindow::pushButtonFileDialog_clicked
                );

    connect(ui->pushButtonOpenArchive, &QAbstractButton::clicked,
            this, &MainWindow::pushButtonOpenArchive_clicked
            );

    connect(_file_dialog, SIGNAL(urlSelected(QUrl)),
                this, SLOT(urlSelected(QUrl))
                );

    ui->pushButtonFileDialog->setIcon(QIcon(":/icons/Dir.png"));
    ui->pushButtonOpenArchive->setIcon(QIcon(":/icons/Confirm.png"));
    ui->pushButtonFileDialog->setIconSize(QSize(20, 30));
    ui->pushButtonOpenArchive->setIconSize(QSize(20, 30));

    resize(550, 330);
}
//=================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//=================================================================================================
void MainWindow::pushButtonOpenArchive_clicked()
{
    QString path = ui->lineEditPathToArchive->text();
    if (path.isEmpty())
    {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Введите путь до архива."));
        return;
    }
    if (!path.contains(".zip"))
    {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Файл должен иметь расширение .zip"));
        return;
    }

    EntriesManager entries_manager(path.toStdString());
    Entries_t entries;
    try
    {
        entries_manager.getAllEntries(entries);
    }
    catch (EntriesManagerException& ex)
    {
        switch (ex._error_type)
        {
        case EntriesManagerException::OpenFileError:
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr("Не удалось открыть файл."));
            return;

        case EntriesManagerException::NoEntriesError:
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr("Архив пуст."));
            return;

        // in this case filling the model anyway
        case EntriesManagerException::GetInfoError:
            std::string msg;
            msg.append("Не удалось открыть файл под номером ") + ex.what() + '.';
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr(msg.c_str()));
            break;

        }
    }


    if (_entries_model)
        delete _entries_model;
    _entries_model = new EntriesModel(entries);

    ui->tableView->setModel(_entries_model);
    ui->tableView->resizeColumnsToContents();

    QRect view_contents_rect = ui->tableView->contentsRect();
    resize(view_contents_rect.width()+175, height());
    setFixedWidth(width());
}
//=================================================================================================
void MainWindow::pushButtonFileDialog_clicked()
{
    _file_dialog->exec();
}
//=================================================================================================
void MainWindow::urlSelected(const QUrl& url)
{
    QString temp(url.path());
    //temp.remove(0, 1); // deleteing first '/'
    ui->lineEditPathToArchive->setText(temp);
}
