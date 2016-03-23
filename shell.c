#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void loop();
int fd[128][2];
char * readinputline();
char **parse(char * line);
int executecmd(char **args,int fin, int fout,int p);
void pwd();
void getname();
int count;
int main()
{
	loop();
	return EXIT_SUCCESS;
}
char pwdarr[1024],namearr[1024];
void loop()
{
	int stat,r;
	char *line,prompt[2048],arr[1024];
	char **parsed;
	int status=1,i,j;
	while(status)
	{
		r=waitpid(-1,&stat,WNOHANG);
		if(r>0)
			printf("%d exited with status %d\n",r,stat);
		for(i=0;prompt[i]!='\0';i++)
		{
			prompt[i]='\0';
		}
		prompt[0]='>';
		getname();
		j=1;
		for(i=0;namearr[i]!='\0';i++)
		{
			prompt[j++]=namearr[i];
		}
		for(i=0;namearr[i]!='\0';i++)
			namearr[i]='\0';
		prompt[j++]=' ';
		prompt[j++]=':';
		prompt[j++]=' ';
		pwd();
		for(i=0;pwdarr[i]!='\0';i++)
		{
			prompt[j++]=pwdarr[i];
		}
		for(i=0;pwdarr[i]!='\0';i++)
			pwdarr[i]='\0';
		prompt[j++]=' ';
		line=readline(prompt);
		add_history(line);
		parsed=parse(line);
		if(parsed!=NULL)
			status=checkpipes(parsed);
		free(line);
		free(parsed);
	}
}
char *readinputline()
{
	char *line=NULL;
	ssize_t bufsize=0;
	line=readline("");
	return line;
}
char **parse(char * line)
{
	if(line==NULL)
		return NULL;
	else if(line[0]=='\0')
		return NULL;
	else
	{
		char **parsed;
		int i,len,j=0;
		parsed=malloc(256*sizeof(char*));
		char *token;
		parsed[j++]=strtok(line," \t\n\a\r");
		while(1)
		{
			parsed[j++]=strtok(NULL," \t\n\r\a");
			if(parsed[j-1]==NULL)
				break;
		}
		return parsed;	
	}
}
int executecmd(char **args,int fin, int fout,int p)
{
//	printf("p is %d\n",p);
	int i,j,in,out;
	int bg,status;
	if(args==NULL)
		return 1;
	for(i=0;args[i]!=0;i++){}
	i--;
	if(args[i][0]=='&' && args[i][1]=='\0')
	{
		bg=1;
	}
	else
		bg=0;
	if(args[0][0]=='c' && args[0][1]=='d')
	{
		if(args[1]==0)
		{
			args[1]=malloc(1024*sizeof(char));
			args[1][0]='/';
			args[1][1]='h';
			args[1][2]='o';
			args[1][3]='m';
			args[1][4]='e';
			args[1][5]='/';
			getname();
			for(i=6,j=0;namearr[j]!='\0';i++,j++)
			{
				args[1][i]=namearr[j];
			}
			args[1][i]='\0';
		}
		if(args[1][0]=='~' && args[1][1]=='\0')
		{
			args[1]=malloc(1024*sizeof(char));
			args[1][0]='/';
			args[1][1]='h';
			args[1][2]='o';
			args[1][3]='m';
			args[1][4]='e';
			args[1][5]='/';
			getname();
			for(i=6,j=0;namearr[j]!='\0';i++,j++)
			{
				args[1][i]=namearr[j];
			}
			args[1][i]='\0';
		}
		if(chdir(args[1])!=0)
		{
			printf("error: Specify the folder.\n");
		}
		return 1; 
	}
	else if(args[0][0]=='e' && args[0][1]=='x' && args[0][2]=='i' && args[0][3]=='t')
		return 0;
	else if(args[0][0]=='e' && args[0][1]=='c' && args[0][2]=='h' && args[0][3]=='o')
		printf("%s\n",args[1]);
	else if(args[0][0]=='p' && args[0][1]=='w' && args[0][2]=='d')
	{
		pwd();
		printf("%s\n",pwdarr);	
	}
	else
	{
		int pid;
		int q,r,sin=0,sout=0;
		char *in,*out;
		pid=fork();
	//	for(q=0;args[q]!=NULL;q++)
	//		printf("%s\n",args[q]);
		if(pid==0)
		{
			if(fin==1)
			{	//printf("p issdf %d\n",p);
				close(0);
				close(fd[p-1][1]);
				dup(fd[p-1][0]);
				close(fd[p-1][0]);
			}
			if(fout==1)
			{
				//printf("dup out\n");
				close(1);
				dup(fd[p][1]);
				close(fd[p][0]);
				close(fd[p][1]);
			}
			for(p=0;args[p]!=0;p++)
			{
				if(args[p][0]=='<')
				{
					//		printf("in spotted\n");	in=malloc(256*sizeof(char));
					in=malloc(256*sizeof(char));
					for(r=1;args[p][r]!='\0';r++)
						in[r-1]=args[p][r];
					in[r-1]='\0';
					sin=1;
				}
				if(args[p][0]=='>')
				{
					//		printf("out spotted\n");
					out=malloc(256*sizeof(char));
					for(r=1;args[p][r]!='\0';r++)
						out[r-1]=args[p][r];
					out[r-1]='\0';
					sout=1;
				}	
			}

			if(sin!=0)
			{
				int fd0;
				fd0=open(in,O_RDONLY,0);
				if(fd0<0)
				{
					printf("Input file %s does not exist\n",in);
					exit(EXIT_FAILURE);
				}
				free(in);
				dup2(fd0,0);
				close(fd0);
			}

			if(sout!=0)
			{
				int fd1;
				fd1=creat(out,0644);
				free(out);
				dup2(fd1,1);
				close(fd1);
			}
			if(bg==1)
			{
				setpgid(0,0);
				args[i]=0;
			}
			for(i=0;args[i]!=NULL;i++)
			{	
				if(args[i][0]=='<' || args[i][0]=='>')
					break;
			}
			for(;args[i]!=NULL;i++)
				args[i]=NULL;
			if(execvp(args[0],args)<0)
				perror("Error executing the command");
			perror("done");
			exit(EXIT_FAILURE);
		}
		else if(pid>0)
		{
			if (bg==0)
				waitpid(pid,&status,WUNTRACED);
			else
				printf("[1] haha %d\n",pid);
			return 1;
		}
		else
		{
			perror("Forking Error");
			return 1;
		}
	}
}
void pwd()
{
	getcwd(pwdarr, sizeof(pwdarr));
}
void getname()
{
	getlogin_r(namearr,sizeof(namearr));
}
int checkpipes(char **args)
{
	int start=0,end=0,status;
	int i,j,p=0,fin=0,fout=0;;
	char **nargs=malloc(1024*sizeof(char *));
	for(i=0;args[i]!=0;i++)
	{
		if(args[i][0]=='|')
		{
			end=i;
			for(j=start;j<i;j++)
			{
				nargs[j-start]=args[j];
			}
			nargs[j]=0;
			pipe(fd[p]);
			if(start!=0)
				fin=1;
			else
				fin=0;
			fout=1;
			start=i+1;
			status=executecmd(nargs,fin,fout,p);
			p++;
			if(status==0)
				return 0;
		}
	}
	for(i=start;args[i]!=0;i++)
	{
		nargs[i-start]=args[i];
	}
	nargs[i-start]=NULL;
	if(start==0)
		fin=0;
	else
		fin=1;
	status=executecmd(nargs,fin,0,p);
	if(status==0)
		return 0;
	return 1;	
}
