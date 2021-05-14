#pragma once


#include <QString>

class EntriesManager
{
    QString _zip_file_path;
    bool _mmap_use; // true

public:
    explicit EntriesManager(const QString& zip_file_path);
    virtual ~EntriesManager();

    // sets mz_zip_reader_open_file or mz_zip_reader_open_file_in_memory using
    void setMmapUse(bool use);
    bool getMmapUse()   const;

    QMap<QString, QString> getAllEntries() const;



};
