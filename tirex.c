/*
 * Copyright (c) 2016, Vitaly Zuevsky <vitaly.zuevsky@hyvd.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "tirex.h"

int
main(int argc, char **argv)
{

	FILE	*f;

	int	i, n;

	char	line[LINEBUF], pref[LINEBUF];

struct	addrinfo	hints, *res;


	if (argc < 3) errx(EXIT_FAILURE, "2 args, e.g. \"%%*s %%*s %%s\" $'%%s\\t\"EIP='$ip$';ASNUM=%%u\"\\n'");

	if ( (f = fopen(APXFILE, "r")) == NULL) err(EXIT_FAILURE, "fopen(%s)", APXFILE);

	for (i = 0; (n = fscanf(f, "%*[^\n]"), n = fscanf(f, "%*c")) != EOF; i++);	// count prefixes

	if ( (entry = calloc(i, sizeof (struct rentry_t))) == NULL) err(EXIT_FAILURE, "calloc()");

	rewind(f);

	bzero(&hints, sizeof hints);

	hints.ai_family = AF_UNSPEC;

	hints.ai_flags = AI_NUMERICHOST;

	for (i = 0; fgets(line, sizeof line, f); i++) {

		if (sscanf(line, "%u %s", &entry[i].asn, pref) < 2)
			errx(EXIT_FAILURE, "%s format, line %d", APXFILE, ++i);

		if ( (n = getaddrinfo(pref, NULL, &hints, &res)) != 0)
			errx(EXIT_FAILURE, "getaddrinfo(%s): %s", pref, gai_strerror(n));

		switch (res->ai_family) {

		case AF_INET:

			memcpy(&entry[i].px, &((struct sockaddr_in*) res->ai_addr)->sin_addr.s_addr, 4);

			break;

		case AF_INET6:

			memcpy(&entry[i].px, &((struct sockaddr_in6*) res->ai_addr)->sin6_addr.s6_addr, 16);

			break;

		default:

			errx(EXIT_FAILURE, "unknown ai_family: %d", res->ai_family);

		}

		freeaddrinfo(res);

	}

	fclose(f);

	qsort(entry, i, sizeof (struct rentry_t), recompar);

	ensize = i;	/* The prefix array is ready */

	/* Reading the line and taking the IP address */

	while (fgets(line, sizeof line, stdin)) {

		if (sscanf(line, argv[1], pref) != 1) {

			fputs(line, stdout);

			continue;

		}

		if ( (n = getaddrinfo(pref, NULL, &hints, &res)) != 0) {

			fputs(line, stdout);

			continue;

		}

		switch (res->ai_family) {

		case AF_INET:

			line[strlen(line) - 1] = '\0';  // killing \n

			printf(argv[2], line, psearch(&((struct sockaddr_in*) res->ai_addr)->sin_addr.s_addr, 4));

			break;

		case AF_INET6:

			line[strlen(line) - 1] = '\0';  // killing \n

			printf(argv[2], line, psearch(&((struct sockaddr_in6*) res->ai_addr)->sin6_addr.s6_addr, 16));

			break;

		default:

			fputs(line, stdout);

		}

		freeaddrinfo(res);

	}


	return EXIT_SUCCESS;

}
