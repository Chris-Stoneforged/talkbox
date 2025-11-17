#ifndef DAEMONIZE_H
#define DAEMONIZE_H

#include <sys/types.h> 

void daemonize(void (*process)(void), const char *pid_path, const char *log_path);
pid_t get_daemon_pid(const char *pid_path);
int kill_daemon(const char *pid_path);

#endif
