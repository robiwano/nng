// Copyright 2018 Staysail Systems, Inc. <info@staysail.tech>
// Copyright 2018 Capitar IT Group BV <info@capitoar.com>
// Copyright 2020 Dirac Research AB <robert.bielik@dirac.com>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

// This program serves as an example for how to write a Websocket service,
// TODO: Document this example

// To run this program, start the server as ./ws_server <url>
// Then connect to it with the client as ./ws_client <url> <message to send>
//
//  For example:
//
//  % ./server ws://127.0.0.1:5555/test &
//  % ./client ws://127.0.0.1:5555/test "Message"
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

typedef struct {
	nng_stream_listener *l;
	nng_aio *            aio;
} listener;

typedef struct {
	listener *  l;
	nng_stream *s;
	nng_aio *   aio;
	int         err;
	char        buf[1024];
	size_t      offset;
} stream;

static void
stream_handle_cb(void *arg)
{
	stream *s   = (stream *) arg;
	int     rv  = nng_aio_result(s->aio);
	size_t  len = nng_aio_count(s->aio);
	fprintf(stderr, "Res: %s, Len: %zu\n", nng_strerror(rv), len);
	switch (rv) {
	case 0: {

	} break;
	case NNG_ECLOSED: {
		// nng_aio_free(s->aio);
	} break;
	default:
		break;
	}
}

static void
stream_accept_cb(void *arg)
{
	listener *l  = (listener *) arg;
	int       rv = nng_aio_result(l->aio);
	switch (rv) {
	case 0: {
		stream *s = (stream *) nng_alloc(sizeof(stream));
		s->l      = l;
		s->s      = nng_aio_get_output(l->aio, 0);
		if ((s->err = nng_aio_alloc(&s->aio, stream_handle_cb, s)) ==
		    0) {
			nng_iov iov;
			iov.iov_buf = s->buf;
			iov.iov_len = sizeof(s->buf);
			if (nng_aio_set_iov(s->aio, 1, &iov) == 0) {
				nng_stream_recv(s->s, s->aio);
			}
		}
	} break;
	case NNG_ECLOSED:
		// Do nothing ?
		return;
	default:
		break;
	}

	// Restart accept
	nng_stream_listener_accept(l->l, l->aio);
}

// The server runs forever.
int
server(const char *url)
{
	listener l;
	int      rv;

	/*  Create stream listener */
	rv = nng_stream_listener_alloc(&l.l, url);
	if (rv != 0) {
		fatal("nng_stream_listener_alloc", rv);
	}

	/* Handle accepts in aio callback */
	rv = nng_aio_alloc(&l.aio, stream_accept_cb, &l);
	if (rv != 0) {
		fatal("nng_aio_alloc", rv);
	}

	rv = nng_stream_listener_listen(l.l);
	if (rv != 0) {
		fatal("nng_stream_listener_listen", rv);
	}

	nng_stream_listener_accept(l.l, l.aio);

	for (;;) {
		nng_msleep(1000); // neither pause() nor sleep() portable
	}
}

int
main(int argc, char **argv)
{
	int rc;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <url>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	rc = server(argv[1]);
	exit(rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
