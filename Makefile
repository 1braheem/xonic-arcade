PROJECT_DIR := Final_Project/Final_Project
SFML_DIR := SFML
TARGET := $(PROJECT_DIR)/dsa_project

CXX := clang++
CXXFLAGS := -std=c++17 -I$(PROJECT_DIR) -I$(SFML_DIR)/include
LDFLAGS := -L$(SFML_DIR)/lib \
	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
	-Wl,-rpath,@loader_path/../../$(SFML_DIR)/lib

.PHONY: all setup-mac run clean

all: $(TARGET)

setup-mac:
	xattr -dr com.apple.quarantine $(SFML_DIR) 2>/dev/null || true
	for framework in $(SFML_DIR)/extlibs/*.framework; do \
		ln -sfn ../extlibs/$$(basename "$$framework") $(SFML_DIR)/Frameworks/$$(basename "$$framework"); \
	done
	codesign --force --deep --sign - $(SFML_DIR)/lib/*.dylib
	codesign --force --deep --sign - $(SFML_DIR)/extlibs/*.framework
	codesign --force --deep --sign - $(SFML_DIR)/Frameworks/*.framework

$(TARGET): $(PROJECT_DIR)/main.cpp $(PROJECT_DIR)/*.h
	$(CXX) $(CXXFLAGS) $(PROJECT_DIR)/main.cpp $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	cd $(PROJECT_DIR) && ALSOFT_LOGLEVEL=0 ./dsa_project

clean:
	rm -f $(TARGET)
