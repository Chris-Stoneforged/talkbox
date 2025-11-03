enum CommandName {
	START_SERVER = 0,
};

typedef struct CommandData {
	char padding[sizeof(char *)]; // Padding
} CommandData;

typedef struct Command {
	enum CommandName cmd_name;
	CommandData *cmd_data;
} Command;

typedef struct StartServer {
	char* port;
} StartServer;
