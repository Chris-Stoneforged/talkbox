#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "daemonize.h"

void daemonize(void (*process)(void), const char *pid_path, const char *log_path) {
	pid_t pid, sid, cid;
	int fd, log_fd, null_fd;
	FILE *pid_file;

	if ((pid_file = fopen(pid_path, "wx")) == NULL) {
		perror("Daemon already exists");
		return;
	}

	cid = fork();
	if (cid < 0)
		goto on_error;
	if (cid > 0) {
		fclose(pid_file);
		return;
	} 

	if ((sid = setsid()) < 0)
		goto on_error;	

	pid = fork();
	if (pid < 0)
		goto on_error;
	if (pid > 0) {
		// As the parent, write the child ID to the pid file
		fprintf(pid_file, "%d\n", pid);
		fflush(pid_file);
		fclose(pid_file);
		return;
	}

	umask(0);
	chdir("/");

	log_fd = open(log_path, O_WRONLY | O_CREAT | O_APPEND, 0664);
	null_fd = open("/dev/null", O_RDONLY);
	if (log_fd < 0 || null_fd < 0) 
		goto on_error;

	for (fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
		if (fd != log_fd && fd != null_fd)
			close(fd);
	}

	dup2(log_fd, STDOUT_FILENO);
	dup2(log_fd, STDERR_FILENO);
	close(log_fd);	

	dup2(null_fd, STDIN_FILENO);
	close(null_fd);

	process();
	_exit(0);

on_error:
	fclose(pid_file);
	unlink(pid_path);
	perror("Failed to start daemon");
}

pid_t get_daemon_pid(const char *pid_path) {
	FILE *pid_file;
	char buffer[32];

	if ((pid_file = fopen(pid_path, "r")) == NULL) {
		perror("File does not exist");
		return -1;
	}

	if (!fgets(buffer, sizeof(buffer), pid_file)) {
		perror("Could not read file");
		fclose(pid_file);
		return -1;
	}
	
	fclose(pid_file);
	buffer[strcspn(buffer, "\n")] = '\0';

	char *end = NULL;
	long pid = strtol(buffer, &end, 10);

	if (end == buffer || *end != '\0') {
		fprintf(stderr, "Invalid file contents at %s\n", pid_path);
		return -1;
	} 
     
	if (pid <= 1 || pid > INTMAX_MAX) {
		fprintf(stderr, "Invalid PID for daemon");
		return -1;
	}

	return (pid_t)pid;
}
