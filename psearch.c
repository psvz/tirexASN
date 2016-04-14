#include "tirex.h"

unsigned int	// ASN
psearch(const void *ip, int nb)
{

	int	a, mid;

	int	bot = 0;

	int	top = ensize - 1;


	do {

		mid = (bot + top) / 2;

		if ( (a = memcmp(&entry[mid].px, ip, nb)) < 0) {

			bot = mid;

		} else if (likely(a > 0)) {

			top = mid;

		} else return entry[mid].asn;

	} while (top - bot > 1);

	return entry[bot].asn;

}
