#include <fstream>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/unordered_map.hpp>
#include <boost/detail/endian.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/atomic.hpp>
#include <boost/algorithm/string.hpp>
#include <memory>
#include <cmath>
#include <boost/static_assert.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include "glm_fix.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <cstdio>
#include <cstdlib>
#include <set>
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rstdint.h"
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
#include "program_phases.h"
#include "timer.h"
#include "texture_base.h"
#include "pngtexture.h"
#include "tgatexture.h"
#include "jsonreader.h"
#include "repository.h"
#include "factory.h"
#include "id_storage.h"
#include "auto_id.h"
#include "cache.h"
#include "model_value.h"


// TODO: remove
using namespace platform;
typedef std::auto_ptr<platform::File> AutoFile;

