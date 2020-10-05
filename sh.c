#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARGLEN	32
#define ARGC	64
#define LINE	256

int	gtline(FILE *, char *, int);
char	**gtlist(char *, int, int);
void	printlist(char **);
void	freelist(char **, int, int);
void	usage(char *);
void	exec(char **);
int	isspc(int);

void main()
{
	char **list;
	char buf[LINE];
	FILE *f;

	f = fdopen(0, "r");
	while (gtline(f, buf, LINE) > 0)  {
		list = gtlist(buf, ARGC, ARGLEN);
		printlist(list);
		exec(list);
		freelist(list, ARGC, ARGLEN);
//		printlist(list);
	}
}

void usage(char *msg)
{
	printf("usage: %s\n");
	exit(0);
}

int isspc(int c)
{
	return (c == ' ' || c == '\t') ? 1 : 0;
}

char **gtlist(char *s, int argc, int arglen)
{
	char **list;
	int n, i, j;

	/* skip blanks at beginning */
	for (n = 0; isspc(s[n]); n++)
		;
	if (!s[n])
		return list = 0;
	i = j = 0;
	list = malloc(sizeof(char *) * argc);
	list[i] = malloc(sizeof(char) * arglen);
	while (i < argc - 1 && s[n]) {
		if (isspc(s[n])) {
			/* skip to start of next word */
nextwd:			while (isspc(s[n]))
				n++;
			list[++i] = malloc(sizeof(char) * arglen);
			j = 0;
			continue;
		}
		/* inner pointer is full, skip the rest of the word */
		else if (j == arglen - 1 && s[n]) {
			while (s[n] && !isspc(s[n++]))
				;
			goto nextwd;
		}
		list[i][j++] = s[n++];
	}
//	puts(list[i]);
//	exit(0);
//	list[++i] = (char *) NULL;
	return list;
}

char **gtlist2(char *s, int argc, int arglen)
{
	char **list;
	int n, i, j;

	list = malloc(sizeof(char *) * argc);
	for (i = 0; i < argc; i++)
		list[i] = malloc(sizeof(char) * arglen);
	/* skip blanks at beginning */
	for (n = 0; isspc(s[n]); n++)
		;
	for (i = j = 0; i < argc - 1 && s[n]; ) {
		if (isspc(s[n])) {
			/* skip to start of next word */
nextwd:			while (isspc(s[n]))
				n++;
			i++;
			j = 0;
			continue;
		}
		/* inner pointer is full, skip the rest of the word */
		else if (j == arglen - 1 && s[n]) {
			while (s[n] && !isspc(s[n++]))
				;
			goto nextwd;
		}
		list[i][j++] = s[n++];
	}
	/* execv exepects a null terminated list */
//	list[i] = (char *) NULL;
	return list;
}

void freelist(char **list, int argc, int arglen)
{
	//int c, i;
	// super weird undefined behavior; figure out later
	/*
	for (c = 0; c < argc; free(list[c++])) 
		;
	*/
	free(list);
}

void printlist(char **l)
{
	int i;
	
	for (i; l[i][0]; i++)
		puts("here");
		printf("%s ", l[i]);
	printf("\n");
}

/* "lines" do not include '\n' */
int gtline(FILE *f, char *b, int lim)
{
	int c, n;

	for (n = 0; n < lim - 1 && (c = getc(f)) > 0 && c != '\n'; n++)
		b[n] = c;
	if (n < lim - 1) 
		b[n] = 0;
	return n;
}

void exec(char **list)
{
	switch (fork()) {
	case -1:
		usage("err fork");
	case  0:
		puts("here");
		execv(list[0], list);
		usage("err exec");
	}
}
