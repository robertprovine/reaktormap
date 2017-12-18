#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

/* REAKTORMAP

   Generates a Reaktor Sample Map from a templated folder structure loaded
   with custom samples. Samples are mapped to different velocities for up to
   128 keys, and the number of samples per key are also summarized in a
   comma separated text file that can be used in a table structure within
   Reaktor Core. This has only been tested with Reaktor 6.0.1 (R1526).

   DIRECTIONS

   1. Create a new directory with a unique name of your choice. We will refer
      to this directory as the 'main directory' in further directions.

   2. Inside this main directory, create one or more 'key' directories (no more
      than 128 directories total), and name them as follows:

      NUMBER (between 0 - 127) + UNDERSCORE + CUSTOM NAME

      correct examples: 36_Kicks  4_Swells  08_Snares  126_Wah_Sounds
      incorrect examples: _36Kicks  Snares  25HiHats

      Each numerical prefix represents the note number to which the velocities
      will be mapped. Make sure there are no duplicate number prefixes. The
      prefixes should be distinct. At this point, I would also recommend
      creating a copy of the main directory and its contents elsewhere as a
      template for future use.

   3. Drag/copy/save audio files of your choice into these key directories (no
      more than 127 total audio files per key directory)

      example: drag/copy 25 kick sounds into 36_Kicks, 40 snares into 37_Snares

   4. Open Terminal and navigate to the main directory with the cd command.
      Run reaktormap. This will create a Reaktor Sample Map file and
      corresponding text file. Each note number (mapped by the numerical
      prefix in each key directory name) can store up to 127 different sounds
      (sounds accessed by velocity values between 1 and 127), allowing a
      maximum total of 16,256 sounds to be loaded (128 x 127). The text file
      generated contains 128 comma separated values, each representing the
      number of audio files per key. This file can be loaded into the table
      structure in Reaktor Core to provide modular access to these values.
      This is helpful in order to prevent out of bounds indexing of
      velocities in Reaktor Sample Maps.

   Notes: Sample maps refer to the sound files by absolute path, so if
          the main directory is moved, reaktormap must be run again to update
          the new absolute paths.

          Don't forget to load the corresponding text file into a Reaktor
          Core table module each time you replace a sample map (if you wish
          to use the table data).

          Sample maps (.map) and text files (.txt) can moved elsewhere once they've been
          created if you wish. */

typedef unsigned char BYTE;

typedef struct Ref_ {
    int pathSize; 
    char *path; 
    int low;
    int high;
    int velocityLow;
    int velocityHigh;
    int root;
    struct Ref_ *next;
} Ref;

typedef struct {
    int size;
    Ref *head;
    Ref *tail;
} RefList;

typedef struct {
    int numberOfRefs;
    int sumOfPathLengths;
    int keyLengths[128];
    RefList *refs;
} ReaktorMap;

void initRefList(RefList *list);
int addRef(RefList *list,
    int pathSize,
    char *path, 
    int low,
    int high,
    int velocityLow,
    int velocityHigh,
    int root);
void deleteRefList(RefList *list);

int getPrefixNumber(char *str);
int isDirectory(const char *path);

int loadReaktorMap(ReaktorMap *map);
int createBuffer(ReaktorMap *map, BYTE **buf);
int createSampleMapFile(BYTE *buf, int bufSize);
int createTextFile(int keyLengths[]);
void deleteBuffer(BYTE *buf);

int main()
{
    ReaktorMap *map = (ReaktorMap *)malloc(sizeof(ReaktorMap));
    map->refs = (RefList *)malloc(sizeof(RefList));
    BYTE *buf;
    BYTE **buf_p = &buf;
    int bufSize;

    loadReaktorMap(map);
    bufSize = createBuffer(map, buf_p);
    createSampleMapFile(buf, bufSize);
    createTextFile(map->keyLengths);
    deleteBuffer(buf);
}

void initRefList(RefList *list)
{
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

int addRef(RefList *list,
    int pathSize,
    char *path, 
    int low,
    int high,
    int velocityLow,
    int velocityHigh,
    int root)
{
    Ref *newRef;

        if ((newRef = (Ref *)malloc(sizeof(Ref))) == NULL)
            return -1;

        newRef->pathSize = pathSize;
        newRef->path = path;
        newRef->low = low;
        newRef->high = high;
        newRef->velocityLow = velocityLow;
        newRef->velocityHigh = velocityHigh;
        newRef->root = root;

        newRef->next = NULL;

        if (list->head == NULL)
            list->head = newRef;

        if (list->tail != NULL)
            list->tail->next = newRef;
        list->tail = newRef;

        list->size++;

        return 0;
}

void deleteRefList(RefList *list)
{
    Ref *oldRef;

    while (list->head != NULL) {
        oldRef = list->head;
        list->head = list->head->next;
        free(oldRef->path);
        free(oldRef);
        list->size--;
    }
}

int getPrefixNumber(char *str)
{
    int i = 0;
    char s[4];
    int len = strlen(str);

    while (i < len) {
        if (str[i] == '_') {
            s[i] = '\0';
            return atoi(s);
        }
        if (isdigit(str[i])) {
            s[i] = str[i];
            i++;
        }
        else
            return -1;
    }

    printf("Missing prefix\n");
    return -1;
}

int isDirectory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int loadReaktorMap(ReaktorMap *map)
{
    int i;
    int pfx;
    int vel = 1;
    DIR *d1, *d2;
    struct dirent *entry1, *entry2;
    char path_t[PATH_MAX + 1];
    char *path;
    int pathSize;

    path_t[0] = '\0';

    map->numberOfRefs = 0;
    map->sumOfPathLengths = 0;

    for (i = 0; i < 128; i++)
        map->keyLengths[i] = 0;

    initRefList(map->refs);

    if ((d1 = opendir(".")) == NULL) {
        fprintf(stderr, "Unable to read current directory\n");
        return -1;
    }

    while ((entry1 = readdir(d1)) != NULL) {
        if (isDirectory(entry1->d_name) &&
            (pfx = getPrefixNumber(entry1->d_name)) != -1) {
            if ((d2 = opendir(entry1->d_name)) == NULL) {
                fprintf(stderr, "Unable to open directory\n");
                return -1;
            }
            while ((entry2 = readdir(d2)) != NULL) {
                if (vel > 127) {
                    fprintf(stderr,
                        "NOTE: more than 127 files in /%s\n", entry1->d_name);
                    break;
                }
                if ((strcmp(entry2->d_name, ".") != 0) &&
                    (strcmp(entry2->d_name, "..") != 0) &&
                    (entry2->d_name[0] != '.')) {
                    
                    map->keyLengths[pfx]++;
                    map->numberOfRefs++;
                    realpath(entry1->d_name, path_t);
                    pathSize = strlen(path_t) + 1 + strlen(entry2->d_name);
                    path = (char *)malloc((pathSize + 1) * sizeof(char));
                    strcpy(path, path_t);
                    strcat(path, "/");
                    strcat(path, entry2->d_name);
                    map->sumOfPathLengths += pathSize;
                    addRef(map->refs, pathSize, path, pfx, pfx, vel, vel, pfx);
                    vel++;
                }
            }

            vel = 1;
        }
    }

    closedir(d1);
    closedir(d2);

    return 0;
}

int createBuffer(ReaktorMap *map, BYTE **buffer)
{
    int bufSize = 50 + map->numberOfRefs * 108 + map->sumOfPathLengths;
    *buffer = (BYTE *)malloc(bufSize * sizeof(BYTE));
    int buf_idx;
    int i;
    Ref *ref = map->refs->head;
    
    (*buffer)[0] = 0x00;
    (*buffer)[1] = 0x00;
    (*buffer)[2] = 0x00;
    (*buffer)[3] = 0x00;
    (*buffer)[4] = 0xD8;
    (*buffer)[5] = 0x02;
    (*buffer)[6] = 0x00;
    (*buffer)[7] = 0x00;
    (*buffer)[8] = 0x4E;
    (*buffer)[9] = 0x49;
    (*buffer)[10] = 0x4D;
    (*buffer)[11] = 0x61;
    (*buffer)[12] = 0x70;
    (*buffer)[13] = 0x46;
    (*buffer)[14] = 0x69;
    (*buffer)[15] = 0x6C;
    (*buffer)[16] = 0x65;
    (*buffer)[17] = 0x02;
    (*buffer)[18] = 0x00;
    (*buffer)[19] = 0x00;
    (*buffer)[20] = 0x00;
    (*buffer)[21] = 0x01;
    (*buffer)[22] = 0x6D;
    (*buffer)[23] = 0x61;
    (*buffer)[24] = 0x70;
    (*buffer)[25] = 0x70;
    (*buffer)[26] = 0x0C;
    (*buffer)[27] = 0x00;
    (*buffer)[28] = 0x00;
    (*buffer)[29] = 0x00;
    (*buffer)[30] = 0x01;
    (*buffer)[31] = 0x00;
    (*buffer)[32] = 0x00;
    (*buffer)[33] = 0x00;
    (*buffer)[34] = 0x01;
    (*buffer)[35] = 0x00;
    (*buffer)[36] = 0x00;
    (*buffer)[37] = 0x00;
    (*buffer)[38] = 0x00;
    (*buffer)[39] = 0x00;
    (*buffer)[40] = 0x00;
    (*buffer)[41] = 0x00;
    (*buffer)[42] = 0x00;
    (*buffer)[43] = 0x00;
    (*buffer)[44] = 0x00;
    (*buffer)[45] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    (*buffer)[46] = (BYTE)map->numberOfRefs;
    (*buffer)[47] = (BYTE)(map->numberOfRefs >> 8);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    (*buffer)[48] = 0x00;
    (*buffer)[49] = 0x00;

    buf_idx = 50;
    i = 0;
    while (ref != NULL) {
        (*buffer)[buf_idx++] = 0x01;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = (BYTE)ref->pathSize;
        (*buffer)[buf_idx++] = (BYTE)(ref->pathSize >> 8);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        for (i = 0; ref->path[i] != '\0'; i++)
            (*buffer)[buf_idx++] = ref->path[i];
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x65;
        (*buffer)[buf_idx++] = 0x6E;
        (*buffer)[buf_idx++] = 0x74;
        (*buffer)[buf_idx++] = 0x72;
        (*buffer)[buf_idx++] = 0x54;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x02;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = (BYTE)ref->low;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = (BYTE)ref->high;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = (BYTE)ref->velocityLow;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = (BYTE)ref->velocityHigh;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        (*buffer)[buf_idx++] = (BYTE)ref->root;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        for (i = 0; i < 23; i++)
            (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x04;
        for (i = 0; i < 31; i++)
            (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x55;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x01;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;
        (*buffer)[buf_idx++] = 0x00;

        ref = ref->next;
    }

    return bufSize;
}

int createSampleMapFile(BYTE *buf, int bufSize)
{
    FILE* f;
    int numBytes;

    f = fopen("map.map", "wb");

    if (f == NULL)
    {
        printf("Could not open \"map.map\" destination file\n");
        exit(0);
    }

    fwrite(buf, sizeof(BYTE), bufSize, f);

    fclose(f);

    return 0;
}

int createTextFile(int keyLengths[])
{
    FILE* f;
    int numBytes;
    int i;
    char pfxStr[4];

    f = fopen("table.txt", "wb");

    if (f == NULL)
    {
        printf("Could not open \"table.txt\" destination file\n");
        exit(0);
    }

    for (i = 0; i < 127; i++) {
        sprintf(pfxStr, "%d", keyLengths[i]);
        fputs(pfxStr, f);
        fputc(',', f);
    }
    sprintf(pfxStr, "%d", keyLengths[i]);
    fputs(pfxStr, f);

    fclose(f);

    return 0;
}

void deleteBuffer(BYTE *buf)
{
    free(buf);
}
