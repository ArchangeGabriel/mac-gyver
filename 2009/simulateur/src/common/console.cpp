#include "console.h"

char lines[CONSOLE_MAX_LINES][CONSOLE_MAX_LEN_LINE+1];
int next_line=0;
int act_line;
int len,max_len;
bool insert=true;

tcflag_t old_c_lflag;

//-----------------------------------------------------------------------------
void read_line(char *buff)
{
  act_line=next_line;
  len=0;
  max_len=0;
  lines[act_line][0]='\0';
  
  // MODIFICATION DU FONTIONNEMENT PAR DEFAUT DU TERMINAL
  // Lit la structure "termios" de l'entrée standard
  struct termios tios;
  tcgetattr(STDIN_FILENO, &tios);
  
  // Sauve l'ancien flag "c_lflag"
  old_c_lflag = tios.c_lflag;
  
  // Passe en mode de saisie non canonique
  // (lecture d'1 caractère à la fois par défaut, car MIN=1 et TIME=0) 
  tios.c_lflag &= ~ICANON;
  tios.c_lflag &= ~ECHO;	  
  tcsetattr(STDIN_FILENO, TCSANOW, &tios);
  // UTILISATION DU TERMINAL
  while(true) {
	  // Lit un caractère sur l'entrée standard
	  unsigned char c;
	  read(STDIN_FILENO, &c, 1);
	  // Quitte si touche "Entrée"
	  if (c=='\n')
	  {
	    printf("\n");
		  break;
		}
		else if(c==127)      //backspace
		{
		  if(len>0)
		  {
		    for(int i=len-1;i<max_len;i++)
		      lines[act_line][i]=lines[act_line][i+1];						      			      
		    len--;
        max_len--;  
		    printf("\b%s \b",&lines[act_line][len]);
		    for(int i=len;i<max_len;i++)
			    printf("\b");         
 	      fflush(stdout);
 	    }
		}
		else if(c==27)     // échappement
		{
		  read(STDIN_FILENO, &c, 1);
		  if(c=='[')
		  {
		    read(STDIN_FILENO, &c, 1);
		    switch(c)
		    {
		      case 'A':
		      if(((act_line+1)%10)!=next_line && lines[(act_line+1)%10][0]!='\0') 
		      {
		        act_line=(act_line+1)%10;
		        for(int i=len;i<max_len;i++)
		          printf(" ");
		        for(int i=len;i<max_len;i++)
		          printf("\b");  
		        for(int i=len;i>0;i--)
		          printf("\b \b");
		        printf("%s",lines[act_line]);
		        fflush(stdout);
		        max_len=strlen(lines[act_line]); 
		        len=max_len;
		      }
		      break;
		      case 'B': 
		      if(act_line!=next_line) 
		      {
		        act_line=(act_line+9)%10; 
		        for(int i=len;i<max_len;i++)
		          printf(" ");
		        for(int i=len;i<max_len;i++)
		          printf("\b");  
		        for(int i=len;i>0;i--)
		          printf("\b \b"); 
		        printf("%s",lines[act_line]);	
		        fflush(stdout);	  
		        max_len=strlen(lines[act_line]); 
		        len=max_len;		                 
		      }
		      break;
		      case 'C': if(len<max_len) {printf("%c",lines[act_line][len]); fflush(stdout); len++;} break;
		      case 'D': if(len>0) {len--; printf("\b"); fflush(stdout);} break;
		      case '2': read(STDIN_FILENO, &c, 1); if(c=='~') insert=!insert; else {printf("([)(2)(%c)",c); fflush(stdout);} break;
		      case '3':
		      read(STDIN_FILENO, &c, 1);
		      if(c=='~')
		      {
		        if(len<max_len)
		        {
  			      for(int i=len;i<max_len;i++)
  	  		      lines[act_line][i]=lines[act_line][i+1];						      			      
              max_len--;  
		          printf("%s \b",&lines[act_line][len]);
		          for(int i=len;i<max_len;i++)
    			      printf("\b");         
     	        fflush(stdout);
     	      }
   	      }
          else {printf("([)(3)(%c)",c); fflush(stdout);}
		      break;
		      default: printf("([)(%c)",c); fflush(stdout);
		    } 
		  }  
		  else
		  {
		    printf("(%c)",c);
		    fflush(stdout);
		  }
		}
		else if(c!=9 /*TAB*/)
		{
      if(len<CONSOLE_MAX_LEN_LINE)
      {
        if(insert)
        {
          for(int i=max_len+1;i>len;i--)
            lines[act_line][i]=lines[act_line][i-1];
		      lines[act_line][len]=c;
  			  printf("%s",&lines[act_line][len]);
  			  for(int i=len;i<max_len;i++)
  			    printf("\b");
          len++;
          max_len++;
			  }
			  else
			  {
			    lines[act_line][len]=c;
  			  printf("%c",c);
  			  len++;
  			  if(len>max_len) max_len++;
			  }
  	    fflush(stdout);
  	  }
		}
  }
  if(strcmp(lines[(next_line+1)%10],lines[act_line]) && lines[act_line][0]!='\0')
  {
    strcpy(lines[next_line],lines[act_line]);
    next_line=(next_line+9)%10;
  }
  strcpy(buff,lines[act_line]);
  
  // FIN
  // Restaure l'ancien flag "c_lflag"
  tios.c_lflag = old_c_lflag;
  tcsetattr(STDIN_FILENO, TCSANOW, &tios);
}	  
//------------------------------------------------------------------------------
void reset_console()
{
  struct termios tios;
  tcgetattr(STDIN_FILENO, &tios);
  tios.c_lflag = old_c_lflag;
  tcsetattr(STDIN_FILENO, TCSANOW, &tios);  
}
