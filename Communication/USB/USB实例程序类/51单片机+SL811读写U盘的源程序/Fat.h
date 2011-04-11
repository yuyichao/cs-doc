unsigned long FirstSectorofCluster(unsigned int n);
unsigned int ThisFatSecNum(unsigned int clusterNum);
unsigned int ThisFatEntOffset(unsigned int clusterNum);
unsigned int GetNextClusterNum(unsigned int clusterNum);
unsigned int GetClusterNumFromSectorNum(unsigned long sectorNum);
//unsigned long GetSecNumFromPointer(void);
unsigned char GoToPointer(unsigned long pointer);
unsigned int GetFreeCusterNum(void);
unsigned int CreateClusterLink(unsigned int currentCluster);
unsigned char DeleteClusterLink(unsigned int clusterNum);
