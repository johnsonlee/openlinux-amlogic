#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define PLAYER_RESPONSE_MSG "/tmp/player_response"

void main(void)
{
	int socket_fd = -1;
	struct sockaddr_un name;

	socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		printf("daemon_palyer socket creation failed\n");
		return -1;
	}

	unlink(PLAYER_RESPONSE_MSG);

	memset(&name,0,sizeof(name));
	name.sun_family = AF_LOCAL;
	strncpy(name.sun_path, PLAYER_RESPONSE_MSG, sizeof(name.sun_path) - 1);
	if (bind(socket_fd, (struct sockaddr *)&name, SUN_LEN(&name)) < 0) {
		printf("daemon_player socket bind failed \n");
		return -1;
	}

	listen(socket_fd, 5);
	while(1) {
		struct sockaddr_un client_name;
    socklen_t client_name_len;
    int client_socket_fd;
    int len = 0;
    char *msg = NULL;

		client_socket_fd = accept(socket_fd, (struct sockaddr *)&client_name, &client_name_len);
		read(client_socket_fd, &len, sizeof(len));
		msg = (char *)malloc(len);
		if(msg){
			read(client_socket_fd, msg, len);
			printf("%s\n",msg);
			free(msg);
		}
		close(client_socket_fd);
	}
}