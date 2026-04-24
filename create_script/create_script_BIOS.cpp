// arch-bootstrap - Arch Linux installer
// Copyright (C) 2026 rei-root
// This program is free software under the GNU GPL v3 license.
// See LICENSE file for details.

// This module was generated with the assistance of AI (Gemini).
// Adapted for BIOS/MBR partitioning logic.


#include "create_script.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

static void clear_buffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void create_script_BIOS() {
    // ======================== Time Zone Settings ========================
    std::string region, city;

    std::cout << "--- Timezone Setup ---" << std::endl;
    system("ls /usr/share/zoneinfo");
    std::cout << "Your Region (default Europe): ";
    if(!(std::cin >> region)) region = "Europe";

    system(("ls /usr/share/zoneinfo/" + region).c_str());
    std::cout << "Your City (default Moscow): ";
    if(!(std::cin >> city)) city = "Moscow";

    std::string timezone = region + "/" + city;
    clear_buffer();

    // ====================== Host Name & User ======================
    std::string host_name, username;
    std::cout << "Your PC Host Name (default arch-pc): ";
    if (!(std::cin >> host_name)) host_name = "arch-pc";
    clear_buffer();

    std::cout << "Enter Your username (default user): ";
    if (!(std::cin >> username)) username = "user";
    clear_buffer();

    // ========================== Passwords ==========================
    std::string root_password, user_password;
    std::cout << "Your root password (default 1): ";
    if (!(std::cin >> root_password)) root_password = "1";
    clear_buffer();

    std::cout << "Your user password (default 1): ";
    if (!(std::cin >> user_password)) user_password = "1";
    clear_buffer();
    
    // ========================== Disk for GRUB ==========================
        std::string target_disk;
        system("lsblk -d -n -o NAME,SIZE,MODEL");
        std::cout << "\nEnter the disk for GRUB installation (e.g., /dev/sda): ";
        std::cin >> target_disk;
        clear_buffer();

    // ========================== GPU Selection ==========================
    int gpu_choice = 0;
    while (gpu_choice < 1 || gpu_choice > 3) {
        std::cout << "\nSelect GPU:\n"
                     "1. AMD\n"
                     "2. Intel\n"
                     "3. NVIDIA\n"
                     "Choice: ";
        if (!(std::cin >> gpu_choice)) {
            std::cin.clear();
            clear_buffer();
        }
    }

    // ========================== Desktop Environment ==========================
    int ui_choice = 0;
    while (ui_choice < 1 || ui_choice > 9) {
        std::cout << "\n--- DE Selection ---\n"
                     "1. KDE\n"
                     "2. GNOME\n"
                     "3. XFCE\n"
                     "4. Cinnamon\n"
                     "5. MATE\n"
                     "\n--- WM Selection ---\n"
                     "6. i3\n"
                     "7. Sway\n"
                     "8. Hyprland\n"
                     "\n--- None DE/WM ---\n"
                     "9. Without DE/WM\n"
                     "Choice: ";
        if (!(std::cin >> ui_choice)) {
            std::cin.clear();
            clear_buffer();
        }
    }

    // ========================== Create install.sh ==========================
    std::ofstream installer("/mnt/install.sh");
    if (!installer.is_open()) {
        std::cerr << "Error: Could not create /mnt/install.sh" << std::endl;
        return;
    }

    installer << "#!/bin/bash\n"
              << "ln -sf /usr/share/zoneinfo/" << timezone << " /etc/localtime\n"
              << "hwclock --systohc\n"
              << "sed -i '/^#en_US.UTF-8 UTF-8/s/^#//' /etc/locale.gen\n"
              << "sed -i '/^#ru_RU.UTF-8 UTF-8/s/^#//' /etc/locale.gen\n"
              << "locale-gen\n"
              << "echo \"LANG=en_US.UTF-8\" > /etc/locale.conf\n"
              << "echo \"" << host_name << "\" > /etc/hostname\n"
              << "cat <<EOF > /etc/hosts\n"
              << "127.0.0.1   localhost\n"
              << "::1         localhost\n"
              << "127.0.1.1   " << host_name << ".localdomain " << host_name << "\n"
              << "EOF\n"
              << "echo \"root:" << root_password << "\" | chpasswd\n"
              << "useradd -m -G wheel -s /bin/bash " << username << "\n"
              << "echo \"" << username << ":" << user_password << "\" | chpasswd\n"
              << "sed -i 's/^# %wheel ALL=(ALL:ALL) ALL/%wheel ALL=(ALL:ALL) ALL/' /etc/sudoers\n"
              << "pacman -S --noconfirm grub networkmanager\n"; // Удален efibootmgr

    if (gpu_choice == 1) installer << "pacman -S --noconfirm xf86-video-amdgpu\n";
    else if (gpu_choice == 2) installer << "pacman -S --noconfirm xf86-video-intel\n";
    else if (gpu_choice == 3) installer << "pacman -S --noconfirm nvidia nvidia-utils\n";

    installer << "pacman -S --noconfirm xorg xorg-server\n";

    switch(ui_choice) {
        case 1: installer << "pacman -S --noconfirm plasma-desktop kde-applications sddm\nsystemctl enable sddm\n"; break;
        case 2: installer << "pacman -S --noconfirm gnome gnome-extra gdm\nsystemctl enable gdm\n"; break;
        case 3: installer << "pacman -S --noconfirm xfce4 xfce4-goodies sddm\nsystemctl enable sddm\n"; break;
        case 4: installer << "pacman -S --noconfirm cinnamon sddm\nsystemctl enable sddm\n"; break;
        case 5: installer << "pacman -S --noconfirm mate mate-extra sddm\nsystemctl enable sddm\n"; break;
        case 6: installer << "pacman -S --noconfirm i3-wm i3status i3lock sddm\nsystemctl enable sddm\n"; break;
        case 7: installer << "pacman -S --noconfirm sway waybar swaylock foot\n"; break;
        case 8: installer << "pacman -S --noconfirm hyprland waybar kitty rofi-wayland\n"; break;
    }

    // BIOS-specific GRUB installation
    installer << "grub-install --target=i386-pc " << target_disk << "\n"
              << "grub-mkconfig -o /boot/grub/grub.cfg\n"
              << "systemctl enable NetworkManager\n"
              << "exit";

    installer.close();

    system("chmod +x /mnt/install.sh");
    system("arch-chroot /mnt /bin/bash /install.sh");

    std::cout << "Installer completed its work!\n" <<
                 "You can reboot with command (reboot)" << std::endl;
}
