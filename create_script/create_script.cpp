
// arch-bootstrap - Arch Linux installer
// Copyright (C) 2026 rei-root
// This program is free software under the GNU GPL v3 license.
// See LICENSE file for details.

#include "create_script.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

void clear_buffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void create_script() {
    // ======================== Time Zone Settings ========================
    std::string region, city;

    std::cout << "--- Timezone Setup ---" << std::endl;
    system("ls /usr/share/zoneinfo");
    std::cout << "Your Region (default Europe): ";
    if(!(std::cin >> region)){
        region = "Europe";
    }

    system(("ls /usr/share/zoneinfo/" + region).c_str());
    std::cout << "Your City (default Moscow): ";
    if(!(std::cin >> city)){
        city = "Moscow";
    }

    std::string timezone = region + "/" + city;
    clear_buffer();

    // ====================== Host Name ======================
    std::string host_name;
    std::cout << "Your PC Host Name (default arch-pc): ";
    std::getline(std::cin, host_name);
    if (host_name.empty()) host_name = "arch-pc";

    // ========================== UserName ==========================
    std::string username;
    std::cout << "Enter Your username (default user): ";
    std::getline(std::cin, username);
    if (username.empty()) username = "user";

    // ========================== Passwords ==========================
    std::string root_password, user_password;
    std::cout << "Your root password (default 1): ";
    std::getline(std::cin, root_password);
    if (root_password.empty()) root_password = "1";

    std::cout << "Your user password (default 1): ";
    std::getline(std::cin, user_password);
    if (user_password.empty()) user_password = "1";

    // ========================== GPU Selection ==========================
    int gpu_choice = 0;
    while (gpu_choice < 1 || gpu_choice > 3) {
        std::cout << "\nSelect GPU:\n1. AMD\n2. Intel\n3. NVIDIA\nChoice: ";
        if (!(std::cin >> gpu_choice)) {
            std::cin.clear();
            clear_buffer();
        }
    }

    // ========================== Desktop Environment ==========================
    int ui_choice = 0;
    while (ui_choice < 1 || ui_choice > 8) {
        std::cout << "\n--- DE/WM ---\n1. KDE\n2. GNOME\n3. XFCE\n4. Cinnamon\n5. MATE\n6. i3\n7. Sway\n8. Hyprland\nChoice: ";
        if (!(std::cin >> ui_choice)) {
            std::cin.clear();
            clear_buffer();
        }
    }

    // ========================== Create install.sh ==========================
    std::ofstream installer("/mnt/install.sh");
    if (!installer.is_open()) {
        std::cerr << "Error: Could not create /mnt/install.sh. Check permissions or /mnt mount." << std::endl;
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
              << "pacman -S --noconfirm grub efibootmgr networkmanager\n";

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

    installer << "grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB\n"
              << "grub-mkconfig -o /boot/grub/grub.cfg\n"
              << "systemctl enable NetworkManager\n"
              << "exit";

    installer.close();

    system("chmod +x /mnt/install.sh");

    system("arch-chroot /mnt /bin/bash /install.sh");

    std::cout << "Installer completed its work!\n" <<
                 "You can reboot with command (reboot)" << std::endl;
}
