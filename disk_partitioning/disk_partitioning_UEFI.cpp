
// arch-bootstrap - Arch Linux installer
// Copyright (C) 2026 rei-root
// This program is free software under the GNU GPL v3 license.
// See LICENSE file for details.

#include "disk_partitioning.hpp"
#include <iostream>
#include <cstdlib>
#include <string>


void disk_partitioning_UEFI() {
    std::string name_disk;
    int root_size = 0, home_size = 0, swap_size = 0;
    int efi_size = 512;
    bool use_home = false, use_swap = false;
    std::string input;

// =================================================================================
    system("lsblk");
    std::cout << "Select disk (for example: /dev/sdb): ";
    std::cin >> name_disk;
    std::cin.ignore(1000, '\n');    

    std::cout << "EFI Size (MiB) (default 512, recommended 512-1024): ";
    if (!(std::cin >> efi_size)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        efi_size = 512;
    }else{
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
// =================================================================================

    long long current_mib = 1;

// =================================================================================
    system(("parted " + name_disk + " --script mklabel gpt").c_str());
// =================================================================================

// =================================================================================
    long long efi_end = current_mib + efi_size;
    system(("parted " + name_disk + " --script mkpart ESP fat32 "
            + std::to_string(current_mib) + "MiB "
            + std::to_string(efi_end) + "MiB").c_str());
    system(("parted " + name_disk + " --script set 1 esp on").c_str());
    current_mib = efi_end;
// =================================================================================

// =================================================================================
    if (use_swap) {
        long long swap_end = current_mib + (swap_size * 1024);
        system(("parted " + name_disk + " --script mkpart primary linux-swap "
                + std::to_string(current_mib) + "MiB "
                + std::to_string(swap_end) + "MiB").c_str());
        current_mib = swap_end;
    }
// =================================================================================

// =================================================================================
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
// =================================================================================

// =================================================================================
std::string disk_prefix = (name_disk.find("nvme") != std::string::npos) ? "p" : "";
std::string efi_dev  = name_disk + disk_prefix + "1";
std::string root_dev = name_disk + disk_prefix + (use_swap ? "3" : "2");
std::string home_dev = name_disk + disk_prefix + (use_swap ? "4" : "3");

system(("mkfs.fat -F32 " + efi_dev).c_str());

if (use_swap){
    system(("mkswap " + name_disk + disk_prefix + "2").c_str());
    system(("swapon " + name_disk + disk_prefix + "2").c_str());
}
system(("mkfs.ext4 " + root_dev).c_str());

if (use_home){
    system(("mkfs.ext4 " + home_dev).c_str());
}
// =================================================================================

// =================================================================================
system(("mount " + root_dev + " /mnt").c_str());

if (use_home){
    system("mkdir -p /mnt/boot /mnt/home");
}else{
    system("mkdir -p /mnt/boot");
}

system(("mount " + efi_dev + " /mnt/boot").c_str());

if(use_home){
    system(("mount " + home_dev + " /mnt/home").c_str());
}
// =================================================================================

}
