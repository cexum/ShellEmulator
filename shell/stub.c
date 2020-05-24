#include <u.h>
#include <libc.h>
#include <bio.h>

void
usage(void)
{
	fprint(2, "usage: %s \n", argv0);
	exits("usage");
}

void
main(int argc, char **argv)
{
	ARGBEGIN {
	default:
		usage();
		break;
	} ARGEND
}


