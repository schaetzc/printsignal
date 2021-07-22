#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// When using glib 2.32 or higher, you can also print sigabbrev_np(...) (e.g. "HUP" for SIGHUP)

struct sigaction siga;

int has_pid(siginfo_t *siginfo) {
    return siginfo->si_signo == SIGCHLD || siginfo->si_code == SI_USER || siginfo->si_code == SI_QUEUE;
}

void handler(int sig, siginfo_t *siginfo, void *context) {
    printf("Caught signal %d (%s) ", sig, strsignal(sig));
    if (has_pid(siginfo)) {
        printf("from process %ld of user %ld", (long) siginfo->si_pid, (long) siginfo->si_uid);
    } else if (siginfo->si_code == SI_KERNEL) {
        printf("from kernel");
    }
    printf("\n");
    exit(0);
}

void register_verbose(int sig, int sigaction_status) {
    if (sigaction_status == 0) {
        printf("Handling signal %d (%s)\n", sig, strsignal(sig));
    } else {
        printf("Cannot handle signal %d (%s), %s\n", sig, strsignal(sig), strerror(errno));
    }
}

void register_all_signals() {
    siga.sa_sigaction = *handler;
    siga.sa_flags |= SA_SIGINFO;
    for (int sig = 1; sig <= SIGRTMAX; ++sig) {
        int status = sigaction(sig, &siga, NULL);
#if VERBOSE
        register_verbose(sig, status);
#endif
    }
}

int main(int argc, char *argv[]) {
    printf("This is process %ld waiting for a signal\n", (long) getpid());
    register_all_signals();
    pause();
    return 0;
}
