#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <readline/readline.h>
#include <readline/history.h>
void loop();
char * readinputline();
char **parse(char * line);
int executecmd(char **args);
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
	char *line,prompt[2048],arr[1024];
	char **parsed;
	int status=1,i,j;
	while(status)
	{

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
		parsed=parse(line);
		if(parsed!=NULL)
			status=executecmd(parsed);
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
	/*
		count=1;
		for(i=0;line[i]!='\0';i++)
		{
			if(line[i]==' ')
				count++;
		}
	}
	printf("hihi\n");
	parsed=malloc(count*sizeof(char*));
	int start=0,ind=0;
	
	for(i=0;line[i]!='\0';i++)
	{
		if(line[i]==' ')
		{
			len=i-start+1;
			parsed[ind]=malloc(len*sizeof(char));
			for(j=0;j<len-1;j++)
			{
				parsed[ind][j]=line[start+j];
			}
			parsed[ind][j]='\0';
			ind++;
			start=i+1;	
		}
	}
	len=i-start+1;
	parsed[ind]=malloc(len*sizeof(char));
	for(j=0;j<len-1;j++)
	{
		parsed[ind][j]=line[start+j];
	}
	parsed[ind][j]='\0';
	*/
}
int executecmd(char **args)
{
	int status,i,j;
	int bg;
	if(args==NULL)
		return 1;
	for(i=0;args[i]!=0;i++){}
	i--;
	if(args[i][0]=='&' && args[i][1]=='\0')
		bg=1;
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
	else if(bg==0)
	{
		int pid;
		pid=fork();
		if(pid==0)
		{

			if(execvp(args[0],args)==-1)
				perror("Error executing the command");
			exit(EXIT_FAILURE);
		}
		else if(pid>0)
		{
			waitpid(pid,&status,WUNTRACED);
			return 1;
		}
		else
		{
			perror("Forking Error");
			return 1;
		}
	}
	else if(bg==1)
	{
		printf("hi\n");
		int pid;
		pid=fork();
		if(pid==0)
		{
			setpgid(0,0);
			if(execvp(args[0],args)==-1)
				perror("Error executing the command");
			exit(EXIT_FAILURE);
		}
		else if(pid>0)
		{
			//waitpid(pid,&status,WUNTRACED);
			printf("[1] %d\n",pid);
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
