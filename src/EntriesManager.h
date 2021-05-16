#pragma once

#include <QString>
#include <QList>
#include <QPair>

using Entries_t = QList<QPair<QString, int64_t>>;

//=================================================================================================
class EntriesManagerException : public std::runtime_error
{
public:
    enum ErrorType : uint8_t
    {
        OpenFileError  = 0x01U,
        NoEntriesError = 0x02U,
        GetInfoError   = 0x03U
    };

    ErrorType _error_type;

    EntriesManagerException(ErrorType error_type, const char* what = "")
        : runtime_error(what),
          _error_type{error_type} {}
};
//=================================================================================================
class EntriesManager
{
protected:
    std::string _zip_file_path;
    void*       _zip_reader;
    bool        _mmap_use;

public:
    explicit EntriesManager(const std::string& zip_file_path, bool mmap_use = true);
    virtual ~EntriesManager();

    // sets mz_zip_reader_open_file() or mz_zip_reader_open_file_in_memory() using
    void setMmapUse(bool use);
    bool getMmapUse()   const;

    // IN-arg in order to write data partially if an exception is thrown
    void getAllEntries(Entries_t& entries);
};
