# Newgoat bootup process
1. Newgoat gains execution
2. Newgoat initializes the HAL
3. Newgoat initializes the object manager
4. Newgoat loads all drivers and firmware from the driver ramfs making copies of them
5. Newgoat initializes all drivers
6. Newgoat frees all initalizing memory including the originals of the drivers 
7. Newgoat initializes the scheduler
8. Newgoat sets up the asr32 startup code
9. Newgoat starts the scheduler
10. Goat64 starts all startup processes from the registery