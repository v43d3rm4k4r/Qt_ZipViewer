#include "EntriesManager.h"

#include "minizip-ng/mz.h"
#ifdef HAVE_COMPAT
#include "mz_compat.h"
#endif
#include "minizip-ng/mz_crypt.h"
#include "minizip-ng/mz_os.h"
#include "minizip-ng/mz_strm.h"
#ifdef HAVE_BZIP2
#include "minizip-ng/mz_strm_bzip.h"
#endif
#ifdef HAVE_PKCRYPT
#include "minizip-ng/mz_strm_pkcrypt.h"
#endif
#include "minizip-ng/mz_strm_mem.h"
#include "minizip-ng/mz_strm_os.h"
#ifdef HAVE_WZAES
#include "mz_strm_wzaes.h"
#endif
#ifdef HAVE_ZLIB
#include "mz_strm_zlib.h"
#endif

#include "minizip-ng/mz_zip.h"
#include "minizip-ng/mz_zip_rw.h"

#include <QDebug>

EntriesManager::EntriesManager(const std::string& zip_file_path, bool mmap_use)
    : _zip_file_path{zip_file_path},
      _zip_reader{nullptr},
      _mmap_use{mmap_use}
{}
//=================================================================================================
void EntriesManager::setMmapUse(bool use)
{
    _mmap_use = use;
}
//=================================================================================================
bool EntriesManager::getMmapUse() const
{
    return _mmap_use;
}
//=================================================================================================
void EntriesManager::getAllEntries(QList<QPair<QString, int64_t>>& entries)
{
    ::mz_zip_reader_create(&_zip_reader);

    if (_mmap_use)
    {
        if (::mz_zip_reader_open_file_in_memory(_zip_reader, _zip_file_path.c_str()) != MZ_OK)
        {
            throw EntriesManagerException(EntriesManagerException::OpenFileError);
            ::mz_zip_reader_delete(&_zip_reader);
        }
    }
    else
    {
        if (::mz_zip_reader_open_file(&_zip_reader, _zip_file_path.c_str()) != MZ_OK)
        {
            throw EntriesManagerException(EntriesManagerException::OpenFileError);
            ::mz_zip_reader_delete(&_zip_reader);
        }
    }
    uint32_t entries_count = 0;
    int32_t err = ::mz_zip_reader_goto_first_entry(_zip_reader);
    if (err == MZ_OK)
    {
        do {
            mz_zip_file* file_info = nullptr;
            if (::mz_zip_reader_entry_get_info(_zip_reader, &file_info) != MZ_OK)
            {
                throw EntriesManagerException(EntriesManagerException::GetInfoError,
                                              std::to_string(entries_count).c_str());
                break;
            }

            if (::mz_zip_reader_entry_is_dir(_zip_reader) != MZ_OK)
            {
                entries.append(QPair<QString, int64_t>());
                entries[entries_count].first  = file_info->filename;
                entries[entries_count].second = file_info->uncompressed_size;
                ++entries_count;
            }



        } while (::mz_zip_reader_goto_next_entry(_zip_reader) == MZ_OK);
    }
    else if (err == MZ_END_OF_LIST)
    {
        throw EntriesManagerException(EntriesManagerException::NoEntriesError);
    }

    ::mz_zip_reader_close(_zip_reader);
    ::mz_zip_reader_delete(&_zip_reader);
}
//=================================================================================================
EntriesManager::~EntriesManager()
{
    if (_zip_reader != nullptr)
    {
        ::mz_zip_reader_close(_zip_reader);
        ::mz_zip_reader_delete(&_zip_reader);
    }
}
