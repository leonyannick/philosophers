#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;

void* routine(void *arg) {
	pthread_mutex_t *lock;

	lock = (pthread_mutex_t *)arg;


    for (int i = 0; i < 1000000; i++) {
		pthread_mutex_lock(lock);
        mails++;
		pthread_mutex_unlock(lock);
        // read mails
        // increment
        // write mails
    }
}
// if (pthread_mutex_init(&philos[i].fork_lock, NULL))
		// 	error_fatal("mutex init fork lock", data);
int main(int argc, char* argv[]) {
    pthread_t p1, p2, p3, p4;
	pthread_mutex_t lock;

	pthread_mutex_init(&lock, NULL);

    if (pthread_create(&p1, NULL, routine, &lock) != 0) {
        return 1;
    }
    if (pthread_create(&p2, NULL, routine, &lock) != 0) {
        return 2;
    }
    if (pthread_join(p1, NULL) != 0) {
        return 3;
    }
    if (pthread_join(p2, NULL) != 0) {
        return 4;
    }
    printf("Number of mails: %d\n", mails);
    return 0;
}

