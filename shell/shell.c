#include <u.h>
#include <libc.h>
#include <bio.h>

#define MAXLINE		256
#define ARGLIMIT	32

typedef struct Cmd Cmd;
typedef struct Sym Sym;

struct Cmd
{
	char *name;
	void (*func)(int, char **);
};

struct Sym
{
	char *Sym;
	int index;
};

int 	bgetline(char *, int);
void 	copy(char *, char *);
void 	printargs(int, char **);
void 	test(void);
void 	execute(int, char **);
void	cecho(int, char **);
void	ccd(int, char **);
void	cexit(int, char **);

Cmd	cmd[] = {
	{ "echo", 	cecho },
	{ "cd",		ccd },
	{ "exit",	cexit },
	{ nil }
};

Sym	sym[] = {
	{ ">",  0 },
	{ "<",  1 },
	{ ">>", 2 },
	{ "|",  3 },
	{ "&",  4 },
	{ nil }
};

void
usage(void)
{
	fprint(2, "usage: %s \n", argv0);
	exits("usage");
}

void
main(int, char **)
{
	char line[MAXLINE], *argv[ARGLIMIT];
	int argc;
	Cmd *cp;

	print("cde shell\n");
	print("* ");
	while (bgetline(line, MAXLINE) > 0) {
		//print("input: %s", line);
		argc = tokenize(line, argv, nelem(argv) -1);
		if (argc < 1)
			continue;
		//printargs(args);
		for (cp = cmd; cp->name; cp++)
			if (strcmp(cp->name, argv[0]) == 0) {
				(*cp->func)(argc, argv);
				break;
			}
		if (cp ->name) {
			print("* ");
			continue;
			}		
		execute(argc, argv);
		print("* ");
	}
	exits(nil);
}

void
execute(int argc, char **argv)
{
	char name[32];
	Sym *sp; 
	int i, fd;
	
	argv[argc] = nil;
	if (access(argv[0], AEXEC) < 0) {
		snprint(name, sizeof name, "/bin/%s", argv[0]);
		if (access(name, AEXEC) < 0) {
			print("unable to execute || couldn't locate binary\n");
			return;
		}
	} else {
		print("local bin\n");
		snprint(name, sizeof name, "%s", argv[0]);
	}
	switch (fork()) {
	case -1:
		sysfatal("fork failed: %r");
	case 0:
		for (i = 0; argv[i]; i++)
			for(sp = sym; sp->sym; sp++)
				if (strcmp(sp->sym, argv[i])
					if (argv[i + 1])
						switch (sp->in)
						case 0:
							print("found >\n");
							fd = create(argv[i + 1], OWRITE, 0664);
							dup(fd, 1);
							close(fd);
							break;
						case 1:
							print("found <\n";
							if (access(argv[i + 1], AREAD) < 0) {
								fd = open(argv[i + 1], OREAD)
								dup(fd, 0);
								close(fd);
							}
							else {
								print("unable to open file\n");
								exits(nil);
							}
							break;
						default:
							print("problem with symbol\n");
					else {
						print("missing argument\n")
						exits(nil);
					}
				
		exec(name, argv);
		sysfatal("exec %s didn't work: %r", name);
	default:
		waitpid();
	}
}

void
printargs(int argc, char **argv)
{
	int i;
	
	for (i = 0; argc-- > 0; argv++, i++)
		print("%s\n", *argv);
}

int 
bgetline(char s[], int lim)
{
	int c, i;

	Biobuf in;

	Binit(&in, 0, OREAD);
	i  = 0;
	while ((c = Bgetc(&in)) != Beof && c != '\n')
		s[i++] = c;
	if (i >= lim - 2) {
		s[lim - 2] = '\n';
		s[lim - 1] = 0;
	}
	else if (c == '\n')
		s[i++] = c;
	s[i] = 0;
	return i;
}

void
copy(char *dest, char *src)
{
	int i;

	for (i = 0; (dest[i] = src[i]) != 0; ++i)
		;
}

void
cecho(int argc, char **argv)
{
	for (argv++, argc--; argc-- > 0; argv++)
		print("%s ", *argv);
	print("\n");
}

void
ccd(int argc, char **argv)
{
	if (chdir(argv[1]) < 0)
		print("unabled to cd to: %s", argv[1]);
}

void
cexit(int argc, char **argv)
{
	exits(nil);
}

void
test(void)
{
	int i;
	char *ar[32], *path;
	char **arp;
	
	path = "/bin/echo";
	ar[0] = "echo";
	ar[1] = "shell";
	ar[2] = "is";
	ar[3] = "good";
	ar[4] = 0;
	arp = &ar[0];
	i = fork();
	if (i == 0) {
		print("child process: attempt to call echo\n");
		exec(path, arp);
		exits(nil);
	}
	else {
		print("parent process: waiting for echo to return\n");
		waitpid();
		print("child has terminated\n");
	}
}
