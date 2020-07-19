// Copyright 2018 Staysail Systems, Inc. <info@staysail.tech>
// Copyright 2018 Capitar IT Group BV <info@capitoar.com>
// Copyright 2020 Dirac Research AB <robert.bielik@dirac.com>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

// This program is just a simple client application for our demo server.
// It is in a separate file to keep the server code clearer to understand.
//
// Our demonstration application layer protocol is simple.  The client sends
// a message which the server just echoes back directly.

//  For example:
//
//  % ./ws_server ws://127.0.0.1:5555/test &
//  % ./ws_client ws://127.0.0.1:5555/test "Message"
//  Client received: Message

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <nng/nng.h>
#include <nng/supplemental/util/platform.h>

void
fatal(const char *func, int rv)
{
	fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
	exit(1);
}

/*  The client runs just once, and then returns. */
int
client(const char *url, const char *msecstr)
{
	// TODO
	return (0);
}

int
main(int argc, char **argv)
{
	int rc;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <url> <message>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	rc = client(argv[1], argv[2]);
	exit(rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
