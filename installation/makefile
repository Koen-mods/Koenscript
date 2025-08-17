CC = gcc
CFLAGS = -Wall -O2
TARGET = koenscript
INSTALL_DIR = "C:\Program Files\koenscript"

all: $(TARGET)

$(TARGET): koenscript.c
	$(CC) $(CFLAGS) -o $(TARGET).exe koenscript.c

install: $(TARGET)
	if not exist $(INSTALL_DIR) mkdir $(INSTALL_DIR)
	copy $(TARGET).exe $(INSTALL_DIR)

clean:
	del $(TARGET).exe