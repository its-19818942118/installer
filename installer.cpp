#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

static int s_allocount = 0;
#ifdef DEBUG
#undef DEBUG
    
    void* operator new(size_t size) {
        s_allocount++;
        void* ptr = std::malloc(size);
        if (!ptr) {
            throw std::bad_alloc(); // Handle allocation failure
        }
        std::cout << "Allocated " << size << " bytes. Total allocations: " << s_allocount << std::endl;
        return ptr;
    }

    // Overriding global delete operator
    void operator delete(void* ptr) noexcept {
        std::free(ptr);
        std::cout << "Deallocated memory." << std::endl;
    }
    
#endif /* DEBUG */

namespace
    pkgString
{
    
    const std::string
        cmdAur = "paru -Syu --needed",
        cmdPac = "sudo pacman -Syu --needed",
        pkgAur = " metar wlogout gradience apple-fonts matugen-bin ttf-rubik-vf python-yapsy ttf-readex-pro ttf-gabarito-git adw-gtk-theme-git ttf-noto-sans-cjk-vf ttf-material-symbols-variable-git ",
        pkgPac = " bc jq gjs npm git pam yad axel curl wget swww gvfs gtk3 fish foot grim cmake meson rsync glib2 glibc qt5ct slurp upower gtkmm3 swappy ddcutil ripgrep ydotool cairomm ghostty hyprland cliphist libpulse libsoup3 tinyxml2 hyprlock hypridle starship coreutils playerctl gammastep libnotify blueberry tesseract dart-sass hyprutils typescript fontconfig hyprpicker glib2-devel wf-recorder wireplumber qt5-wayland pavucontrol rofi-wayland polkit-gnome python-build wl-clipboard python-wheel python-psutil brightnessctl xdg-user-dirs gnome-keyring python-pillow networkmanager gtksourceview3 python-libsass gtk-layer-shell gtksourceviewmm libdbusmenu-gtk3 python-pywayland noto-fonts-emoji xdg-user-dirs-gtk webp-pixbuf-loader xdg-desktop-portal tesseract-data-eng gnome-bluetooth-3.0 gnome-control-center gobject-introspection power-profiles-daemon python-setuptools-scm xdg-desktop-portal-gtk xdg-desktop-portal-hyprland ",
        paru = "sudo pacman -S --needed git base-devel && mkdir -p /tmp/paru && cd /tmp/paru && git clone https://aur.archlinux.org/paru.git && cd paru && makepkg -si && rm -rf /tmp/paru",
        yay = "sudo pacman -S --needed git base-devel && mkdir -p /tmp/yay && cd /tmp/yay && git clone https://aur.archlinux.org/yay.git && cd yay && makepkg -si && rm -rf /tmp/yay",
        agsV1 = "mkdir -p /tmp/agsv1 && cd /tmp/agsv1 && git clone https://github.com/Lunaris-Project/agsv1 && ",
        hyprLunaRepo = "https://github.com/Lunaris-Project/HyprLuna.git",
        // hyprlunaDir = std::string(getenv("$HOME")) + "/.config/HyprLuna",
        // backDir = std::string(getenv("$HOME")) + "/hyprLuna-user.bak",
        cmdPacPkg = cmdPac + pkgPac,
        cmdAurPKg = cmdAur + pkgAur
    ;
    
    
} /* namespace pkgString */

void executeWithRetry(const std::string& command, int maxRetries) {
    int attempts = 0;
    int result;
    while (attempts < maxRetries) {
        result = system(command.c_str());
        if (result == 0) {
            return; // Command succeeded
        }
        attempts++;
        std::cerr << "Error: Command failed with return code: " << result << ". Attempts left: " << (maxRetries - attempts) << std::endl;
        if (attempts < maxRetries) {
            std::cout << "Would you like to retry? (Y/N): ";
            std::string retryInput;
            std::getline(std::cin, retryInput);
            if (retryInput != "y" && retryInput != "Y") {
                std::cout << "Exiting after failed attempts." << std::endl;
                return; // Exit if user chooses not to retry
            }
        }
    }
    std::cerr << "Error: Maximum attempts reached. Command failed." << std::endl;
}

std::string
    packagesString
    ( std::string packagetype , bool nc , std::optional<std::string> helperType = std::nullopt )
{
    
    
    if
        ( packagetype == "pacman" && nc == false )
    {
        return pkgString::cmdPacPkg;
    }
    
    if
        ( packagetype == "pacman" && nc == true )
    {
        return pkgString::cmdPacPkg + "--no-confirm";
    }
    
    else if
        ( packagetype == "aur" && nc == false )
    {
        return /* *helperType + */ pkgString::cmdAurPKg;
    }
    
    else if
        ( packagetype == "aur" && nc == true )
    {
        return /* *helperType + */ pkgString::cmdAurPKg + "--no-confirm";
    }
    
    return "null";
    
}

void
    installDots
    ( )
{
    
}

#include <iostream>
#include <limits> // For std::numeric_limits

#include <iostream>
#include <limits> // For std::numeric_limits

#include <iostream>
#include <limits> // For std::numeric_limits

void installHyprLuna(bool nc) {
    std::cout
        << ":: INFO: Install the following Dependencies Required for HyprLuna? [Yy/Nn]\n" << "  >> ";
    std::string confirmInput , helperType;
    int falseCount = 0;

    while (falseCount < 3) {
        std::getline(std::cin, confirmInput);

        if (confirmInput == "y" || confirmInput == "Y") {
            int aurHelperInput = -1; // Initialize to an invalid value
            falseCount = 0; // Reset falseCount for AUR helper selection

            while (falseCount < 3) {
                std::cout << ":: Please choose an Aur Helper: [0/1/2]\n" 
                          << ":: 0. Default (Yay)\n"
                          << ":: 1. Yay\n" 
                          << ":: 2. Paru\n" 
                          << "  >> ";
                
                // Attempt to read aurHelperInput
                std::cin >> aurHelperInput;

                // Check if the input was valid
                if (std::cin.fail()) {
                    std::cin.clear(); // Clear the fail state
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
                    std::cerr << ":: WARN: Invalid input! Please select 0, 1, or 2." << std::endl;
                    falseCount++; // Increment falseCount for invalid input
                    if (falseCount >= 3) {
                        std::cout << ":: Error: 3 failed attempts for Aur Helper input\n" << ":: Exiting...\n";
                        return; // Exit the function after 3 failed attempts
                    }
                    else
                    continue; // Continue to the next iteration of the loop
                }

                // Clear the newline character from the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Check for valid input
                if (aurHelperInput < 0 || aurHelperInput > 2 ) {
                    std::cerr << ":: WARN: Invalid input! Please select 0, 1, or 2." << std::endl;
                    falseCount++; // Increment falseCount for invalid input
                } else {
                    break; // Valid input, exit the loop
                }
            }

            // Execute the selected AUR helper installation with retry
            if (aurHelperInput == 0 || aurHelperInput == 1) {
                std::cout << "Selected AUR Helper: Yay" << std::endl
                // <<  ":: INFO: Beggining installation proccess for AUR Helper: Yay"
                ;
                
                helperType = "yay ";
                executeWithRetry(packagesString("aur", nc , helperType ), 3);
                break;
                
            } else if (aurHelperInput == 2) {
                // Call your function to execute with Paru
                std::cout << "Selected AUR Helper: Paru" << std::endl
                //   << ":: INFO: Beggining installation proccess for AUR Helper: Paru"
                ;
                helperType = "paru ";
                executeWithRetry(packagesString("aur", nc , helperType ), 3);
                break;
            }
            
            
            // Proceed to install Pacman packages with retry
            executeWithRetry(packagesString("pacman", nc), 3);

            // Proceed to install AUR packages with retry

            return; // Exit the function after successful installation
        } else if (confirmInput == "n" || confirmInput == "N") {
            std::cout << ":: Exiting without installation...\n";
            return; // Exit the function
        } else {
            falseCount++;
            std::cout << ":: WARN: Invalid input. Please enter 'Y' or 'N'. Attempts left: " << (3 - falseCount) << "\n>> ";
        }
    }

    // std::cout << "\r:: Error: 3 failed attempts for confirmation input\n" << ":: Exiting...\n";
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return ""; // No content
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string getOSType() {
    std::ifstream osType("/etc/os-release");
    if (!osType) {
        return ":: ERROR: Unable to open /etc/os-release\n";
    }
    
    std::string line;
    while (std::getline(osType, line)) {
        if (line.find("ID=") == 0) {
            std::string osID = line.substr(3); // Get everything after "ID="
            osID.erase(std::remove(osID.begin(), osID.end(), '\"'), osID.end()); // Remove quotes if present
            osID = trim(osID); // Trim whitespace

            // Debugging output
            // std::cout << "Detected OS ID: '" << osID << "' (length: " << osID.length() << ")" << std::endl;

            if (osID != "arch") {
                std::cout
                << ":: INFO: THIS INSTALLER IS STRICTLY FOR ARCH BASED DISTROS\n ANYOTHER USES MAY BREAK YOUR SYSTEM: " + line;
                return ":: INFO: THIS INSTALLER IS STRICTLY FOR ARCH BASED DISTROS\n ANYOTHER USES MAY BREAK YOUR SYSTEM: " + line;
            }
            else {
                // return ":: SUCCESS: This installer is meant for Arch-based systems. Detected: " + osID + "\n";
            }
        }
    }
    osType.close();
    return ":: ERROR: ID not found in /etc/os-release\n";
}

void
    execShell
    ( )
{
    
    
    
}

void
    parseCmd
    ( int argc , char* argv [ ] )
{
    
    std::string cmd , helpMsg , errCmd;
    std::vector<std::string> invalidArgs;
    helpMsg =
    ":: INFO: Installer <args>\n"
    ":: INFO: flags <args> are:\n"
    "  -h  | --help : Prints the help message\n"
    "  -i  | --install : Install the HyprLuna dots\n"
    "  -nc | --no-confirm : Install Hyprluna without multiple confirmations!\n"
    "        => will still ask for confirmation when installing the dotfiles\n"
    " :: WARN: the `nc` flag must a combined flag. so `-inc` is valid.\n"
    "    => Using it standalone will produce errors\n"
    ;
    
    
    for
        ( size_t i = 1; i < argc; ++i )
    {
        
        cmd = argv [ i ];
        
        if 
            ( cmd == "-i" || cmd == "--install" )
        {
            std::cout
                <<  ":: INFO: Installing Hyprluna ...\n"
            ;
            
            installHyprLuna (false);
            
        }
        
        else if
            ( cmd == "-inc" || cmd == "--install --no-confirm" )
        {
            std::cout
                <<  ":: INFO: Installing Hyprluna...\n"
            ;
            
            installHyprLuna(true);
            
        }
        
        else if
            ( cmd == "-h" || cmd == "--help" )
        {
            std::cout
                << helpMsg
            ;
        }
        
        else
        {
            errCmd = invalidArgs.emplace_back (cmd);
        }
        
    }
    
    if (!invalidArgs.empty())
    {
        std::cerr << ":: ERROR: Invalid argument/s: ";
        for (const auto& arg : invalidArgs) {
            std::cerr << "`" << arg << "` ";
        }
        std::cerr << "\n" << helpMsg;
    }
    
}

int
    main
    ( int argc , char* argv [ ] )
{
    
    if
        ( argc < 2 )
    {
        
        std::cerr
            <<  ":: Error:\n"
            <<  "  No Arguments passed!\n"
            <<  "  Please provide atleast 1 argument\n"
            <<  "  Use -h | --help to print out a help message"
        ;
        
    }
    
    else
    {
        getOSType();
        parseCmd ( argc , argv );
        
    }
    
    #ifdef DEBUG
        
        std::cout
            <<  "Number of shitty allocations: "
            <<  s_allocount
            <<  '\n'
        ;
        
    #endif /* DEBUG */
    
}
