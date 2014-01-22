//
//  main.c
//  MobileDevice
//
//  Created by zhengzhilin on 14-1-9.
//  Copyright (c) 2014年 zhengzhilin. All rights reserved.
//

#include <stdio.h>
#include "mobiledevice.h"

enum MobileDeviceCommandType
{
    GetUDID,
    InstallApp,
    UninstallApp,
    ListInstalledApps,
    Tunnel
};

struct
{
    struct am_device_notification *notification;
    enum MobileDeviceCommandType type;
    const char *app_path;
    const char *bundle_id;
    int print_paths;
    uint16_t src_port;
    uint16_t dst_port;
} command;

extern void register_device_notification();
extern char *create_cstr_from_cfstring(CFStringRef cfstring);
extern CFStringRef get_bundle_id(const char *app_path);
extern void print_syntax();



int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        print_syntax();
        exit(2);
    }
    
    // get_udid
    if ((argc == 2) && (strcmp(argv[1], "-u") == 0))
    {
        command.type = GetUDID;
    }
    // get_bundle_id
    else if ((argc == 3) && (strcmp(argv[1], "-b") == 0))
    {
        CFStringRef bundle_id = get_bundle_id(argv[2]);
        if (bundle_id == NULL)
        {
            printf("bundle_id is NULL\n");
            exit(1);
        }
        
        char *_bundle_id = create_cstr_from_cfstring(bundle_id);
        CFRelease(bundle_id);
        
        if (_bundle_id == NULL)
        {
            printf("_bundle_id is NULL\n");
            exit(1);
        }
        printf("%s\n", _bundle_id);
        free(_bundle_id);
        exit(0);
    }
    // install_app
    else if ((argc == 3) && (strcmp(argv[1], "-i") == 0))
    {
        command.type = InstallApp;
        command.app_path = argv[2];
    }
    // uninstall_app
    else if ((argc == 3) && (strcmp(argv[1], "-r") == 0))
    {
        command.type = UninstallApp;
        command.bundle_id = argv[2];
    }
    // list_installed_apps
    else if (strcmp(argv[1], "-l") == 0)
    {
        command.type = ListInstalledApps;
        
        if ((argc == 3) && (strcmp(argv[2], "-p") == 0))
        {
            command.print_paths = 1;
        }
        else
        {
            command.print_paths = 0;
        }
    }
    // tunnel
    else if ((argc == 4) && (strcmp(argv[1], "-t") == 0))
    {
        command.type = Tunnel;
        command.src_port = (uint16_t)atoi(argv[2]);
        command.dst_port = (uint16_t)atoi(argv[3]);
    }
    else
    {
        fprintf(stderr, "Unknown command\n");
        print_syntax();
        exit(1);
    }
    
    // wait for device
    register_device_notification();
    return 1;

}

