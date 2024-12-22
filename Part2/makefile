# Compiler and Flags
CC       = gcc
CFLAGS   = -g -Wall
LDFLAGS  = -lzmq -lncurses -lpthread

# Directories
SERVER_DIR          = Server
UTILS_DIR           = Utils
DISPLAY_CLIENT_DIR  = Display-client

# Output Directories
SERVER_OBJ_DIR      = $(SERVER_DIR)/Compiled
UTILS_OBJ_DIR       = $(UTILS_DIR)/Compiled
DISPLAY_CLIENT_OBJ  = $(DISPLAY_CLIENT_DIR)/Compiled

# Targets
all: game-server astronaut-display-client

game-server: \
	$(SERVER_OBJ_DIR)/game-server.o \
	$(UTILS_OBJ_DIR)/communication.o \
	$(SERVER_OBJ_DIR)/space_ship.o \
	$(UTILS_OBJ_DIR)/Utils.o \
	$(SERVER_OBJ_DIR)/space_aliens.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

astronaut-display-client: \
    $(DISPLAY_CLIENT_OBJ)/astronaut-display-client.o \
    $(UTILS_OBJ_DIR)/communication.o \
    $(UTILS_OBJ_DIR)/Utils.o \
    $(SERVER_OBJ_DIR)/space_ship.o \
	$(DISPLAY_CLIENT_OBJ)/outer-space-display.o \
	$(DISPLAY_CLIENT_OBJ)/astronaut-client.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Pattern Rules for .o files
$(SERVER_OBJ_DIR)/%.o: $(SERVER_DIR)/%.c
	mkdir -p $(SERVER_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(UTILS_OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	mkdir -p $(UTILS_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(DISPLAY_CLIENT_OBJ)/%.o: $(DISPLAY_CLIENT_DIR)/%.c
	mkdir -p $(DISPLAY_CLIENT_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Target
clean:
	rm -f \
	$(SERVER_OBJ_DIR)/*.o \
	$(UTILS_OBJ_DIR)/*.o \
	$(DISPLAY_CLIENT_OBJ)/*.o \
	game-server \
	astronaut-display-client