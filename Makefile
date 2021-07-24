GCC = g++
CFLAGS = -Wall -I lib/ -I src/
LDFLAGS =

PROGRAM = engine.a

ODIR = obj
BIN = bin
IDIR = src
SDIR = as

LIB = lib
LIBS = -lGL -lglfw -lGLU -lGLEW -lassimp -lpthread lib/imgui.a lib/imgui_editor.a


OUT = $(BIN)/$(PROGRAM)

release: CFLAGS += -O3 -msse4 -mavx2
release: LDFLAGS += -flto
release: all

debug: CFLAGS += -g
debug: all

all: $(IDIR) $(ODIR) $(BIN) $(LIBS) $(OUT)

IMGUI_OBJS= lib/imgui/imgui_draw.o \
			          lib/imgui/imgui.o \
					  lib/imgui/imgui_tables.o \
					  lib/imgui/imgui_widgets.o \
					  lib/imgui/imgui_impl_glfw.o \
					  lib/imgui/imgui_impl_opengl3.o

lib/imgui.a: $(IMGUI_OBJS)
	ar rvs $@ $^

lib/imgui/%.o: lib/imgui/%.cpp
	$(GCC) $(CFLAGS) -c $^ -o $@

lib/imgui_editor.a: lib/ImGuiColorTextEdit/TextEditor.o
	ar rvs $@ $^

lib/ImGuiColorTextEdit/%.o: lib/ImGuiColorTextEdit/%.cpp
	$(GCC) $(CFLAGS) -c $^ -o $@

$(ODIR):
	mkdir $(ODIR)
$(BIN):
	mkdir $(BIN)
$(IDIR):
	mkdir $(IDIR)
$(SDIR):
	mkdir $(SDIR)

C_SOURCES = $(shell find $(IDIR) -type f -name *.cpp -printf "%f\n")
OBJECTS = $(patsubst %.cpp, $(ODIR)/%.o,$(C_SOURCES))
S_CODE = $(patsubst %.cpp, $(SDIR)/%.s,$(C_SOURCES))

$(ODIR)/%.o : $(IDIR)/%.cpp
	$(GCC) $(CFLAGS) -c $^ -o $@

$(SDIR)/%.s : $(IDIR)/%.cpp
	$(GCC) -g -o $@ $(CFLAGS) -S $^

$(ODIR)/%.o : $(IDIR)/**/%.cpp
	$(GCC) $(CFLAGS) -c $^ -o $@

$(SDIR)/%.s : $(IDIR)/**/%.cpp
	$(GCC) -g -o $@ $(CFLAGS) -S $^


$(ODIR)/%.o : $(IDIR)/**/**/%.cpp
	$(GCC) $(CFLAGS) -c $^ -o $@

$(SDIR)/%.s : $(IDIR)/**/**/%.cpp
	$(GCC) -g -o $@ $(CFLAGS) -S $^

$(OUT): $(OBJECTS) $(LIBS)
	ar rvs $@ $^
	
clean: $(ODIR) $(BIN)
	rm -rf $(ODIR)
	rm -rf $(BIN)
	rm -rf $(SDIR)

clean-all: clean
	rm -rf lib/imgui/*.o

clean-dis: $(SDIR)
	rm -rf $(SDIR)

dis: all $(SDIR) $(S_CODE)
