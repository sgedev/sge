/*
 *
 */
#ifndef SGE_RES_H
#define SGE_RES_H

#include <GL/glex.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

bool sge_res_init(void);
void sge_res_shutdown(void);

GLEXMesh *sge_res_get_mesh(const char *path);
void sge_res_put_mesh(GLEXMesh *mesh);

SGE_END_DECLS

#endif /* SGE_RES_H */

