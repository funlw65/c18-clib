/* 
 * File:   rosso_f32.h
 * Author: texan
 *
 * Created on February 9, 2015, 1:43 PM
 */

#ifndef ROSSO_F32_H
#define	ROSSO_F32_H

#define MAX_STRING_SIZE     100

//Structure to access Master Boot Record for getting info about partitions
struct MBRinfo_Structure {
	uint8_t nothing[446]; //ignore, placed here to fill the gap in the structure
	uint8_t partitionData[64];//partition records (16x4)
	uint16_t signature;//0xaa55
};

//Structure to access info of the first partition of the disk
struct partitionInfo_Structure {
	uint8_t status; //0x80 - active partition
	uint8_t headStart;//starting head
	uint16_t cylSectStart;//starting cylinder and sector
	uint8_t type;//partition type
	uint8_t headEnd;//ending head of the partition
	uint16_t cylSectEnd;//ending cylinder and sector
	uint32_t firstSector;//total sectors between MBR & the first sector of the partition
	uint32_t sectorsTotal;//size of this partition in sectors
};

//Structure to access boot sector data
struct BS_Structure {
	uint8_t jumpBoot[3]; //default: 0x009000EB
	uint8_t OEMName[8];
	uint16_t bytesPerSector;//default: 512
	uint8_t sectorPerCluster;
	uint16_t reservedSectorCount;
	uint8_t numberofFATs;
	uint16_t rootEntryCount;
	uint16_t totalSectors_F16;//must be 0 for FAT32
	uint8_t mediaType;
	uint16_t FATsize_F16;//must be 0 for FAT32
	uint16_t sectorsPerTrack;
	uint16_t numberofHeads;
	uint32_t hiddenSectors;
	uint32_t totalSectors_F32;
	uint32_t FATsize_F32;//count of sectors occupied by one FAT
	uint16_t extFlags;
	uint16_t FSversion;//0x0000 (defines version 0.0)
	uint32_t rootCluster;//first cluster of root directory (=2)
	uint16_t FSinfo;//sector number of FSinfo structure (=1)
	uint16_t BackupBootSector;
	uint8_t reserved[12];
	uint8_t driveNumber;
	uint8_t reserved1;
	uint8_t bootSignature;
	uint32_t volumeID;
	uint8_t volumeLabel[11];//"NO NAME "
	uint8_t fileSystemType[8];//"FAT32"
	uint8_t bootData[420];
	uint16_t bootEndSignature;//0xaa55
};

//Structure to access FSinfo sector data
struct FSInfo_Structure {
	uint32_t leadSignature; //0x41615252
	uint8_t reserved1[480];
	uint32_t structureSignature;//0x61417272
	uint32_t freeClusterCount;//initial: 0xffffffff
	uint32_t nextFreeCluster;//initial: 0xffffffff
	uint8_t reserved2[12];
	uint32_t trailSignature;//0xaa550000
};

//Structure to access Directory Entry in the FAT
struct dir_Structure {
	uint8_t name[11];
	uint8_t attrib; //file attributes
	uint8_t NTreserved;//always 0
	uint8_t timeTenth;//tenths of seconds, set to 0 here
	uint16_t createTime;//time file was created
	uint16_t createDate;//date file was created
	uint16_t lastAccessDate;
	uint16_t firstClusterHI;//higher word of the first cluster number
	uint16_t writeTime;//time of last write
	uint16_t writeDate;//date of last write
	uint16_t firstClusterLO;//lower word of the first cluster number
	uint32_t fileSize;//size of file in bytes
};

//Attribute definitions for file/directory
#define ATTR_READ_ONLY     0x01
#define ATTR_HIDDEN        0x02
#define ATTR_SYSTEM        0x04
#define ATTR_VOLUME_ID     0x08
#define ATTR_DIRECTORY     0x10
#define ATTR_ARCHIVE       0x20
#define ATTR_LONG_NAME     0x0f

#define DIR_ENTRY_SIZE     0x32
#define EMPTY              0x00
#define DELETED            0xe5
#define GET                   0
#define SET                   1
#define READ                  0
#define VERIFY                1
#define ADD                   0
#define REMOVE                1
#define LOW                   0
#define HIGH                  1
#define TOTAL_FREE            1
#define NEXT_FREE             2
#define GET_LIST              0
#define GET_FILE              1
#define DELETE                2
#define FAT32_EOF    0x0fffffff

//************* external variables *************
volatile uint32_t firstDataSector, rootCluster, totalClusters;
volatile uint16_t bytesPerSector, sectorPerCluster, reservedSectorCount;
uint32_t unusedSectors, appendFileSector, appendFileLocation, fileSize,
appendStartCluster;

//global flag to keep track of free cluster count updating in FSinfo sector
uint8_t freeClusterCountUpdated;

//data string where data is collected before sending to the card
volatile uint8_t dataString[MAX_STRING_SIZE];
uint16_t timeFAT, dateFAT;

// TODO: to be rewritten... depends on PCF8583 RTC functions...

uint8_t getDateTime_FAT(void) {
#ifdef ENABLE_PCF8583
    //it should get the time from RTC but for now, returns "error" code
    return 1;
#else
    return 1; // returning "error" code if RTC not available
    // date and time are 0 - good enough for testing and budget
#endif
}

uint32_t F32_getFirstSector(uint32_t clusterNumber) {
    return (((clusterNumber - 2) * sectorPerCluster) + firstDataSector);
}


uint32_t F32_getSetFreeCluster(uint8_t totOrNext, uint8_t get_set,
        uint32_t FSEntry) {

    struct FSInfo_Structure *FS = (struct FSInfo_Structure *) &SD_buffer;

    SD_readSingleBlock(unusedSectors + 1);

    if ((FS->leadSignature != 0x41615252)
            || (FS->structureSignature != 0x61417272)
            || (FS->trailSignature != 0xaa550000))
        return 0xffffffff;

    if (get_set == GET) {
        if (totOrNext == TOTAL_FREE)
            return (FS->freeClusterCount);
        else
            // when totOrNext = NEXT_FREE
            return (FS->nextFreeCluster);
    } else {
        if (totOrNext == TOTAL_FREE)
            FS->freeClusterCount = FSEntry;
        else
            // when totOrNext = NEXT_FREE
            FS->nextFreeCluster = FSEntry;

        SD_writeSingleBlock(unusedSectors + 1); //update FSinfo
    }
    return 0xffffffff;
}

uint8_t F32_getBootSectorData(void) {
    struct BS_Structure *bpb; //mapping the buffer onto the structure
    struct MBRinfo_Structure *mbr;
    struct partitionInfo_Structure *partition;
    uint32_t dataSectors;

    unusedSectors = 0;

    SD_readSingleBlock(0);
    bpb = (struct BS_Structure *) SD_buffer;

    if (bpb->jumpBoot[0] != 0xE9 && bpb->jumpBoot[0] != 0xEB)//check if it is boot sector
    {
        mbr = (struct MBRinfo_Structure *) SD_buffer; //if it is not boot sector, it must be MBR

        if (mbr->signature != 0xaa55)
            return 1; //if it is not even MBR then it's not FAT32

        partition = (struct partitionInfo_Structure *) (mbr->partitionData); //first partition
        unusedSectors = partition->firstSector; //the unused sectors, hidden to the FAT

        SD_readSingleBlock(partition->firstSector); //read the bpb sector
        bpb = (struct BS_Structure *) SD_buffer;
        if (bpb->jumpBoot[0] != 0xE9 && bpb->jumpBoot[0] != 0xEB)
            return 1;
    }

    bytesPerSector = bpb->bytesPerSector;
#ifdef ENABLE_SD_CARD_DEBUG
    //serial_puthexU16(bytesPerSector); serial_putchar(' ');
#endif
    sectorPerCluster = bpb->sectorPerCluster;
#ifdef ENABLE_SD_CARD_DEBUG
    //serial_puthexU08(sectorPerCluster); serial_putchar(' ');
#endif
    reservedSectorCount = bpb->reservedSectorCount;
    rootCluster = bpb->rootCluster; // + (sector / sectorPerCluster) +1;
    firstDataSector = bpb->hiddenSectors + reservedSectorCount
            + (bpb->numberofFATs * bpb->FATsize_F32);

    dataSectors = bpb->totalSectors_F32 - bpb->reservedSectorCount
            - (bpb->numberofFATs * bpb->FATsize_F32);
    totalClusters = dataSectors / sectorPerCluster;
#ifdef ENABLE_SD_CARD_DEBUG
    //serial_puthexU32(totalClusters); serial_putchar(' ');
#endif
    if ((F32_getSetFreeCluster(TOTAL_FREE, GET, 0)) > totalClusters) //check if FSinfo free clusters count is valid
        freeClusterCountUpdated = 0;
    else
        freeClusterCountUpdated = 1;
    return 0;
}

uint32_t F32_getSetNextCluster(uint32_t clusterNumber, uint8_t get_set,
        uint32_t clusterEntry) {
    uint16_t FATEntryOffset;
    uint32_t *FATEntryValue;
    uint32_t FATEntrySector;
    uint8_t retry = 0;

    //get sector number of the cluster entry in the FAT
    FATEntrySector = unusedSectors + reservedSectorCount
            + ((clusterNumber * 4) / bytesPerSector);

    //get the offset address in that sector number
    FATEntryOffset = (uint16_t) ((clusterNumber * 4) % bytesPerSector);

    //read the sector into a buffer
    while (retry < 10) {
        if (!SD_readSingleBlock(FATEntrySector))
            break;
        retry++;
    }

    //get the cluster address from the buffer
    FATEntryValue = (uint32_t *) & SD_buffer[FATEntryOffset];

    if (get_set == GET)
        return ((*FATEntryValue) & 0x0fffffff);

    *FATEntryValue = clusterEntry; //for setting new value in cluster entry in FAT

    SD_writeSingleBlock(FATEntrySector);

    return (0);
}

void F32_displayMemory(uint8_t flag, uint32_t memory) {
    uint8_t memoryString[] = "              Bytes"; //19 character long string for memory display
    uint8_t i;
    for (i = 12; i > 0; i--)//converting freeMemory into ASCII string
    {
        if (i == 5 || i == 9) {
            memoryString[i - 1] = ',';
            i--;
        }
        memoryString[i - 1] = (memory % 10) | 0x30;
        memory /= 10;
        if (memory == 0)
            break;
    }
    if (flag == HIGH)
        memoryString[13] = 'K';
#if defined(ENABLE_SERIAL) || defined(ENABLE_SERIAL_POLL)
    serial_puts(memoryString);
#endif
}

void F32_freeMemoryUpdate(uint8_t flag, uint32_t size) {
    uint32_t freeClusters;
    //convert file size into number of clusters occupied
    if ((size % 512) == 0)
        size = size / 512;
    else
        size = (size / 512) + 1;
    if ((size % 8) == 0)
        size = size / 8;
    else
        size = (size / 8) + 1;

    if (freeClusterCountUpdated) {
        freeClusters = F32_getSetFreeCluster(TOTAL_FREE, GET, 0);
        if (flag == ADD)
            freeClusters = freeClusters + size;
        else
            //when flag = REMOVE
            freeClusters = freeClusters - size;
        F32_getSetFreeCluster(TOTAL_FREE, SET, freeClusters);
    }
}


struct dir_Structure* F32_findFiles(uint8_t flag, uint8_t *fileName) {
    uint32_t cluster, sector, firstSector, firstCluster, nextCluster;
    struct dir_Structure *dir;
    uint16_t i;
    uint8_t j;

    cluster = rootCluster; //root cluster

    while (1) {
        firstSector = F32_getFirstSector(cluster);

        for (sector = 0; sector < sectorPerCluster; sector++) {
            SD_readSingleBlock(firstSector + sector);

            for (i = 0; i < bytesPerSector; i += 32) {
                dir = (struct dir_Structure *) &SD_buffer[i];

                if (dir->name[0] == EMPTY) //indicates end of the file list of the directory
                {
#ifdef ENABLE_SD_CARD_DEBUG
                    if (flag == DELETE)
                        serial_puts_f("\r\nFile does not exist!");
#endif
                    return 0;
                }
                if ((dir->name[0] != DELETED)
                        && (dir->attrib != ATTR_LONG_NAME)) {
                    if ((flag == GET_FILE) || (flag == DELETE)) {
                        for (j = 0; j < 11; j++)
                            if (dir->name[j] != fileName[j])
                                break;
                        if (j == 11) {
                            if (flag == GET_FILE) {
                                appendFileSector = firstSector + sector;
                                appendFileLocation = i;
                                appendStartCluster =
                                        (((uint32_t) dir->firstClusterHI) << 16)
                                        | dir->firstClusterLO;
                                fileSize = dir->fileSize;
                                return (dir);
                            } else //when flag = DELETE
                            {
#ifdef ENABLE_SD_CARD_DEBUG
                                //TX_NEWLINE;
                                serial_puts_f("\r\nDeleting..");
                                //TX_NEWLINE;
                                //TX_NEWLINE;
#endif
                                firstCluster = (((uint32_t) dir->firstClusterHI)
                                        << 16) | dir->firstClusterLO;

                                //mark file as 'deleted' in FAT table
                                dir->name[0] = DELETED;
                                SD_writeSingleBlock(firstSector + sector);

                                F32_freeMemoryUpdate(ADD, dir->fileSize);

                                //update next free cluster entry in FSinfo sector
                                cluster = F32_getSetFreeCluster(NEXT_FREE, GET,
                                        0);
                                if (firstCluster < cluster)
                                    F32_getSetFreeCluster(NEXT_FREE, SET,
                                        firstCluster);

                                //mark all the clusters allocated to the file as 'free'
                                while (1) {
                                    nextCluster = F32_getSetNextCluster(
                                            firstCluster, GET, 0);
                                    F32_getSetNextCluster(firstCluster, SET, 0);
                                    if (nextCluster > 0x0ffffff6) {
#ifdef ENABLE_SD_CARD_DEBUG
                                        serial_puts_f("\r\nFile deleted!");
#endif
                                        return 0;
                                    }
                                    firstCluster = nextCluster;
                                }
                            }
                        }
                    } else //when flag = GET_LIST
                    {
#ifdef ENABLE_SD_CARD_DEBUG
                        TX_NEWLINE;
#endif
                        for (j = 0; j < 11; j++) {
#ifdef ENABLE_SD_CARD_DEBUG
                            if (j == 8)
                                serial_putc(' ');
                            serial_putc(dir->name[j]);
#endif
                        }
#ifdef ENABLE_SD_CARD_DEBUG
                        serial_puts("   ");
#endif
                        if ((dir->attrib != 0x10) && (dir->attrib != 0x08)) {
#ifdef ENABLE_SD_CARD_DEBUG
                            serial_puts_f("FILE");
                            serial_puts_f("   ");
#endif
                            F32_displayMemory(LOW, dir->fileSize);
                        } else {
#ifdef ENABLE_SD_CARD_DEBUG
                            serial_putstr(
                                    (dir->attrib == 0x10) ? (int8_t *) "DIR\0" : (int8_t *) "ROOT\0");
#endif
                        }
                    }
                }
            }
        }

        cluster = (F32_getSetNextCluster(cluster, GET, 0));

        if (cluster > 0x0ffffff6)
            return 0;
        if (cluster == 0) {
#ifdef ENABLE_SD_CARD_DEBUG
            serial_puts_f("\r\nError in getting cluster");
#endif
            return 0;
        }
    }
    return 0;
}

uint8_t F32_readFile(uint8_t flag, uint8_t *fileName) {
    struct dir_Structure *dir;
    uint32_t cluster, byteCounter = 0, fileSize, firstSector;
    uint16_t k;
    uint8_t j, error;

    //error = F32_convertFileName(fileName); //convert fileName into FAT format
    //if (error)
    //	return 2;

    dir = F32_findFiles(GET_FILE, fileName); //get the file location
    if (dir == 0) {
        if (flag == READ)
            return (1);
        else
            return (0);
    }

    if (flag == VERIFY)
        return (1); //specified file name is already existing

    cluster = (((uint32_t) dir->firstClusterHI) << 16) | dir->firstClusterLO;

    fileSize = dir->fileSize;
#ifdef ENABLE_SD_CARD_DEBUG
    //TX_NEWLINE;
    //TX_NEWLINE;
#endif
    while (1) {
        firstSector = F32_getFirstSector(cluster);

        for (j = 0; j < sectorPerCluster; j++) {
            SD_readSingleBlock(firstSector + j);

            for (k = 0; k < 512; k++) {
#ifdef ENABLE_SD_CARD_DEBUG
                serial_putc(SD_buffer[k]);
#endif
                if ((byteCounter++) >= fileSize)
                    return 0;
            }
        }
        cluster = F32_getSetNextCluster(cluster, GET, 0);
        if (cluster == 0) {
#ifdef ENABLE_SD_CARD_DEBUG
            serial_puts_f("\r\nError in getting cluster");
#endif
            return 0;
        }
    }
    return 0;
}

uint32_t F32_searchNextFreeCluster(uint32_t startCluster) {
    uint32_t cluster, *value, sector;
    uint8_t i;

    startCluster -= (startCluster % 128); //to start with the first file in a FAT sector
    for (cluster = startCluster; cluster < totalClusters; cluster += 128) {
        sector = unusedSectors + reservedSectorCount
                + ((cluster * 4) / bytesPerSector);
        SD_readSingleBlock(sector);
        for (i = 0; i < 128; i++) {
            value = (uint32_t *) & SD_buffer[i * 4];
            if (((*value) & 0x0fffffff) == 0)
                return (cluster + i);
        }
    }

    return 0;
}


uint8_t F32_writeFile(uint8_t *fileName, uint8_t *dataString) {
    uint8_t j, k, data = 0, error, fileCreatedFlag = 0, start = 0, appendFile =
            0, sector = 0;
    uint16_t i, firstClusterHigh = 0, firstClusterLow = 0; //value 0 is assigned just to avoid warning in compilation
    struct dir_Structure *dir;
    uint32_t cluster, nextCluster, prevCluster, firstSector, clusterCount,
            extraMemory;

    j = F32_readFile(VERIFY, fileName);

    if (j == 1) {
#ifdef ENABLE_SD_CARD_DEBUG
        serial_puts_f("\r\nFile already exists, appending data..");
#endif
        appendFile = 1;
        cluster = appendStartCluster;
        clusterCount = 0;
        while (1) {
            nextCluster = F32_getSetNextCluster(cluster, GET, 0);
            if (nextCluster == FAT32_EOF)
                break;
            cluster = nextCluster;
            clusterCount++;
        }

        sector = (fileSize - (clusterCount * sectorPerCluster * bytesPerSector))
                / bytesPerSector; //last sector number of the last cluster of the file
        start = 1;
    } else if (j == 2)
        return 1; //invalid file name

    else {
#ifdef ENABLE_SD_CARD_DEBUG
        //TX_NEWLINE;
        serial_puts_f("\r\nCreating File..");
#endif
        cluster = F32_getSetFreeCluster(NEXT_FREE, GET, 0);
        if (cluster > totalClusters)
            cluster = rootCluster;

        cluster = F32_searchNextFreeCluster(cluster);
        if (cluster == 0) {
#ifdef ENABLE_SD_CARD_DEBUG
            //TX_NEWLINE;
            serial_puts_f("\r\nNo free cluster!");
#endif
            return 1;
        }
        F32_getSetNextCluster(cluster, SET, FAT32_EOF); //last cluster of the file, marked EOF

        firstClusterHigh = (uint16_t) ((cluster & 0xffff0000) >> 16);
        firstClusterLow = (uint16_t) (cluster & 0x0000ffff);
        fileSize = 0;
    }

    k = 0;

    while (1) {
        if (start) {
            start = 0;
            SD_startBlock = F32_getFirstSector(cluster) + sector;
            SD_readSingleBlock(SD_startBlock);
            i = fileSize % bytesPerSector;
            j = sector;
        } else {
            SD_startBlock = F32_getFirstSector(cluster);
            i = 0;
            j = 0;
        }

        do {
            data = dataString[k++];
#ifdef ENABLE_SD_CARD_DEBUG
            serial_putchar(data); // was data
#endif
            SD_buffer[i++] = data;
            fileSize++;

            if (i >= 512) //though 'i' will never become greater than 512, it's kept here to avoid
            { //infinite loop in case it happens to be greater than 512 due to some data corruption
                i = 0;
                error = SD_writeSingleBlock(SD_startBlock);
                j++;
                if (j == sectorPerCluster) {
                    j = 0;
                    break;
                }
                SD_startBlock++;
            }
        } while ((data != '\n') && (k < MAX_STRING_SIZE)); //stop when newline character is found
        //or when string size limit reached

        if ((data == '\n') || (k >= MAX_STRING_SIZE)) {
            for (; i < 512; i++) //fill the rest of the buffer with 0x00
                SD_buffer[i] = 0x00;
            error = SD_writeSingleBlock(SD_startBlock);

            break;
        }

        prevCluster = cluster;

        cluster = F32_searchNextFreeCluster(prevCluster); //look for a free cluster starting from the current cluster

        if (cluster == 0) {
#ifdef ENABLE_SD_CARD_DEBUG
            //TX_NEWLINE;
            serial_puts_f("\r\nNo free cluster!");
#endif
            return 1;
        }

        F32_getSetNextCluster(prevCluster, SET, cluster);
        F32_getSetNextCluster(cluster, SET, FAT32_EOF); //last cluster of the file, marked EOF
    }

    F32_getSetFreeCluster(NEXT_FREE, SET, cluster); //update FSinfo next free cluster entry

    error = getDateTime_FAT(); //get current date & time from the RTC
    if (error) {
        dateFAT = 0;
        timeFAT = 0;
    }

    if (appendFile) //executes this loop if file is to be appended
    {
        SD_readSingleBlock(appendFileSector);
        dir = (struct dir_Structure *) &SD_buffer[appendFileLocation];

        dir->lastAccessDate = 0; //date of last access ignored
        dir->writeTime = timeFAT; //setting new time of last write, obtained from RTC
        dir->writeDate = dateFAT; //setting new date of last write, obtained from RTC
        extraMemory = fileSize - dir->fileSize;
        dir->fileSize = fileSize;
        SD_writeSingleBlock(appendFileSector);
        F32_freeMemoryUpdate(REMOVE, extraMemory); //updating free memory count in FSinfo sector;

#ifdef ENABLE_SD_CARD_DEBUG
        //TX_NEWLINE;
        serial_puts_f("\r\nFile appended!");
#endif
        return 0;
    }

    //executes following portion when new file is created

    prevCluster = rootCluster; //root cluster

    while (1) {
        firstSector = F32_getFirstSector(prevCluster);

        for (sector = 0; sector < sectorPerCluster; sector++) {
            SD_readSingleBlock(firstSector + sector);

            for (i = 0; i < bytesPerSector; i += 32) {
                dir = (struct dir_Structure *) &SD_buffer[i];

                if (fileCreatedFlag) //to mark last directory entry with 0x00 (empty) mark
                { //indicating end of the directory file list
                    //dir->name[0] = EMPTY;
                    //SD_writeSingleBlock (firstSector + sector);
                    return 0;
                }

                if ((dir->name[0] == EMPTY) || (dir->name[0] == DELETED)) //looking for an empty slot to enter file info
                {
                    for (j = 0; j < 11; j++)
                        dir->name[j] = fileName[j];
                    dir->attrib = ATTR_ARCHIVE; //setting file attribute as 'archive'
                    dir->NTreserved = 0; //always set to 0
                    dir->timeTenth = 0; //always set to 0
                    dir->createTime = timeFAT; //setting time of file creation, obtained from RTC
                    dir->createDate = dateFAT; //setting date of file creation, obtained from RTC
                    dir->lastAccessDate = 0; //date of last access ignored
                    dir->writeTime = timeFAT; //setting new time of last write, obtained from RTC
                    dir->writeDate = dateFAT; //setting new date of last write, obtained from RTC
                    dir->firstClusterHI = firstClusterHigh;
                    dir->firstClusterLO = firstClusterLow;
                    dir->fileSize = fileSize;

                    SD_writeSingleBlock(firstSector + sector);
                    fileCreatedFlag = 1;
#ifdef ENABLE_SD_CARD_DEBUG
                    //TX_NEWLINE;
                    //TX_NEWLINE;
                    serial_puts_f("\r\nFile Created! ");
#endif

                    F32_freeMemoryUpdate(REMOVE, fileSize); //updating free memory count in FSinfo sector

                }
            }
        }

        cluster = F32_getSetNextCluster(prevCluster, GET, 0);

        if (cluster > 0x0ffffff6) {
            if (cluster == FAT32_EOF) //this situation will come when total files in root is multiple of (32*sectorPerCluster)
            {
                cluster = F32_searchNextFreeCluster(prevCluster); //find next cluster for root directory entries
                F32_getSetNextCluster(prevCluster, SET, cluster); //link the new cluster of root to the previous cluster
                F32_getSetNextCluster(cluster, SET, FAT32_EOF); //set the new cluster as end of the root directory
            }
            else {
#ifdef ENABLE_SD_CARD_DEBUG
                serial_puts_f("\r\nEnd of Cluster Chain");
#endif
                return 1;
            }
        }
        if (cluster == 0) {
#ifdef ENABLE_SD_CARD_DEBUG
            serial_puts_f("\r\nError in getting cluster");
#endif
            return 1;
        }

        prevCluster = cluster;
    }

    return 0;
}

void F32_deleteFile(uint8_t *fileName) {
    //uint8_t error;

    //error = F32_convertFileName(fileName);
    //if (error)
    //	return;

    F32_findFiles(DELETE, fileName);
}



#endif	/* ROSSO_F32_H */

