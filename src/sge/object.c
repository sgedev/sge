/*
 *
 */
#include <sge/object.h>

struct sge_object {
	sge_object_t *parent;
	cx_list_t children;
	cx_list_node_t node;
	char *name;
	int flags;
	hmm_vec3 pos;
	hmm_vec3 scale;
	hmm_quaternion rotation;
};



