#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define APXFILE "apx"		// format: <ASN><Whitespace><Prefix>
				// no CIDR and IPv4/6 agnostic
#define LINEBUF 4096		// max size for a line in APXFILE and logs

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

struct	rentry_t {

	char	px[16];		// IPv6 prefix the longest

unsigned int	asn;

	}	*entry;

	size_t	ensize;

int		recompar(const void*, const void*);

unsigned int	psearch(const void*, int);
