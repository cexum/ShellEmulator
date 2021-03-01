#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WDLEN	24
#define LINE	256
#define MAXARGS	5

typedef struct Cmd Cmd;

struct Cmd
{
	char *name;
	void (*func)(int, char **);
};

void	printlst(char**);
int 	getln(FILE* , char*, int);
char 	**getlst2(char*, int, int); 
char    **getlst3(char*, int, int);
int 	isspc(int);
int	gtargc(char**);
void 	exec(char**);
void 	freelist(char**, int);
void	cecho(int, char**);
void	ccd(int, char**);
void	cexit(int, char**);
void	cpwd(int, char**);

Cmd	cmd[] = {
	{ "echo", 	cecho 	},
	{ "cd", 	ccd 	},
	{ "exit",	cexit 	},
	{ "pwd", 	cpwd 	},
	{ 0			}
};

int main()
{
	char line[LINE], **list;
	FILE *f;

	f = fdopen(0, "r");
	while (getln(f, line, LINE) > 0) {
//		printf("line:\t%s", line);
		list = getlst3(line, MAXARGS, WDLEN);
//		printlst(list);
		exec(list);
		freelist(list, MAXARGS);
	}
	return 0;
}

int gtargc(char **args)
{
	int c;

	c = 0;
	while (args[c])
		c++;
	return c;
}

void cpwd(int argc, char **args)
{
	char p[64];

	getcwd(p, 64);
	printf("%s\n", p);
}

void cecho(int argc, char **args)
{
	puts("define: echo");
}

void ccd(int argc, char **args)
{
	if (chdir(args[1]) == 0)
		cpwd(0, 0);
	else
		printf("could not cd\n");
}

void cexit(int argc, char **args)
{
        puts("define: exit");
}

void exec(char **list)
{
	Cmd *cp;

	for (cp = cmd; cp->name; cp++) 
		if (strcmp(list[0], cp->name) == 0) {
			(*cp->func)(gtargc(list), list);
			return;	
		}
	switch (fork()) {
		case -1:
			puts("err fork");
			exit(0);
		case  0:
			execv(list[0], list);
			puts("err execv");
			exit(0);
	}
}

void printlst(char **list)
{
	int i;
	
	i = 0;
	if (list[i] == 0) {
		puts("empty list...");
		return;
	}
	for ( ; list[i]; i++) 
		printf("token: %s\n", list[i]);
}

int getln(FILE *f, char *b, int lim)
{
	int n, c;

	n = 0;
	while ((c = getc(f)) > 0 && n < lim && c != '\n') {
		b[n++] = c;		
	}	
//	if (c && n < lim)
//		b[n++] = '\n';
	b[n] = 0;
	return n;
}

int isspc(int c)
{
	return (c == '\t'|| c == ' ') ? 1 : 0;
}

char **getlst3(char *b, int maxargs, int wdlen)
{
        char **list, *tok;
        int n;

        list = malloc(maxargs * sizeof(char*));
        for (n = 0; n < maxargs; n++)
                list[n] = malloc(wdlen * sizeof(char));
        tok = strtok(b, " ");
        n = 0;
        while (tok) {
                list[n++] = strndup(tok, wdlen - 1);
                tok = strtok(0, " ");
        }
        list[n] = 0;
        return list;
}

char **getlst2(char *b, int maxargs, int wdlen)
{
        static char *list[32];
        char *tok;
        int n;

        tok = strtok(b, " ");
        for (n = 0; tok; n++, tok = strtok(0, " "))
                list[n] = strndup(tok, wdlen);
        return list;
}

void freelist(char **list, int maxargs)
{
	int i;

	for (i = 0; i < maxargs; i++)
		free(list[i]);
	free(list);
}	
