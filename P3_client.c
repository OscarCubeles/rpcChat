/***********************************************
*
* @Purpose: Client side implementation of an RPC chat app
* @Author: Oscar Cubeles
* @Creation Date: 26/11/2022
* @Last Updated: 27/11/2022
*
************************************************/
#define _XOPEN_SOURCE 500
#include "P3.h"
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <ncurses.h>


int  num_messages = 0;
char *write_chat_1_arg = NULL;
char *nickname;
pthread_t thread_receive;
pthread_mutex_t lock;


/***********************************************
*
* @Purpose: Increments the number of messages read by the client using a mutex.
* @Parameters: -
* @Return: -
*
************************************************/
void incrementMsgCount(){
	pthread_mutex_lock(&lock);
	num_messages++;
	pthread_mutex_unlock(&lock);
}


/***********************************************
*
* @Purpose: Sets to 1 the number of messages read by the client using a mutex.
* @Parameters: -
* @Return: -
*
************************************************/
void initMsgCount(){
	pthread_mutex_lock(&lock);
	num_messages = 1;
	pthread_mutex_unlock(&lock);
}


/***********************************************
*
* @Purpose: Method that constantly checks if there are new unread messages in the server.
* @Parameters: in: *host: pointer containing the host where to connect to the server.
* @Return: -
*
************************************************/
void * requestNewChats(void *host){
	CLIENT *clnt;
	char **result_2;

	clnt = clnt_create (host, RPC, FIRST_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	initMsgCount();
	// Infinite loop requesting new chat messages
	while(1){
		// Request server for new chat messages
		result_2 = get_chat_1(&num_messages, clnt);
		if(result_2 != NULL){
			// Writting the message to screen
			//write(1, *result_2, strlen(*result_2));
			//printw("%s", result_2);
		//	getmaxyx(stdscr,row,col);
			mvprintw(num_messages,0,"%s",result_2[0]);
			refresh();
			// Incrementing the number of messages read by this client
			incrementMsgCount();
		}
	}
	return NULL;
}


/***********************************************
*
* @Purpose: Method that handling signals raised from the os. In this case just handling ctrl+C
* @Parameters: in: sig_num: Number of the signal raised.
* @Return: -
*
************************************************/
void sig_handler(int sig_num){
	// Safely finish program when ctrl+C
	if(sig_num == SIGINT){
		free(write_chat_1_arg);
		pthread_kill(thread_receive, SIGTERM);
		pthread_join(thread_receive, NULL);
		pthread_mutex_destroy(&lock);
		exit(0);
	}
	endwin();
}




/***********************************************
*
* @Purpose: Method that constantly checks if the user has written new messages
*           and uploads them to the server.
* @Parameters: in: *host: pointer containing the host where to connect to the server.
* @Return: -
*
************************************************/
void rpc_1(char *host) {
	void  *result_1;
	char *write_chat_1_arg;
	char msg[512];
	char chat[1024];
	CLIENT *clnt;

	#ifndef	DEBUG
		clnt = clnt_create (host, RPC, FIRST_VERSION, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
			endwin();
		}
	#endif

	// initial memory allocation for the chat msg
	write_chat_1_arg = (char *)malloc(sizeof(char)*1);
	int i = 0;
	char ch;
	int end = 0;

	// Infinite loop to listen to terminal input
	while(1){
		// Read from terminal and assign terminal input to *write_chat_1_arg
		i = 0;
		end = 0;
		bzero(msg, sizeof(msg));
		while(!end){
	    ch = getch();
	    msg[i++] = ch;
			printw("%c", ch);
			refresh();
	    if(ch == '\n'){
	      end = 1;
	      i++;
	      msg[i - 1] = '\0';
	    }
			if(ch == '*'){
				raise(SIGINT);
			}
		}
		if(strcmp("*", msg) == 0){
			raise(SIGINT);
		}
		incrementMsgCount();
		sprintf(chat, "%s: %s", nickname, msg);
		write_chat_1_arg = (char *)realloc(write_chat_1_arg, sizeof(char)*(i + 1 + strlen(chat)));
		bzero(write_chat_1_arg, sizeof(char) * (i + 1 + strlen(chat)));
		strcpy(write_chat_1_arg, chat);
		// Request server to write the chat *write_chat_1_arg to its log file
		result_1 = write_chat_1(&write_chat_1_arg, clnt);
		if (result_1 == (void *) NULL) {
			clnt_perror (clnt, "call failed");
		}
	}

	#ifndef	DEBUG
		clnt_destroy (clnt);
	#endif	 /* DEBUG */
}


/***********************************************
*
* @Purpose: Prints in screen if the program arguments are not valid.
* @Parameters: in: *argv0: pointer containing the program invoked
* @Return: -
*
************************************************/
void printArgcError(char* argv0){
	printf("usage: %s server_host.\n", argv0);
	printf("Incorrect number of arguments.\n");
	printf("Correnct format: P3_client <host> <nickname>\n");
}


/***********************************************
*
* @Purpose: Prints in screen a welcomming message to the client.
* @Parameters: -
* @Return: -
*
************************************************/
void printWelcomeMsg(){
	initscr();
  printw("------Welcome!------\n");
  refresh();
	raw();
	keypad(stdscr, TRUE);
	noecho();
}


/***********************************************
*
* @Purpose: Creates the thread that checks for new messages in the server.
* @Parameters: in: *host: pointer containing the host where to connect to the server.
* @Return: -
*
************************************************/
void startMsgRequests(char *host){
	pthread_mutex_init(&lock, NULL);
	pthread_create(&thread_receive, NULL, &requestNewChats, host);
}


int main (int argc, char *argv[]){
	char *host;
	if (argc != 3) {
		printArgcError(argv[0]);
		exit (1);
	}
	host = argv[1];
	nickname = argv[2];
	printWelcomeMsg();
	signal(SIGINT, sig_handler);
	startMsgRequests(host);
	rpc_1(host);
	pthread_join(thread_receive, NULL);
	exit (0);
}
