CXXFlags = -std=c++23

all: installer
clean: clean

installer: installer.cpp
	@echo "Compiling..."
	@echo ":: 10%..."
	@ccache g++ $(CXXFlags) installer.cpp -o installer
	@echo ":: 15%..."
	@ccache g++ $(CXXFlags) installer.cpp -o installerd -o ad.out -DDEBUG
	@echo ":: 45%..."
	@ccache g++ $(CXXFlags) -static installer.cpp -o a.out -o s_installer
	@echo ":: 100%..."
clean:
