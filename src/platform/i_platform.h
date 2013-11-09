#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/detail/endian.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <memory>
#include <cmath>
#include <boost/static_assert.hpp>
#include "glm_fix.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <zlib.h>
#include <cstdio>
#include <cstdlib>
#include <png.h>
#include <json/json.h>

#include "stdint.h"
#include "serialize.h"
#include "singleton.h"
#include "log.h"

#include "compression.h"
#include "ifile.h"
#include "package.h"
#include "filesystem.h"
#include "osfile.h"
#include "memoryfile.h"
#include "register.h"
#include "event.h"
#include "timer.h"
#include "pngtexture.h"
#include "jsonreader.h"
#include "repository.h"
#include "id_storage.h"
#include "auto_id.h"
