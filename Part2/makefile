# Compiler and Flags
CC       = gcc
CFLAGS   = -O3 -Wall
LDFLAGS  = -lzmq -lncurses -lpthread

# Directories
SERVER_DIR          = Server
CLIENT_DIR          = Client
UTILS_DIR           = Utils
DISPLAY_CLIENT_DIR  = Display-client
REMOTE_DISPLAY_DIR  = Remote-Display-Client

# Output Directories
SERVER_OBJ_DIR      = $(SERVER_DIR)/Compiled
CLIENT_OBJ_DIR      = $(CLIENT_DIR)/Compiled
UTILS_OBJ_DIR       = $(UTILS_DIR)/Compiled
DISPLAY_CLIENT_OBJ  = $(DISPLAY_CLIENT_DIR)/Compiled
REMOTE_DISPLAY_OBJ  = $(REMOTE_DISPLAY_DIR)/Compiled

# Targets
all: game-server astronaut-client outer-space-display astronaut-display-client

game-server: \
	$(SERVER_OBJ_DIR)/game-server.o \
	$(UTILS_OBJ_DIR)/communication.o \
	$(SERVER_OBJ_DIR)/space_ship.o \
	$(UTILS_OBJ_DIR)/Utils.o \
	$(SERVER_OBJ_DIR)/space_aliens.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

astronaut-client: \
	$(CLIENT_OBJ_DIR)/astronaut-client-main.o \
	$(CLIENT_OBJ_DIR)/astronaut-client-lib.o \
	$(UTILS_OBJ_DIR)/communication.o \
	$(UTILS_OBJ_DIR)/Utils.o
	$(CC) $(CFLAGS) $^ -o $@ -lzmq -lncurses

outer-space-display: \
  $(REMOTE_DISPLAY_OBJ)/outer-space-display-main.o \
  $(REMOTE_DISPLAY_OBJ)/outer-space-display-lib.o \
  $(UTILS_OBJ_DIR)/communication.o \
  $(SERVER_OBJ_DIR)/space_ship.o \
  $(SERVER_OBJ_DIR)/space_aliens.o \
  $(UTILS_OBJ_DIR)/Utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

astronaut-display-client: \
    $(DISPLAY_CLIENT_OBJ)/astronaut-display-client.o \
    $(UTILS_OBJ_DIR)/communication.o \
    $(UTILS_OBJ_DIR)/Utils.o \
    $(SERVER_OBJ_DIR)/space_ship.o \
	$(REMOTE_DISPLAY_OBJ)/outer-space-display-lib.o \
	$(CLIENT_OBJ_DIR)/astronaut-client-lib.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Pattern Rules for .o files
$(SERVER_OBJ_DIR)/%.o: $(SERVER_DIR)/%.c
	mkdir -p $(SERVER_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ_DIR)/%.o: $(CLIENT_DIR)/%.c
	mkdir -p $(CLIENT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(UTILS_OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	mkdir -p $(UTILS_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(DISPLAY_CLIENT_OBJ)/%.o: $(DISPLAY_CLIENT_DIR)/%.c
	mkdir -p $(DISPLAY_CLIENT_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(REMOTE_DISPLAY_OBJ)/%.o: $(REMOTE_DISPLAY_DIR)/%.c
	mkdir -p $(REMOTE_DISPLAY_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(REMOTE_DISPLAY_OBJ)/outer-space-display-main.o: \
	$(REMOTE_DISPLAY_DIR)/outer-space-display-main.c \
	$(REMOTE_DISPLAY_DIR)/outer-space-display.h
	mkdir -p $(REMOTE_DISPLAY_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(REMOTE_DISPLAY_OBJ)/outer-space-display-lib.o: \
	$(REMOTE_DISPLAY_DIR)/outer-space-display-lib.c \
	$(REMOTE_DISPLAY_DIR)/outer-space-display.h
	mkdir -p $(REMOTE_DISPLAY_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(CLIENT_OBJ_DIR)/astronaut-client-main.o: \
    $(CLIENT_DIR)/astronaut-client-main.c \
    $(CLIENT_DIR)/astronaut-client.h
	mkdir -p $(CLIENT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ_DIR)/astronaut-client-lib.o: \
    $(CLIENT_DIR)/astronaut-client-lib.c \
    $(CLIENT_DIR)/astronaut-client.h
	mkdir -p $(CLIENT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Target
clean:
	rm -f \
	$(SERVER_OBJ_DIR)/*.o \
	$(CLIENT_OBJ_DIR)/*.o \
	$(UTILS_OBJ_DIR)/*.o \
	$(DISPLAY_CLIENT_OBJ)/*.o \
	$(REMOTE_DISPLAY_OBJ)/*.o \
	game-server \
	astronaut-client \
	outer-space-display \
	astronaut-display-client