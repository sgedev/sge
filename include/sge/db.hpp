//
//
#ifndef SGE_DB_HPP
#define SGE_DB_HPP

#include <memory>

#include <sge/db/common.hpp>
#include <sge/db/blob.hpp>

SGE_DB_BEGIN

typedef std::shared_ptr<blob> blob_ptr;

bool init(const char *filename);
void shutdown(void);
bool get_boolean(const char *path, bool def_value = false);
int get_integer(const char *path, int def_value = 0);
double get_number(const char *path, double def_value = 0.0);
const char *get_string(const char *path, const char *def_value = NULL);
glm::ivec3 get_vector3i(const char *path, const glm::ivec3 def_value);
glm::vec3 get_vector3f(const char *path, const glm::vec3 def_value);
blob_ptr get_blob(const char *path);

SGE_DB_END

#endif // SGE_DB_HPP

