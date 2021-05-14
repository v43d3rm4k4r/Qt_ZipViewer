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


EntriesManager::EntriesManager(const QString& zip_file_path)
{

}
