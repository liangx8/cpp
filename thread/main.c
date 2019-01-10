#include <pthread.h>
#include <stdio.h>
#include <locale.h>
#include <libintl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)


#define LOCALEDIR "locale/"
#ifndef PACKAGE
#define PACKAGE "sample"
#endif
struct thread_info{
	pthread_t	thread_id;
	int			thread_num;
	char		*argv_string;
};

static void *thread_start(void *arg)
{
	struct thread_info *tinfo=arg;
	char *uargv,*p;
	printf(gettext("Thread %d: top of stack near %p; argv_start=%s\n"),
		tinfo->thread_num,&p,tinfo->argv_string);
	uargv = strdup(tinfo->argv_string);
	if(uargv==NULL)
		handle_error("strdup");
	for(p = uargv; *p != '\0';p++){
		*p=toupper(*p);
	}
	return uargv;
}
int main(int argc,char **argv)
{
	const char *loc_name=setlocale(LC_ALL,"");
	bindtextdomain(PACKAGE,LOCALEDIR);
	textdomain(PACKAGE);
	printf(gettext("Locale name:%s\n"),loc_name);
	int s,tnum,opt,num_threads;
	struct thread_info *tinfo;
	pthread_attr_t attr;
	int stack_size;
	char *res;
	stack_size = -1;
	while((opt=getopt(argc,argv,"s:"))==-1){
		switch(opt){
			case 's':
				stack_size=strtoul(optarg,NULL,0);
			default:
				printf(gettext("Usage: %s [-s stack-size] arg...\n"),argv[0]);
				exit(-1);
		}
	}
	num_threads = argc - optind;
	s = pthread_attr_init(&attr);
	if (s != 0){
		handle_error_en(s,"pthread_attr_init");
	}
	if(stack_size > 0){
		s = pthread_attr_setstacksize(&attr,stack_size);
		if(s != 0){
			handle_error_en(s,"pthread_attr_init() set stack size failed");
		}
	}
	tinfo = calloc(num_threads,sizeof(struct thread_info));
	if(tinfo==NULL){
		handle_error("calloc");
	}
	for (tnum = 0; tnum < num_threads;tnum++){
		tinfo[tnum].thread_num = tnum + 1;
		tinfo[tnum].argv_string = argv[optind + tnum];
		s = pthread_create(&tinfo[tnum].thread_id,&attr,thread_start,&tinfo[tnum]);
		if(s != 0){
			handle_error_en(s,"pthread_create");
		}
	}
	s = pthread_attr_destroy(&attr);
	if(s != 0){
		handle_error_en(s,"pthread_attr_destroy");
	}
	/* Now join with each thread, and display its returned value */
	for (tnum = 0 ;tnum < num_threads;tnum ++){
		s = pthread_join(tinfo[tnum].thread_id,(void **)&res);
		if(s != 0){
			handle_error_en(s,"pthread_join");
		}
		printf(gettext("Joined with thread %d; returned value was %s\n"),
			tinfo[tnum].thread_num,res);
			free(res);
	}
	free(tinfo);
	exit(0);
}