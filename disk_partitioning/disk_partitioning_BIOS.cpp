
// arch-bootstrap - Arch Linux installer
// Copyright (C) 2026 rei-root
// This program is free software under the GNU GPL v3 license.
// See LICENSE file for details.

// arch-bootstrap - Arch Linux installer (BIOS/MBR version)
// Copyright (C) 2026 rei-root

// This module was generated with the assistance of AI (Gemini).
// Adapted for BIOS/MBR partitioning logic.


#include "disk_partitioning.hpp"
#include <iostream>
#include <cstdlib>
#include <string>

void disk_partitioning_BIOS() {
    std::string name_disk;
    int root_size = 0, swap_size = 0, boot_size = 512;
    bool use_home = false, use_swap = false;
    std::string input;

    system("lsblk");
    std::cout << "Select disk (for example: /dev/sdb): ";
    std::cin >> name_disk;
    std::cin.ignore(1000, '\n');

    std::cout << "Boot Partition Size (MiB) (default 512): ";
    if (!(std::cin >> boot_size)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        boot_size = 512;
    } else {
        std::cin.ignore(1000, '\n');
    }

    std::cout << "Root size (GB): ";
    if(!(std::cin >> root_size)){
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        root_size = 20;
    } else {
        std::cin.ignore(1000, '\n');
    }

    std::cout << "Use Swap? (y/n): ";
    std::cin >> input;
    std::cin.ignore(1000, '\n');
    use_swap = (input == "y" || input == "Y");

    if (use_swap) {
        std::cout << "Size Swap (GB): ";
        if(!(std::cin >> swap_size)){
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            swap_size = 8;
        } else {
            std::cin.ignore(1000, '\n');
        }
    }

    std::cout << "Use Home? (y/n): ";
    std::cin >> input;
    std::cin.ignore(1000, '\n');
    use_home = (input == "y" || input == "Y");

    // Начальный отступ 1MiB для выравнивания
    long long current_mib = 1;

    // 1. Создание таблицы MBR (msdos)
    system(("parted " + name_disk + " --script mklabel msdos").c_str());

    // 2. Создание Boot раздела (в BIOS он обычно ext4)
    long long boot_end = current_mib + boot_size;
    system(("parted " + name_disk + " --script mkpart primary ext4 "
            + std::to_string(current_mib) + "MiB "
            + std::to_string(boot_end) + "MiB").c_str());
    // Установка флага загрузки для BIOS
    system(("parted " + name_disk + " --script set 1 boot on").c_str());
    current_mib = boot_end;

    // 3. Swap раздел
    if (use_swap) {
        long long swap_end = current_mib + (swap_size * 1024);
        system(("parted " + name_disk + " --script mkpart primary linux-swap "
                + std::to_string(current_mib) + "MiB "
                + std::to_string(swap_end) + "MiB").c_str());
        current_mib = swap_end;
    }

    // 4. Root и Home разделы
    long long root_end = current_mib + (root_size * 1024);
    if (use_home) {
        system(("parted " + name_disk + " --script mkpart primary ext4 "
                + std::to_string(current_mib) + "MiB "
                + std::to_string(root_end) + "MiB").c_str());

        system(("parted " + name_disk + " --script mkpart primary ext4 "
                + std::to_string(root_end) + "MiB 100%").c_str());
    } else {
        system(("parted " + name_disk + " --script mkpart primary ext4 "
                + std::to_string(current_mib) + "MiB 100%").c_str());
    }

    // Форматирование и монтирование
    std::string disk_prefix = (name_disk.find("nvme") != std::string::npos) ? "p" : "";
    std::string boot_dev = name_disk + disk_prefix + "1";
    std::string swap_dev = name_disk + disk_prefix + "2";
    std::string root_dev = name_disk + disk_prefix + (use_swap ? "3" : "2");
    std::string home_dev = name_disk + disk_prefix + (use_swap ? "4" : "3");

    // Форматируем boot в ext4 (в BIOS FAT32 не обязателен)
    system(("mkfs.ext4 " + boot_dev).c_str());

    if (use_swap){
        system(("mkswap " + swap_dev).c_str());
        system(("swapon " + swap_dev).c_str());
    }

    system(("mkfs.ext4 " + root_dev).c_str());

    if (use_home){
        system(("mkfs.ext4 " + home_dev).c_str());
    }

    // Монтирование
    system(("mount " + root_dev + " /mnt").c_str());
    system("mkdir -p /mnt/boot");
    system(("mount " + boot_dev + " /mnt/boot").c_str());

    if(use_home){
        system("mkdir -p /mnt/home");
        system(("mount " + home_dev + " /mnt/home").c_str());
    }
}
