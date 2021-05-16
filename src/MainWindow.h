#pragma once

#include <QMainWindow>
#include <QFileDialog>

#include "EntriesModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow* ui;
    QFileDialog*  _file_dialog;
    EntriesModel* _entries_model;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonOpenArchive_clicked();
    void on_pushButtonFileDialog_clicked();
    void urlSelected(const QUrl&);
};
