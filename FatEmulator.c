#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#pragma warning(disable : 4996)

#pragma pack(push, 1)
typedef struct {
    uint8_t  jumpInstruction[3];  // Jump instruction
    char     oemName[8];           // OEM Name
    uint16_t bytesPerSector;       // Bytes per sector
    uint8_t  sectorsPerCluster;    // Sectors per cluster
    uint16_t reservedSectors;      // Reserved sectors
    uint8_t  numFATs;              // Number of FATs
    uint16_t rootEntryCount;       // Root entry count (FAT32 uses 0)
    uint16_t totalSectors16;       // Total sectors (small volumes)
    uint8_t  media;                // Media descriptor
    uint16_t sectorsPerFAT16;      // Sectors per FAT (FAT16 field)
    uint16_t sectorsPerTrack;      // Sectors per track
    uint16_t numberOfHeads;        // Number of heads
    uint32_t hiddenSectors;        // Hidden sectors
    uint32_t totalSectors32;       // Total sectors (large volumes)
    uint32_t sectorsPerFAT32;      // Sectors per FAT (FAT32 field)
    uint16_t extFlags;             // Flags
    uint16_t fsVersion;            // File system version
    uint32_t rootCluster;          // Root directory start cluster
    uint16_t fsInfo;               // FSInfo sector
    uint16_t backupBootSector;     // Backup boot sector
    uint8_t  reserved[12];         // Reserved
    uint8_t  driveNumber;          // Drive number
    uint8_t  reserved1;            // Reserved
    uint8_t  bootSignature;        // Extended boot signature (0x29)
    uint32_t volumeID;             // Volume serial number
    char     volumeLabel[11];      // Volume label
    char     fsType[8];            // File system type (e.g., "FAT32   ")
    uint8_t  bootCode[420];        // Boot code
    uint16_t bootSectorSig;        // Boot sector signature (0x55, 0xAA)
} FAT32BootSector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    uint32_t leadSignature;       // Must be 0x41615252
    uint8_t  reserved1[480];      // Reserved, must be zero
    uint32_t structSignature;     // Must be 0x61417272
    uint32_t freeCount;           // Last known number of free clusters on the volume, or 0xFFFFFFFF if unknown
    uint32_t nextFree;            // Cluster number of the next free cluster, or 0xFFFFFFFF if unknown
    uint8_t  reserved2[12];       // Reserved, must be zero
    uint32_t trailSignature;      // Must be 0xAA550000
} FSInfo;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    char name[11];          // File name (8 characters) + extension (3 characters)
    uint8_t attr;           // File attributes
    uint8_t ntReserved;     // Reserved for use by Windows NT
    uint8_t creationTimeTenth; // Creation time, fine resolution
    uint16_t creationTime;   // Creation time
    uint16_t creationDate;   // Creation date
    uint16_t lastAccessDate; // Last access date
    uint16_t firstClusterHI; // High word of the entry's first cluster number
    uint16_t writeTime;      // Last write time
    uint16_t writeDate;      // Last write date
    uint16_t firstClusterLO; // Low word of the entry's first cluster number
    uint32_t fileSize;       // File size in bytes
} DirectoryEntry;
#pragma pack(pop)



#define SECTOR_SIZE 512
#define SECTOR_PER_CLUST 8
#define CLUSTER_SIZE 4096  // 8 sectors per cluster * 512 bytes per sector

uint32_t getNextCluster(FILE* disk, FAT32BootSector* bs, uint32_t currentCluster);
void extendDirectory(FILE* disk, FAT32BootSector* bs, uint32_t startCluster);
void readDirectoryEntryTime(uint16_t packedTime, uint16_t packedDate);
void mkdir(FILE* disk, const char* dirName, uint32_t totalClusters, FAT32BootSector* bs, uint32_t currentDirCluster);

void formatFileName(char* dest, const char* name, const char* ext) {
    memset(dest, ' ', 11);
    strncpy(dest, name, strlen(name));
    strncpy(dest + 8, ext, strlen(ext));
    // Function to pack time into FAT32 format
    uint16_t packTime(struct tm* timeinfo) {
        return ((timeinfo->tm_hour << 11) & 0xF800) |
            ((timeinfo->tm_min << 5) & 0x07E0) |
            ((timeinfo->tm_sec / 2) & 0x001F);
    }

    // Function to pack date into FAT32 format
    uint16_t packDate(struct tm* timeinfo) {
        return (((timeinfo->tm_year - 80) << 9) & 0xFE00) |
            ((timeinfo->tm_mon + 1) << 5) & 0x01E0 |
            (timeinfo->tm_mday & 0x001F);
    }

    void unpackTime(uint16_t packedTime, struct tm* timeinfo) {
        timeinfo->tm_hour = (packedTime >> 11) & 0x1F;
        timeinfo->tm_min = (packedTime >> 5) & 0x3F;
        timeinfo->tm_sec = (packedTime & 0x1F) * 2;
    }

    // Function to unpack FAT32 date into a `struct tm`
    void unpackDate(uint16_t packedDate, struct tm* timeinfo) {
        timeinfo->tm_year = ((packedDate >> 9) & 0x7F) + 80;
        timeinfo->tm_mon = ((packedDate >> 5) & 0x0F) - 1;
        timeinfo->tm_mday = packedDate & 0x1F;
    }


    void readDirectoryEntryTime(uint16_t packedTime, uint16_t packedDate) {
        struct tm timeinfo;
        unpackTime(packedTime, &timeinfo);
        unpackDate(packedDate, &timeinfo);

        if (timeinfo.tm_mon < 0 || timeinfo.tm_mon > 11) {
            printf("Error: Invalid month value %d\n", timeinfo.tm_mon);
            return;
        }
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
        printf("Date and Time: %s\n", buffer);
    }







    uint32_t calculateTotalClusters(FAT32BootSector * bs) {
        return (bs->totalSectors32 - bs->reservedSectors) / SECTOR_PER_CLUST;
    }


    uint32_t calculateROOTDIR_START(FAT32BootSector * bs) {
        uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
        return firstDataSector + (bs->rootCluster) * bs->sectorsPerCluster;
    }

    void initializeRootDirectory(FILE * disk, FAT32BootSector * bs) {
        uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
        uint32_t firstSectorOfCluster = firstDataSector + (bs->rootCluster) * bs->sectorsPerCluster;


        uint8_t emptyEntry[SECTOR_SIZE] = { 0 };
        for (uint32_t i = 0; i < bs->sectorsPerCluster; ++i) {
            fseek(disk, (firstSectorOfCluster + i) * SECTOR_SIZE, SEEK_SET);
            fwrite(emptyEntry, SECTOR_SIZE, 1, disk);
        }






        DirectoryEntry dotEntry, dotDotEntry;
        memset(&dotEntry, 0, sizeof(DirectoryEntry));
        memset(&dotDotEntry, 0, sizeof(DirectoryEntry));

        formatFileName(dotEntry.name, ".", "");
        dotEntry.attr = 0x10;
        dotEntry.firstClusterHI = (bs->rootCluster >> 16) & 0xFFFF;
        dotEntry.firstClusterLO = bs->rootCluster & 0xFFFF;
        dotEntry.fileSize = 0;

        formatFileName(dotDotEntry.name, "..", "");
        dotDotEntry.attr = 0x10;
        dotDotEntry.firstClusterHI = (bs->rootCluster >> 16) & 0xFFFF;
        dotDotEntry.firstClusterLO = bs->rootCluster & 0xFFFF;
        dotDotEntry.fileSize = 0;


        fseek(disk, firstSectorOfCluster * SECTOR_SIZE, SEEK_SET);
        fwrite(&dotEntry, sizeof(DirectoryEntry), 1, disk);
        fwrite(&dotDotEntry, sizeof(DirectoryEntry), 1, disk);




        printf("Initialized root directory with volume label.\n");
    }


    void initializeFAT(FILE * disk, uint32_t fatSize, uint32_t totalClusters) {
        uint8_t* fatBuffer = (uint8_t*)calloc(SECTOR_SIZE, fatSize);

        if (!fatBuffer) {
            perror("Failed to allocate memory for FAT");
            return;
        }

        for (uint32_t i = 0; i < totalClusters; ++i) {
            uint32_t fatOffset = i * 4;
            uint32_t fatIndex = fatOffset / SECTOR_SIZE;
            uint32_t fatEntryOffset = fatOffset % SECTOR_SIZE;

            if (i == 0) {
                // Reserved entry (boot sector, FAT tables)
                *(uint32_t*)(fatBuffer + fatIndex * SECTOR_SIZE + fatEntryOffset) = 0x0FFFFFF8;
            }
            else if (i == 1) {
                // Reserved entry (root directory)
                *(uint32_t*)(fatBuffer + fatIndex * SECTOR_SIZE + fatEntryOffset) = 0x0FFFFFFF;
            }
            else {
                // Free cluster (not allocated)
                *(uint32_t*)(fatBuffer + fatIndex * SECTOR_SIZE + fatEntryOffset) = 0x00000000;
            }
        }

        fseek(disk, SECTOR_SIZE * 32, SEEK_SET);  // Skip the reserved sectors
        fwrite(fatBuffer, SECTOR_SIZE, fatSize, disk);  // Write the first FAT table
        fwrite(fatBuffer, SECTOR_SIZE, fatSize, disk);  // Write the second FAT table
        free(fatBuffer);
    }

    void initializeBootSector(FAT32BootSector * bs, int disk_size) {
        memset(bs, 0, sizeof(FAT32BootSector));

        bs->jumpInstruction[0] = 0xEB;
        bs->jumpInstruction[1] = 0x58;
        bs->jumpInstruction[2] = 0x90;
        memcpy(bs->oemName, "MSWIN4.1", 8);

        bs->bytesPerSector = SECTOR_SIZE;
        bs->sectorsPerCluster = SECTOR_PER_CLUST;  // 4 KB per cluster
        bs->reservedSectors = 32;
        bs->numFATs = 2;
        bs->rootEntryCount = 0;
        bs->totalSectors32 = (disk_size * 1024 * 1024) / SECTOR_SIZE;  // Total sectors
        bs->media = 0xF8;

        // Calculate Sectors per FAT
        uint32_t clusters = (((disk_size * 1024 * 1024) / SECTOR_SIZE) - bs->reservedSectors) / bs->sectorsPerCluster;  // Total sectors minus reserved sectors, divided by sectors per cluster

        bs->sectorsPerFAT32 = (clusters * 4 + 511) / 512;  // Each FAT entry is 4 bytes, round up to nearest sector

        bs->rootCluster = 0;
        bs->fsInfo = 1;
        bs->backupBootSector = 6;
        bs->bootSignature = 0x29;
        bs->volumeID = 0x12345678;

        memcpy(bs->volumeLabel, "EMULATED  ", 11);
        memcpy(bs->fsType, "FAT32  ", 8);

        bs->bootSectorSig = 0xAA55;
    }

    void initializeFSInfo(FILE * disk, uint32_t totalClusters) {
        FSInfo fsInfo;

        memset(&fsInfo, 0, sizeof(FSInfo));

        fsInfo.leadSignature = 0x41615252;
        fsInfo.structSignature = 0x61417272;
        fsInfo.freeCount = totalClusters - 2;
        fsInfo.nextFree = 2;
        fsInfo.trailSignature = 0xAA550000;

        fseek(disk, SECTOR_SIZE, SEEK_SET);
        fwrite(&fsInfo, sizeof(FSInfo), 1, disk);
    }



    char** splitPath(const char* path, int* numComponents) {

        char* pathCopy = strdup(path);
        char* token = strtok(pathCopy, "/");
        int componentsCount = 0;

        char** components = NULL;

        while (token) {
            components = realloc(components, sizeof(char*) * (componentsCount + 1));
            components[componentsCount] = strdup(token);
            componentsCount++;
            token = strtok(NULL, "/");
        }

        *numComponents = componentsCount;
        free(pathCopy);

        return components;
    }

    void freePathComponents(char** components, int numComponents) {
        for (int i = 0; i < numComponents; i++) {
            free(components[i]);
        }
        free(components);
    }

    int compareFAT32Names(const char* entryName, const char* inputName) {
        char formattedName[12] = { ' ' };
        strncpy(formattedName, inputName, strlen(inputName));
        for (int i = strlen(inputName); i < 11; i++) {
            formattedName[i] = ' ';
        }
        return strncmp(entryName, formattedName, 11) == 0;
    }

    void createDirectoryEntry(DirectoryEntry * entry, const char* name, const char* ext, uint8_t attr, uint16_t firstCluster, uint32_t fileSize) {
        formatFileName(entry->name, name, ext);
        entry->attr = attr;
        entry->ntReserved = 0;

        // Get current time
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // Set the creation time and date
        entry->creationTime = packTime(timeinfo);
        entry->creationDate = packDate(timeinfo);
        entry->lastAccessDate = packDate(timeinfo);
        entry->writeTime = packTime(timeinfo);
        entry->writeDate = packDate(timeinfo);

        // Set the starting cluster number
        entry->firstClusterHI = (firstCluster >> 16) & 0xFFFF;
        entry->firstClusterLO = firstCluster & 0xFFFF;

        // Set the file size
        entry->fileSize = fileSize;


    }

    void writeDirectoryEntry(FILE * disk, uint32_t clusterNumber, uint32_t entryIndex, DirectoryEntry * entry, FAT32BootSector * bs) {
        uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
        uint32_t firstSectorOfCluster = firstDataSector + (clusterNumber)*bs->sectorsPerCluster;
        uint32_t offsetWithinCluster = entryIndex * sizeof(DirectoryEntry);

        fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + offsetWithinCluster, SEEK_SET);
        fwrite(entry, sizeof(DirectoryEntry), 1, disk);
    }

    uint32_t findFreeDirectoryEntry(FILE * disk, FAT32BootSector * bs, uint32_t startCluster) {
        uint32_t clusterNumber = startCluster;

        while (1) {
            uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
            uint32_t firstSectorOfCluster = firstDataSector + (clusterNumber)*bs->sectorsPerCluster;
            DirectoryEntry entry;

            for (uint32_t i = 0; i < (CLUSTER_SIZE / sizeof(DirectoryEntry)); i++) {
                fseek(disk, firstSectorOfCluster * SECTOR_SIZE + i * sizeof(DirectoryEntry), SEEK_SET);
                fread(&entry, sizeof(DirectoryEntry), 1, disk);
                uint8_t comparator = entry.name[0];
                if (comparator == 0x00 || comparator == 0xE5) {  // 0x00 or 0xE5 indicates a free entry
                    return (clusterNumber << 16) | i;  // Return the cluster number and entry index
                }
            }

            uint32_t nextCluster = getNextCluster(disk, bs, clusterNumber);
            if (nextCluster == 0xFFFFFFFF) {
                extendDirectory(disk, bs, clusterNumber);
                return (clusterNumber << 16) | 0;
            }

            clusterNumber = nextCluster;
        }
    }

    uint32_t allocateCluster(FILE * disk, uint32_t totalClusters) {
        for (uint32_t i = 2; i < totalClusters; ++i) {
            uint32_t fatOffset = i * 4;
            uint32_t fatEntry;
            fseek(disk, SECTOR_SIZE * 32 + fatOffset, SEEK_SET);
            fread(&fatEntry, sizeof(uint32_t), 1, disk);

            if (fatEntry == 0x00000000) {
                fatEntry = 0x0FFFFFFF;
                fseek(disk, SECTOR_SIZE * 32 + fatOffset, SEEK_SET);
                fwrite(&fatEntry, sizeof(uint32_t), 1, disk);
                return i;
            }
        }

        return 0;

        void touch(FILE * disk, const char* fileName, const char* fileExt, uint32_t totalClusters, FAT32BootSector * bs, uint32_t currentDirCluster) {

            fseek(disk, 0, SEEK_SET);
            fread(bs, sizeof(FAT32BootSector), 1, disk);


            uint32_t newFileCluster = allocateCluster(disk, totalClusters);
            if (newFileCluster == 0) {
                printf("No free clusters available.\n");
                return;
            }


            uint32_t freeEntryPos = findFreeDirectoryEntry(disk, bs, currentDirCluster);
            if (freeEntryPos == (uint32_t)-1) {
                printf("No free directory entries available.\n");
                return;
            }


            uint32_t entryCluster = freeEntryPos >> 16;
            uint32_t entryIndex = freeEntryPos & 0xFFFF;

            DirectoryEntry fileEntry;
            createDirectoryEntry(&fileEntry, fileName, fileExt, 0x20, newFileCluster, 0); // 0x20 indicates a regular file

            writeDirectoryEntry(disk, entryCluster, entryIndex, &fileEntry, bs);


            fseek(disk, 0, SEEK_SET);
            fwrite(bs, sizeof(FAT32BootSector), 1, disk);

            printf("Created empty file: %s.%s\n", fileName, fileExt);
        }







        uint32_t getNextCluster(FILE * disk, FAT32BootSector * bs, uint32_t currentCluster) {
            uint32_t fatOffset = bs->reservedSectors * SECTOR_SIZE + currentCluster * 4;
            uint32_t nextCluster;

            fseek(disk, fatOffset, SEEK_SET);
            fread(&nextCluster, sizeof(uint32_t), 1, disk);

            // Mask out the upper 4 bits (FAT32 uses 28 bits for cluster addresses)
            nextCluster &= 0x0FFFFFFF;


            if (nextCluster >= 0x0FFFFFF8) {
                return 0xFFFFFFFF;
            }

            return nextCluster;
        }



        void ls(FILE * disk, FAT32BootSector * bs, uint32_t currentCluster, const char* path) {

            if (path[0] == '/') {

                currentCluster = bs->rootCluster;
                path++;
            }


            int numComponents = 0;
            char** components = splitPath(path, &numComponents);

            for (int i = 0; i < numComponents; i++) {
                uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
                uint32_t foundCluster = 0;

                do {
                    uint32_t firstSectorOfCluster = firstDataSector + (currentCluster)*bs->sectorsPerCluster;
                    DirectoryEntry entry;
                    for (uint32_t j = 0; j < (CLUSTER_SIZE / sizeof(DirectoryEntry)); j++) {
                        fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (j * sizeof(DirectoryEntry)), SEEK_SET);
                        fread(&entry, sizeof(DirectoryEntry), 1, disk);
                        uint8_t comparator = entry.name[0];

                        if (comparator == 0x00) {
                            break;
                        }
                        if (comparator == 0xE5) {
                            continue;
                        }


                        if ((entry.attr & 0x10) && compareFAT32Names(entry.name, components[i])) {
                            foundCluster = (entry.firstClusterHI << 16) | entry.firstClusterLO;
                            break;
                        }
                    }


                    if (foundCluster == 0) {
                        currentCluster = getNextCluster(disk, bs, currentCluster);
                    }
                } while (foundCluster == 0 && currentCluster != 0xFFFFFFFF);



                if (foundCluster == 0) {
                    printf("Directory '%s' not found.\n", components[i]);
                    freePathComponents(components, numComponents);
                    return;
                }

                currentCluster = foundCluster;
            }

            freePathComponents(components, numComponents);


            uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);

            char volumetag[11];
            strcpy(volumetag, bs->volumeLabel);
            for (int j = 9; j >= 0; j--) {
                if (volumetag[j] == ' ') {
                    volumetag[j] = '\0';
                }
                else {
                    break;
                }
            }

            printf("Directory: %s/%s\n", volumetag, path);
            printf("====================\n");

            do {
                uint32_t firstSectorOfCluster = firstDataSector + (currentCluster)*bs->sectorsPerCluster;

                DirectoryEntry entry;
                for (uint32_t i = 0; i < (CLUSTER_SIZE / sizeof(DirectoryEntry)); i++) {
                    fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (i * sizeof(DirectoryEntry)), SEEK_SET);
                    fread(&entry, sizeof(DirectoryEntry), 1, disk);
                    uint8_t comparator = entry.name[0];

                    if (comparator == 0x00) {

                        return;
                    }
                    if (comparator == 0xE5) {

                        continue;
                    }


                    if (entry.attr == 0x08) {
                        continue;
                    }


                    char fileName[9];
                    char fileExt[4];
                    memcpy(fileName, entry.name, 8);
                    fileName[8] = '\0';
                    memcpy(fileExt, entry.name + 8, 3);
                    fileExt[3] = '\0';


                    for (int j = 7; j >= 0; j--) {
                        if (fileName[j] == ' ') {
                            fileName[j] = '\0';
                        }
                        else {
                            break;
                        }
                    }

                    for (int j = 2; j >= 0; j--) {
                        if (fileExt[j] == ' ') {
                            fileExt[j] = '\0';
                        }
                        else {
                            break;
                        }
                    }


                    if (entry.attr & 0x10) {
                        printf("%s <DIR>        ", fileName);
                    }
                    else { // File
                        printf("%s.%s ", fileName, fileExt);
                        printf("%10u bytes ", entry.fileSize);
                        readDirectoryEntryTime(entry.creationTime, entry.creationDate);
                    }


                    printf("\n");
                }


                currentCluster = getNextCluster(disk, bs, currentCluster);
            } while (currentCluster != 0xFFFFFFFF);
        }






        uint32_t findLastCluster(FILE * disk, FAT32BootSector * bs, uint32_t startCluster) {
            uint32_t currentCluster = startCluster;
            uint32_t nextCluster;

            while (1) {
                nextCluster = getNextCluster(disk, bs, currentCluster);
                if (nextCluster == 0xFFFFFFFF) {
                    return currentCluster;
                }

                currentCluster = nextCluster;
            }
        }


        void extendDirectory(FILE * disk, FAT32BootSector * bs, uint32_t startCluster) {
            uint32_t lastCluster = findLastCluster(disk, bs, startCluster);


            uint32_t newCluster = allocateCluster(disk, calculateTotalClusters(bs));
            if (newCluster == 0) {
                printf("No free clusters available to extend the directory.\n");
                return;
            }


            uint32_t fatOffset = bs->reservedSectors * SECTOR_SIZE + lastCluster * 4;
            fseek(disk, fatOffset, SEEK_SET);
            fwrite(&newCluster, sizeof(uint32_t), 1, disk);


            fatOffset = bs->reservedSectors * SECTOR_SIZE + newCluster * 4;
            fseek(disk, fatOffset, SEEK_SET);
            uint32_t endOfChainMarker = 0x0FFFFFFF;
            fwrite(&endOfChainMarker, sizeof(uint32_t), 1, disk);


            uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
            uint32_t firstSectorOfCluster = firstDataSector + (newCluster)*bs->sectorsPerCluster;

            uint8_t emptyEntry[SECTOR_SIZE] = { 0 };
            for (uint32_t i = 0; i < bs->sectorsPerCluster; ++i) {
                fseek(disk, (firstSectorOfCluster + i) * SECTOR_SIZE, SEEK_SET);
                fwrite(emptyEntry, SECTOR_SIZE, 1, disk);
            }

            printf("Directory extended by adding cluster %u.\n", newCluster);
        }

        void mkdir(FILE * disk, const char* dirName, uint32_t totalClusters, FAT32BootSector * bs, uint32_t currentDirCluster) {
            fseek(disk, 0, SEEK_SET);
            fread(bs, sizeof(FAT32BootSector), 1, disk);


            uint32_t newDirCluster = allocateCluster(disk, totalClusters);
            if (newDirCluster == 0) {
                printf("No free clusters available.\n");
                return;
            }


            uint32_t freeEntryPos = findFreeDirectoryEntry(disk, bs, currentDirCluster);
            if (freeEntryPos == (uint32_t)-1) {
                printf("No free directory entries available.\n");
                return;
            }

            uint32_t entryCluster = freeEntryPos >> 16;
            uint32_t entryIndex = freeEntryPos & 0xFFFF;


            DirectoryEntry dirEntry;
            createDirectoryEntry(&dirEntry, dirName, "", 0x10, newDirCluster, 0); // 0x10 indicates a directory


            writeDirectoryEntry(disk, entryCluster, entryIndex, &dirEntry, bs);

            // Initialize the newly created directory cluster
            uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
            uint32_t firstSectorOfCluster = firstDataSector + (newDirCluster)*bs->sectorsPerCluster;

            // Initialize the directory with "." and ".." entries
            DirectoryEntry dotEntry, dotDotEntry;
            memset(&dotEntry, 0, sizeof(DirectoryEntry));
            memset(&dotDotEntry, 0, sizeof(DirectoryEntry));

            // "." entry (current directory)
            formatFileName(dotEntry.name, ".", "");
            dotEntry.attr = 0x10;
            dotEntry.firstClusterHI = (newDirCluster >> 16) & 0xFFFF;
            dotEntry.firstClusterLO = newDirCluster & 0xFFFF;
            dotEntry.fileSize = 0;

            // ".." entry (parent directory)
            formatFileName(dotDotEntry.name, "..", "");
            dotDotEntry.attr = 0x10;
            dotDotEntry.firstClusterHI = (currentDirCluster >> 16) & 0xFFFF;
            dotDotEntry.firstClusterLO = currentDirCluster & 0xFFFF;
            dotDotEntry.fileSize = 0;

            // Write "." and ".." entries to the new directory cluster
            fseek(disk, firstSectorOfCluster * SECTOR_SIZE, SEEK_SET);
            fwrite(&dotEntry, sizeof(DirectoryEntry), 1, disk);
            fwrite(&dotDotEntry, sizeof(DirectoryEntry), 1, disk);


            uint8_t emptyEntry[SECTOR_SIZE] = { 0 };
            for (uint32_t i = 2 * sizeof(DirectoryEntry); i < CLUSTER_SIZE; i += SECTOR_SIZE) {
                fwrite(emptyEntry, SECTOR_SIZE, 1, disk);
            }

            fseek(disk, 0, SEEK_SET);
            fwrite(bs, sizeof(FAT32BootSector), 1, disk);

            printf("Directory '%s' created in current directory.\n", dirName);
        }




        uint32_t cd(FILE * disk, FAT32BootSector * bs, const char* path) {
            if (path[0] != '/') {
                printf("Error: Path must be absolute (starting with '/').\n");
                return (uint32_t)-1;
            }

            uint32_t currentCluster = bs->rootCluster;



            int numComponents = 0;
            char** components = splitPath(path, &numComponents);

            // Traverse each component in the path
            for (int i = 0; i < numComponents; i++) {
                uint32_t firstDataSector = (bs->numFATs * bs->sectorsPerFAT32) + bs->reservedSectors;
                uint32_t foundCluster = 0;


                while (currentCluster != 0xFFFFFFFF) {
                    uint32_t firstSectorOfCluster = firstDataSector + (currentCluster)*bs->sectorsPerCluster;
                    DirectoryEntry entry;
                    int found = 0;

                    for (uint32_t j = 0; j < (CLUSTER_SIZE / sizeof(DirectoryEntry)); j++) {
                        fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (j * sizeof(DirectoryEntry)), SEEK_SET);
                        fread(&entry, sizeof(DirectoryEntry), 1, disk);

                        uint8_t comparator = entry.name[0];

                        if (comparator == 0xE5) {

                            continue;
                        }


                        if ((entry.attr & 0x10) && compareFAT32Names(entry.name, components[i])) {
                            foundCluster = (entry.firstClusterHI << 16) | entry.firstClusterLO;
                            found = 1;
                            break;
                        }
                    }


                    if (found) break;

                    currentCluster = getNextCluster(disk, bs, currentCluster);
                }


                if (foundCluster == 0) {
                    printf("Directory '%s' not found.\n", components[i]);
                    freePathComponents(components, numComponents);
                    return (uint32_t)-1;
                }

                // Move to the found directory
                currentCluster = foundCluster;
            }
            freePathComponents(components, numComponents);

            return currentCluster;
        }

        void createFileWithSize(FILE * disk, const char* fileName, const char* fileExt, uint32_t fileSize, uint32_t totalClusters, FAT32BootSector * bs, uint32_t currentDirCluster) {
            uint32_t clustersNeeded = (fileSize + CLUSTER_SIZE - 1) / CLUSTER_SIZE; // Calculate required clusters
            uint32_t firstCluster = 0;
            uint32_t previousCluster = 0;


            for (uint32_t i = 0; i < clustersNeeded; ++i) {
                uint32_t newCluster = allocateCluster(disk, totalClusters);
                if (newCluster == 0) {
                    printf("No free clusters available.\n");
                    return;
                }

                if (i == 0) {
                    firstCluster = newCluster;
                }
                else {

                    uint32_t fatOffset = bs->reservedSectors * SECTOR_SIZE + previousCluster * 4;
                    fseek(disk, fatOffset, SEEK_SET);
                    fwrite(&newCluster, sizeof(uint32_t), 1, disk);
                }


                if (i == clustersNeeded - 1) {
                    uint32_t fatOffset = bs->reservedSectors * SECTOR_SIZE + newCluster * 4;
                    uint32_t endOfChainMarker = 0x0FFFFFFF;
                    fseek(disk, fatOffset, SEEK_SET);
                    fwrite(&endOfChainMarker, sizeof(uint32_t), 1, disk);
                }


                uint8_t buffer[SECTOR_SIZE];
                memset(buffer, 0xAF, SECTOR_SIZE);
                uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
                uint32_t firstSectorOfCluster = firstDataSector + (newCluster)*bs->sectorsPerCluster;
                for (uint32_t j = 0; j < bs->sectorsPerCluster; ++j) {
                    fseek(disk, (firstSectorOfCluster + j) * SECTOR_SIZE, SEEK_SET);
                    fwrite(buffer, SECTOR_SIZE, 1, disk);
                }

                previousCluster = newCluster;
            }


            uint32_t freeEntryPos = findFreeDirectoryEntry(disk, bs, currentDirCluster);
            if (freeEntryPos == (uint32_t)-1) {
                printf("No free directory entries available.\n");
                return;
            }

            uint32_t entryIndex = freeEntryPos & 0xFFFF;

            DirectoryEntry fileEntry;
            createDirectoryEntry(&fileEntry, fileName, fileExt, 0x20, firstCluster, fileSize);


            writeDirectoryEntry(disk, entryCluster, entryIndex, &fileEntry, bs);

            printf("Created file: %s.%s with size %u bytes\n", fileName, fileExt, fileSize);
        }

        void deleteFileInCurrentDir(FILE * disk, FAT32BootSector * bs, uint32_t currentCluster, const char* fileName, const char* fileExt) {
            uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
            uint32_t entryCluster = currentCluster;
            uint32_t entryIndex = -1;
            DirectoryEntry entry;

            do {
                uint32_t firstSectorOfCluster = firstDataSector + (entryCluster)*bs->sectorsPerCluster;

                for (uint32_t i = 0; i < (CLUSTER_SIZE / sizeof(DirectoryEntry)); i++) {
                    fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (i * sizeof(DirectoryEntry)), SEEK_SET);
                    fread(&entry, sizeof(DirectoryEntry), 1, disk);

                    uint8_t comparator = entry.name[0];
                    if (comparator == 0x00) break;
                    if (comparator == 0xE5) continue;


                    char formattedName[12];
                    formatFileName(formattedName, fileName, fileExt);

                    if (strncmp(entry.name, formattedName, 11) == 0) {
                        entryIndex = i;
                        break;
                    }
                }

                if (entryIndex != -1) break;

                entryCluster = getNextCluster(disk, bs, entryCluster);
            } while (entryCluster != 0xFFFFFFFF);

            if (entryIndex == -1) {
                printf("File '%s.%s' not found in the current directory.\n", fileName, fileExt);
                return;
            }


            entry.name[0] = 0xE5;
            uint32_t firstSectorOfCluster = firstDataSector + (entryCluster)*bs->sectorsPerCluster;
            fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (entryIndex * sizeof(DirectoryEntry)), SEEK_SET);
            fwrite(&entry, sizeof(DirectoryEntry), 1, disk);


            uint32_t clusterToFree = (entry.firstClusterHI << 16) | entry.firstClusterLO;
            while (clusterToFree != 0xFFFFFFFF && clusterToFree != 0) {
                uint32_t fatOffset = bs->reservedSectors * SECTOR_SIZE + clusterToFree * 4;
                fseek(disk, fatOffset, SEEK_SET);
                uint32_t nextCluster;
                fread(&nextCluster, sizeof(uint32_t), 1, disk);

                nextCluster &= 0x0FFFFFFF;  // Mask out the upper 4 bits
                if (nextCluster >= 0x0FFFFFF8) nextCluster = 0xFFFFFFFF;


                uint32_t freeClusterMarker = 0x00000000;
                fseek(disk, fatOffset, SEEK_SET);
                fwrite(&freeClusterMarker, sizeof(uint32_t), 1, disk);

                clusterToFree = nextCluster;
            }

            printf("Deleted file '%s.%s' from the current directory.\n", fileName, fileExt);
        }

        void freeClusterChain(FILE * disk, FAT32BootSector * bs, uint32_t startCluster) {
            uint32_t cluster = startCluster;
            while (cluster != 0xFFFFFFFF && cluster != 0) {
                uint32_t fatOffset = bs->reservedSectors * SECTOR_SIZE + cluster * 4;
                fseek(disk, fatOffset, SEEK_SET);
                uint32_t nextCluster;
                fread(&nextCluster, sizeof(uint32_t), 1, disk);

                nextCluster &= 0x0FFFFFFF;  // Mask out the upper 4 bits
                if (nextCluster >= 0x0FFFFFF8) nextCluster = 0xFFFFFFFF;

                // Mark the current cluster as free
                uint32_t freeClusterMarker = 0x00000000;
                fseek(disk, fatOffset, SEEK_SET);
                fwrite(&freeClusterMarker, sizeof(uint32_t), 1, disk);

                cluster = nextCluster;
            }
        }
        void extractFileNameAndExt(const char* fullName, char* fileName, char* fileExt) {
            strncpy(fileName, fullName, 8);
            fileName[8] = '\0';
            for (int i = 7; i >= 0 && fileName[i] == ' '; i--) fileName[i] = '\0';

            strncpy(fileExt, fullName + 8, 3);
            fileExt[3] = '\0';
            for (int i = 2; i >= 0 && fileExt[i] == ' '; i--) fileExt[i] = '\0';
        }
        void deleteDirectoryInCurrentDir(FILE * disk, FAT32BootSector * bs, uint32_t currentCluster, const char* dirName);


        void deleteDirectoryContents(FILE * disk, FAT32BootSector * bs, uint32_t dirCluster) {
            uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
            uint32_t entryCluster = dirCluster;
            DirectoryEntry entry;

            do {
                uint32_t firstSectorOfCluster = firstDataSector + (entryCluster)*bs->sectorsPerCluster;

                for (uint32_t i = 0; i < (CLUSTER_SIZE / sizeof(DirectoryEntry)); i++) {
                    fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (i * sizeof(DirectoryEntry)), SEEK_SET);
                    fread(&entry, sizeof(DirectoryEntry), 1, disk);

                    if (entry.name[0] == 0x00) break;
                    if (entry.name[0] == 0xE5) continue;


                    if (entry.attr & 0x10) {

                        if (entry.name[0] == '.' && (entry.name[1] == 0x20 || entry.name[1] == '.')) continue;

                        // Recursively delete subdirectory
                        uint32_t subDirCluster = (entry.firstClusterHI << 16) | entry.firstClusterLO;
                        deleteDirectoryContents(disk, bs, subDirCluster);
                        deleteDirectoryInCurrentDir(disk, bs, dirCluster, entry.name);
                    }
                    else {

                        char fileName[9], fileExt[4];
                        extractFileNameAndExt(entry.name, fileName, fileExt);
                        deleteFileInCurrentDir(disk, bs, dirCluster, fileName, fileExt);
                    }
                }

                entryCluster = getNextCluster(disk, bs, entryCluster);
            } while (entryCluster != 0xFFFFFFFF);
        }

        void deleteDirectoryInCurrentDir(FILE * disk, FAT32BootSector * bs, uint32_t currentCluster, const char* dirName) {
            uint32_t firstDataSector = bs->reservedSectors + (bs->numFATs * bs->sectorsPerFAT32);
            uint32_t entryCluster = currentCluster;
            uint32_t entryIndex = -1;
            DirectoryEntry entry;

            do {
                uint32_t firstSectorOfCluster = firstDataSector + (entryCluster)*bs->sectorsPerCluster;

                for (uint32_t i = 0; i < (CLUSTER_SIZE / sizeof(DirectoryEntry)); i++) {
                    fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (i * sizeof(DirectoryEntry)), SEEK_SET);
                    fread(&entry, sizeof(DirectoryEntry), 1, disk);

                    if (entry.name[0] == 0x00) break;
                    if (entry.name[0] == 0xE5) continue;


                    if ((entry.attr & 0x10)) {
                        char formattedName[12];
                        formatFileName(formattedName, dirName, "");
                        if (strncmp(entry.name, formattedName, 11) == 0) {
                            entryIndex = i;
                            break;
                        }
                    }
                }

                if (entryIndex != -1) break;

                entryCluster = getNextCluster(disk, bs, entryCluster);
            } while (entryCluster != 0xFFFFFFFF);

            if (entryIndex == -1) {
                printf("Directory '%s' not found in the current directory.\n", dirName);
                return;
            }


            uint32_t dirCluster = (entry.firstClusterHI << 16) | entry.firstClusterLO;


            deleteDirectoryContents(disk, bs, dirCluster);


            entry.name[0] = 0xE5;
            uint32_t firstSectorOfCluster = firstDataSector + (entryCluster)*bs->sectorsPerCluster;
            fseek(disk, (firstSectorOfCluster * SECTOR_SIZE) + (entryIndex * sizeof(DirectoryEntry)), SEEK_SET);
            fwrite(&entry, sizeof(DirectoryEntry), 1, disk);


            freeClusterChain(disk, bs, dirCluster);

            printf("Deleted directory '%s' and its contents.\n", dirName);
        }





        int FAT32valid_CHECK(FILE * disk, char* path)
        {
            FAT32BootSector valid_bs;


            disk = fopen(path, "rb+");
            if (!disk) {
                perror("Failed to create disk image");
                return -1;
            }

            fread(&valid_bs, sizeof(FAT32BootSector), 1, disk);
            if (valid_bs.bootSectorSig != 0xAA55)
            {
                return -1;
            }
        }

        void format_volume(FAT32BootSector * bs, uint32_t size, uint32_t * current_dir, char* path) {
            FILE* disk = fopen(path, "wb");
            if (!disk) {
                perror("Failed to create disk image");
                return;
            }


            initializeBootSector(bs, size);


            fwrite(bs, sizeof(FAT32BootSector), 1, disk);

            uint32_t totalClusters = calculateTotalClusters(bs);
            uint32_t fatSize = bs->sectorsPerFAT32;


            uint8_t* emptyCluster = (uint8_t*)calloc(CLUSTER_SIZE, 1);
            if (!emptyCluster) {
                perror("Failed to allocate memory for data area");
                fclose(disk);
                return;
            }


            for (uint32_t i = 0; i < totalClusters; ++i) {
                fwrite(emptyCluster, CLUSTER_SIZE, 1, disk);
            }

            free(emptyCluster);


            initializeFAT(disk, fatSize, totalClusters);


            initializeFSInfo(disk, totalClusters);


            initializeRootDirectory(disk, bs);

            *current_dir = bs->rootCluster;

            fclose(disk);
        }


        int main(int argc, char* argv[]) {
            FAT32BootSector bs;
            uint8_t flag = 0;
            char command[256];
            uint32_t totalClusters;
            uint32_t fatSize;
            uint32_t currentDirectory;
            if (argc < 2) {
                printf("Usage: %s <file_path>\n", argv[0]);
                return 1;
            }


            FILE* disk = fopen(argv[1], "rb+");
            if (disk == NULL) {
                printf("Error: Could not open file %s\n", argv[1]);

            }




            fread(&bs, sizeof(FAT32BootSector), 1, disk);
            if (FAT32valid_CHECK(&bs, argv[1]) != -1) {
                flag = 1;
            }
            else
            {
                printf("Error: VALID FAT32 volume is abcent on chosen disk\nFormat disk or choose another one\n");
                printf("> ");
                fgets(command, sizeof(command), stdin);
                command[strcspn(command, "\n")] = 0;
                int size = -1;

                if (strncmp(command, "format", 6) == 0) {

                    sscanf(command, "format %d", &size);
                    if (size == -1) format_volume(&bs, 20, &currentDirectory, argv[1]);
                    else format_volume(&bs, size, &currentDirectory, argv[1]);

                    flag = 1;
                }
                else if (strncmp(command, "exit", 4) == 0) {

                    fclose(disk);
                    return 0;
                }
                else {
                    printf("Unknown command: %s\n", command);
                }
            }


            totalClusters = calculateTotalClusters(&bs);
            fatSize = bs.sectorsPerFAT32;
            currentDirectory = bs.rootCluster;

            fclose(disk);

            disk = fopen(argv[1], "rb+");
            if (!disk) {
                perror("Failed to reopen disk image");
                return 1;
            }
            fread(&bs, sizeof(FAT32BootSector), 1, disk);






            while (1) {
                printf("> ");
                fgets(command, sizeof(command), stdin);
                command[strcspn(command, "\n")] = 0;
                int size = NULL;

                if (strncmp(command, "format", 6) == 0) {

                    sscanf(command, "format %d", &size);
                    if (size == NULL) format_volume(&bs, 20, &currentDirectory, argv[1]);
                    else format_volume(&bs, size, &currentDirectory, argv[1]);

                    continue;
                }

                if (strncmp(command, "ls", 2) == 0) {
                    if (!disk) {
                        printf("No disk loaded. .\n");
                        continue;
                    }
                    disk = fopen(argv[1], "rb+");
                    if (!disk) {
                        perror("Failed to reopen disk image");
                        continue;
                    }
                    ls(disk, &bs, currentDirectory, command + 3);
                    fclose(disk);

                }
                else if (strncmp(command, "touch", 5) == 0) {
                    if (!disk) {
                        printf("No disk loaded. Use 'load' or 'format' first.\n");
                        continue;
                    }
                    char filename[256], ext[256];
                    sscanf(command, "touch %[^.].%s", filename, ext);
                    disk = fopen(argv[1], "rb+");
                    if (!disk) {
                        perror("Failed to reopen disk image");
                        continue;
                    }
                    touch(disk, filename, ext, totalClusters, &bs, currentDirectory);
                    fclose(disk);

                }
                else if (strncmp(command, "mkdir", 5) == 0) {
                    if (!disk) {
                        printf("No disk loaded. Use 'load' or 'format' first.\n");
                        continue;
                    }
                    char dirname[256];
                    sscanf(command, "mkdir %s", dirname);
                    disk = fopen(argv[1], "rb+");
                    if (!disk) {
                        perror("Failed to reopen disk image");
                        continue;
                    }
                    mkdir(disk, dirname, totalClusters, &bs, currentDirectory);
                    fclose(disk);

                }
                else if (strncmp(command, "cd", 2) == 0) {
                    if (!disk) {
                        printf("No disk loaded. Use 'load' or 'format' first.\n");
                        continue;
                    }
                    char path[256];
                    sscanf(command, "cd %s", path);
                    disk = fopen(argv[1], "rb+");
                    if (!disk) {
                        perror("Failed to reopen disk image");
                        continue;
                    }
                    currentDirectory = cd(disk, &bs, path);
                    fclose(disk);

                }
                else if (strncmp(command, "create", 6) == 0) {
                    if (!disk) {
                        printf("No disk loaded. Use 'load' or 'format' first.\n");
                        continue;
                    }
                    char filename[256], ext[256];
                    uint32_t size;
                    sscanf(command, "create %[^.].%s %u", filename, ext, &size);
                    disk = fopen(argv[1], "rb+");
                    if (!disk) {
                        perror("Failed to reopen disk image");
                        continue;
                    }
                    createFileWithSize(disk, filename, ext, size, totalClusters, &bs, currentDirectory);
                    fclose(disk);

                }
                else if (strncmp(command, "rm", 2) == 0) {
                    if (!disk) {
                        printf("No disk loaded. Use 'load' or 'format' first.\n");
                        continue;
                    }
                    char filename[256], ext[256];
                    sscanf(command, "rm %[^.].%s", filename, ext);
                    disk = fopen(argv[1], "rb+");
                    if (!disk) {
                        perror("Failed to reopen disk image");
                        continue;
                    }
                    deleteFileInCurrentDir(disk, &bs, currentDirectory, filename, ext);
                    fclose(disk);

                }
                else if (strncmp(command, "dd", 2) == 0) {
                    if (!disk) {
                        printf("No disk loaded. Use 'load' or 'format' first.\n");
                        continue;
                    }
                    char dirname[256];
                    sscanf(command, "dd %s", dirname);
                    disk = fopen(argv[1], "rb+");
                    if (!disk) {
                        perror("Failed to reopen disk image");
                        continue;
                    }
                    deleteDirectoryInCurrentDir(disk, &bs, currentDirectory, dirname);
                    fclose(disk);

                }

                else if (strncmp(command, "exit", 4) == 0) {
                    break;
                    fclose(disk);
                }
                else {
                    printf("Unknown command: %s\n", command);
                }

            }





        }





