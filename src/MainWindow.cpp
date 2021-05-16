#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "EntriesModel.h"

#include <QMessageBox>

#include <regex>

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
                this, &MainWindow::on_pushButtonFileDialog_clicked
                );

    connect(ui->pushButtonOpenArchive, &QAbstractButton::clicked,
            this, &MainWindow::on_pushButtonOpenArchive_clicked
            );

    connect(_file_dialog, &QFileDialog::urlSelected,
                this, &MainWindow::urlSelected
                );

    ui->pushButtonFileDialog->setIcon(QIcon("../src/icons/Dir.png"));
    ui->pushButtonOpenArchive->setIcon(QIcon("../src/icons/Confirm.png"));
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
void MainWindow::on_pushButtonOpenArchive_clicked()
{
    // проверяем путь до файла, передаём в конструктор
    QString path = ui->lineEditPathToArchive->text();
    if (path.isEmpty())
    {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Укажите для сохранения результатов вычислений."));
        return;
    }
    //std::regex re("\\.zip$");
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


    // заполняем модель
    if (_entries_model)
        delete _entries_model;
    _entries_model = new EntriesModel(entries);

    // вставляем модель в представление
    ui->tableView->setModel(_entries_model);
    ui->tableView->resizeColumnsToContents();

    // опционально: делаем делегат
}
//=================================================================================================
void MainWindow::on_pushButtonFileDialog_clicked()
{
    _file_dialog->exec();
}
//=================================================================================================
void MainWindow::urlSelected(const QUrl& url)
{
    QString temp(url.path());
    temp.remove(0, 1); // deleteing first '/'
    ui->lineEditPathToArchive->setText(temp);
}
