# Filenames
EXECUTABLE = media_player
ICON = icons/$(EXECUTABLE).png
PIXMAPS = icons/next.xpm \
          icons/play_pause.xpm \
          icons/previous.xpm \
          icons/stop.xpm

# Install variables
INSTALL = /usr/bin/install
OWNER = root
GROUP = root
BIN_DIR = /usr/local/bin
ICON_DIR = /usr/local/share/pixmaps
PIXMAPS_DIR = $(ICON_DIR)/$(EXECUTABLE)

# C++ compiler
CXX = g++

# Compile flags
CXXFLAGS = -Wall -W -g -c \
           `pkg-config --cflags gtkmm-2.4 gstreamer-0.10` \
           `taglib-config --cflags`

# Linker flags
LDFLAGS = `pkg-config --libs gtkmm-2.4 gstreamer-0.10` `taglib-config --libs`

# Source files
SOURCES = media_player.cc \
          library/Album.cc \
          library/Artist.cc \
          library/Library.cc \
          library/LibraryFileBuilder.cc \
          library/Song.cc \
          gui/MainWindow.cc \
          gui/TreeView.cc \
          gui/SonglistView.cc \
          gui/PlaylistView.cc \
          gui/ModelColumns.cc \
          gui/TreeColumn.cc \
          player/Player.cc \
          config/Configurations.cc

# Header files
HEADERS = library/Album.h \
          library/Artist.h \
          library/LibraryFileBuilder.h \
          library/Library.h \
          library/Song.h \
          gui/MainWindow.h \
          gui/TreeView.h \
          gui/SonglistView.h \
          gui/PlaylistView.h \
          gui/ModelColumns.h \
          gui/TreeColumn.h \
          player/Player.h \
          config/Configurations.h

# Object files (from names of source files)
OBJS = $(SOURCES:.cc=.o)

###
# Targets
###

all: $(SOURCES) $(HEADERS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@
	
.cc.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm -f *.o */*.o $(EXECUTABLE)

install: $(EXECUTABLE) $(ICON) $(PIXMAPS)
	$(INSTALL) -c -o $(OWNER) -g $(GROUP) -m 755 -d $(BIN_DIR) $(PIXMAPS_DIR)
	$(INSTALL) -c -o $(OWNER) -g $(GROUP) -m 755 $(EXECUTABLE) $(BIN_DIR)
	$(INSTALL) -c -o $(OWNER) -g $(GROUP) -m 644 $(ICON) $(ICON_DIR)
	$(INSTALL) -c -o $(OWNER) -g $(GROUP) -m 644 $(PIXMAPS) $(PIXMAPS_DIR)

new: $(EXECUTABLE)
	$(INSTALL) -c -o $(OWNER) -g $(GROUP) -m 755 $(EXECUTABLE) $(BIN_DIR)
