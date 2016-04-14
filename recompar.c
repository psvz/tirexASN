#include "tirex.h"

int
recompar(const void *s1, const void *s2)
{

	return memcmp(&((struct rentry_t*) s1)->px, &((struct rentry_t*) s2)->px, 16);

}
