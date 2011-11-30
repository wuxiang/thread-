#include <pthread.h>
#include <signal.h>
#include "extend_thread.h"
#include "configure.h"

int main(int argc, char* argv[])
{
	configure config(argc, argv);
	config.dump_configure();

	sigset_t new_mask;
	sigfillset(&new_mask);
	sigset_t old_mask;
	pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

	sigset_t wait_mask;
	sigemptyset(&wait_mask);
	sigaddset(&wait_mask, SIGINT);
	sigaddset(&wait_mask, SIGQUIT);
	sigaddset(&wait_mask, SIGTERM);
	pthread_sigmask(SIG_BLOCK, &new_mask, 0);

	int sig = 0;
	sigwait(&wait_mask, &sig);

	return 0;
}
