/* $Id$ */

#include <sys/types.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define PATH_DEV_ADBEV "/dev/adbev"

struct keytab;

void lowervol(struct keytab *);
void raisevol(struct keytab *);
void mute(struct keytab *);

struct keytab {
	char *name;
	int id;
	void (*action)(struct keytab *);
} keys[] = {
	{ "F1", 0, NULL },
	{ "F2", 1, NULL },
	{ "F3", 2, lowervol },
	{ "F4", 3, raisevol },
	{ "F5", 4, NULL },
	{ "F6", 5, mute }
};
#define NKEYS (sizeof(keys) / sizeof(keys[0]))

int verbose;

void
usage(int status)
{
	extern char *__progname;

	fprintf(stderr, "usage: %s [-dhv]\n", __progname);
	exit(status);
}

struct keytab *
lookup(int id)
{
	struct keytab *kte;

	for (kte = keys; kte - keys < NKEYS; kte++)
		if (kte->id == id)
			return kte;
	return NULL;
}

int
main(int argc, char *argv[])
{
	int ch, fd;
	char buf[BUFSIZ], *p, *dev = PATH_DEV_ADBEV;
	ssize_t len;
	struct keytab *kte;

	while ((ch = getopt(argc, argv, "d:hv")) != -1)
		switch (ch) {
			case 'd':
				dev = optarg;
				break;
			case 'h':
				usage(0);
				/* NOTREACHED */
			case 'v':
				verbose = 1;
				break;
			default:
				usage(1);
				/* NOTREACHED */
		}

	if ((fd = open(dev, O_RDONLY)) == -1)
		err(1, "%s", dev);

	while ((len = read(fd, buf, sizeof(buf))) > 0)
		for (p = buf; p - buf < len; p++)
			if ((kte = lookup(*p)) != NULL &&
			    kte->action != NULL)
				(*kte->action)(kte);
	if (len == -1)
		err(1, "read");
	close(fd);
	return 0;
}

void
mute(struct keytab *kte)
{
	printf("mute\n");
}

void
raisevol(struct keytab *kte)
{
	printf("raisevol\n");
}

void
lowervol(struct keytab *kte)
{
	printf("lowervol\n");
}
