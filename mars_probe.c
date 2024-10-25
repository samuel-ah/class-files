//-----------------------------------------
// NAME: Sam Heppner
// STUDENT NUMBER: 7998801
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Tim Zapp
// ASSIGNMENT: 2, QUESTION: 1, 2
// 
// REMARKS: This program simulates a probe exploring a series of simple
//          "zones" in order to find resources. The probe explores the zone,
//          reads in the resource data, and prints a summary of its findings.
//
//-----------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_DIMENSION 20
#define BUFFERSIZE 128
#define INPUT_PREFIX_SIZE 5

const char WALL = 'W';
const char OPEN_SPACE = '0';
const char VISITED = '.';
const char PROBE = 'L';
const char UNREADABLE = 'X';
const char NULL_TERMINATOR = '\0';
const char NEWLINE = '\n';

typedef struct CELL Cell;
typedef struct CELL_NODE CellNode;
typedef struct PROBE ProbeData;
typedef struct ZONE ZoneData;
typedef struct RESOURCE ResourceData;

typedef enum BOOL Boolean;

// Function prototypes //

// Memory allocation functions
Cell* makeCell(const int row, const int col);
CellNode* makeNode(Cell *cell);
ZoneData* makeZone();
ProbeData* makeProbe(ZoneData *zone);

// Cleanup functions
void destroyProbeList(ProbeData *curr);

// Stack functions
Boolean noMoreCells();
Cell* nextCell();
void addCell(Cell *toAdd);
void addNeighbour(ZoneData *zone, const int row, const int col);

// Output functions
void printZoneSummary(ZoneData *zone);
void printZone(ZoneData *zone, char area[MAX_DIMENSION][MAX_DIMENSION]);
void printProbeSummary();

// Data processing functions
Boolean loadZone();
Boolean isResource(char c);
void exploreZone(ZoneData *zone);
void computeResources(ZoneData *zone, Cell *cell);
void estimateResources(ZoneData *zone);
void insertInOrder(ZoneData *zone);
Cell* findProbe(ZoneData *zone);

// DBC functions
void checkZone(ZoneData *zone);
void checkState();
Boolean validProbeCell(ZoneData *zone);

// Enumerations //

enum BOOL {
    false,
    true   
};

// Structs //

struct CELL {
    int row;
    int col;
};

struct CELL_NODE {
    Cell *cell;
    CellNode *next;
};

struct ZONE {
    char zoneArea[MAX_DIMENSION][MAX_DIMENSION];
    char probedMapping[MAX_DIMENSION][MAX_DIMENSION];
    int zoneID;
    int zoneRows;
    int zoneCols;
    Boolean initialized;
    ResourceData *resources;
};

struct RESOURCE {
    int areasAvail;
    int richAreas;
    int unreadableAreas;
    int accessibleResources;
    int estPotentialResources;
    int totalResources;
};

struct PROBE {
    ZoneData *probedZone;
    ProbeData *next;
};

// File variables //

CellNode *top = NULL;
ProbeData *probe = NULL;

// Function definitions //

//------------------------------------------------------
// makeCell
//
// PURPOSE: This function returns a pointer to newly allocated memory containing
//          an instance of the Cell struct.
// INPUT PARAMETERS:
// row (int): The row of the cell.
// col (int): The column of the cell.
// OUTPUT PARAMETERS:
// Return: A pointer to the newly allocated memory, or NULL on failure to allocate.
//------------------------------------------------------

Cell* makeCell(const int row, const int col) {
    Cell *new = (Cell *) malloc(sizeof(Cell));

    assert(new != NULL);

    if (new != NULL) {
        new->row = row;
        new->col = col;
    }

    return new;
}

//------------------------------------------------------
// makeNode
//
// PURPOSE: This function returns a pointer to newly allocated memory containing
//          an instance of the CellNode struct.
// INPUT PARAMETERS:
// cell (Cell*): A pointer to the cell to be contained within the newly created
//               node.
// OUTPUT PARAMETERS:
// Return: A pointer to the newly allocated memory, or NULL on failure to allocate.
//------------------------------------------------------

CellNode* makeNode(Cell *cell) {
    CellNode *new = (CellNode *) malloc(sizeof(CellNode));

    assert(new != NULL);

    if (new != NULL) {
        new->cell = cell;
        new->next = NULL;
    }

    return new;
}

//------------------------------------------------------
// makeZone
//
// PURPOSE: This function returns a pointer to newly allocated memory containing
//          an instance of the ZoneData struct.
// INPUT PARAMETERS:
// (-)
// OUTPUT PARAMETERS:
// Return: A pointer to the newly allocated memory, or NULL on failure to allocate.
//------------------------------------------------------

ZoneData* makeZone() {
    ZoneData *new = (ZoneData *) malloc(sizeof(ZoneData));

    assert(new != NULL);

    if(new != NULL) {
        new->initialized = false;
        new->zoneRows = 0;
        new->zoneCols = 0;
        new->resources = NULL;
    }

    return new;
}

//------------------------------------------------------
// makeProbe
//
// PURPOSE: This function returns a pointer to newly allocated memory containing
//          an instance of the ProbeData struct.
// INPUT PARAMETERS:
// row (int): The row of the cell.
// col (int): The column of the cell.
// OUTPUT PARAMETERS:
// Return: A pointer to the newly allocated memory, or NULL on failure to allocate.
//------------------------------------------------------

ProbeData* makeProbe(ZoneData *zone) {
    ProbeData *new = (ProbeData *) malloc(sizeof(ProbeData));

    assert(new != NULL);

    if (new != NULL) {
        new->probedZone = zone;
        new->next = NULL;
    }

    return new;
}

//------------------------------------------------------
// addCell
//
// PURPOSE: This function adds a given Cell to the top of the stack of cells to be
//          searched.
// INPUT PARAMETERS:
// toAdd (Cell*): Pointer to the cell to be added.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void addCell(Cell *toAdd) {
    CellNode *new = makeNode(toAdd);

    assert(new != NULL);

    if (new != NULL) {
        if (noMoreCells()) {
            top = new;
        } else {
            new->next = top;
            top = new;
        }
    }
}

//------------------------------------------------------
// nextCell
//
// PURPOSE: This function removes the top Cell from the stack of Cells to be searched.
// INPUT PARAMETERS:
// (-)
// OUTPUT PARAMETERS:
// Return: A pointer to the Cell that was popped off the stack, or NULL if there are no
//         Cells available to pop.
//------------------------------------------------------

Cell* nextCell() {
    Cell *popped = NULL;
    CellNode *toFree;

    if(top != NULL) {
        popped = top->cell;
        toFree = top;

        if (top->next != NULL) {
            top = top->next;
        } else {
            top = NULL;
        }

        free(toFree);
    }

    return popped;
}

//------------------------------------------------------
// noMoreCells
//
// PURPOSE: This function returns true if there are no more Cells to be searched.
// INPUT PARAMETERS:
// (-)
// OUTPUT PARAMETERS:
// Return: true if there are no more cells to be searched.
//------------------------------------------------------

Boolean noMoreCells() {
    return top == NULL;
}

//------------------------------------------------------
// isResource
//
// PURPOSE: This function returns true if a given character represents an accessible resource
//          rich area.
// INPUT PARAMETERS:
// c (char): The character value to be checked.
// OUTPUT PARAMETERS:
// Return: true if the character represents an accessible resource rich area.
//------------------------------------------------------

Boolean isResource(char c) {
    // character has numeric value between 0 and 9
    return (1 <= c - '0') && (c - '0' <= 9);
}

//------------------------------------------------------
// printZone
//
// PURPOSE: This function outputs a visual representation of a two-dimensional array
//          of characters from a given zone.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone from which the area is to be printed.
// area (char[][]): Pointer to the area to be printed.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void printZone(ZoneData *zone, char area[MAX_DIMENSION][MAX_DIMENSION]) {
    checkZone(zone);

    if (zone != NULL && zone->initialized) {
        for (int i = 0; i < zone->zoneRows; i++) {
            for (int j = 0; j < zone->zoneCols; j++) {
                printf("%c ", area[i][j]);
            }

            printf("\n");
        }

        printf("\n");
    } else {
        printf("Invalid zone.\n");
    }
}

//------------------------------------------------------
// loadZone
//
// PURPOSE: This function attempts to load a zone into the Probe system from standard
//          input, and then analyze it.
// INPUT PARAMETERS:
// stdin
// OUTPUT PARAMETERS:
// Return: true if a zone was successfully loaded.
//------------------------------------------------------

Boolean loadZone() {
    char prefix[INPUT_PREFIX_SIZE];
    char zoneArea[MAX_DIMENSION][MAX_DIMENSION] = {0};
    char c, temp;
    int zoneID, zoneRows, zoneCols;
    Boolean readZoneSuccess = true;
    ZoneData *zone = makeZone();

    assert(zone != NULL);

    if (zone == NULL) {
        readZoneSuccess = false;
    }

    // remove "Zone " from input stream
    fgets(prefix, INPUT_PREFIX_SIZE, stdin);

    // load the zone ID
    if (scanf(" %d", &zoneID) < 1) {
        readZoneSuccess = false;
    }

    getchar();

    // load the number of rows and columns
    if (scanf(" %d %d", &zoneRows, &zoneCols) < 2) {
        readZoneSuccess = false;
    }

    getchar();

    if (readZoneSuccess) {
        for (int i = 0; i < zoneRows && readZoneSuccess; i++) {
            for (int j = 0; j < zoneCols && readZoneSuccess; j++) {
                c = getchar();
                temp = getchar();

                // zone is invalid if EOF is reached before the expected number of rows and columns
                // was reached, or if a row contains an invalid number of columns
                if (c == EOF && i != zoneRows - 1 && j != zoneCols - 1) {
                    readZoneSuccess = false;
                } else if (temp != NEWLINE && temp != EOF && j == zoneCols - 1) {
                    readZoneSuccess = false;
                } else {
                    zoneArea[i][j] = c;
                }
            }
        }
    }

    if (readZoneSuccess) {
        zone->zoneID = zoneID;
        zone->zoneRows = zoneRows;
        zone->zoneCols = zoneCols;

        // copy the zone data into a zone struct
        for (int i = 0; i < MAX_DIMENSION; i++) {
            strcpy(zone->zoneArea[i], zoneArea[i]);
            strcpy(zone->probedMapping[i], zoneArea[i]);
        }

        exploreZone(zone);

        insertInOrder(zone);

        checkZone(zone);
    }

    return readZoneSuccess;
}

//------------------------------------------------------
// exploreZone
//
// PURPOSE: This function explores a given zone and estimates the number of unreadable resources
//          in it.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone to be explored.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void exploreZone(ZoneData *zone) {
    Cell *probeCell = findProbe(zone);

    assert(probeCell != NULL);

    // only explore the zone if it has a unique probe cell
    if (probeCell != NULL) {
        computeResources(zone, probeCell);
        estimateResources(zone);
    }
}

//------------------------------------------------------
// estimateResources
//
// PURPOSE: This function estimates the unreadable resources in a given zone, based on
//          the total number of resource rich areas and the number of accessible resources.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone on which the estimation is to be performed.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void estimateResources(ZoneData *zone) {
    assert(zone != NULL);
    assert(zone->resources != NULL);

    if (zone != NULL && zone->resources != NULL) {
        // avoid division by 0 in the case where all accessible areas are unreadable
        if (zone->resources->richAreas - zone->resources->unreadableAreas >= 1) {
            zone->resources->estPotentialResources = zone->resources->accessibleResources 
                                                     / (zone->resources->richAreas - zone->resources->unreadableAreas);
        } else {
            zone->resources->estPotentialResources = 0;
        }

        zone->resources->totalResources = zone->resources->estPotentialResources 
                                                   + zone->resources->accessibleResources;
    }
}

//------------------------------------------------------
// computeResources
//
// PURPOSE: This function implements the backtracking algorithm to compute the number of
//          resources and accessible areas in a given zone.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone to compute.
// cell (Cell*): Pointer to the cell from which to start the algorithm.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void computeResources(ZoneData *zone, Cell *cell) {
    int numResources = 0, unreadable = 0, areasAvail = 0;
    int richAreas = 0;
    Cell *curr = cell;
    Cell *temp;
    ResourceData *zoneResources = (ResourceData *) malloc(sizeof(ResourceData));

    assert(zone != NULL);
    assert(cell != NULL);
    assert(zoneResources != NULL);

    while (curr != NULL && zoneResources != NULL) {
        areasAvail++;

        // determine the value of the current cell and update findings accordingly
        if (zone->probedMapping[curr->row][curr->col] == UNREADABLE) {
            unreadable++;
            richAreas++;
        } else if (isResource(zone->probedMapping[curr->row][curr->col])) {
            numResources += zone->probedMapping[curr->row][curr->col] - '0';
            richAreas++;
        }
        
        zone->probedMapping[curr->row][curr->col] = VISITED;

        // add the unvisited and accessible neighbours of the current cell
        addNeighbour(zone, curr->row - 1, curr->col);
        addNeighbour(zone, curr->row + 1, curr->col);
        addNeighbour(zone, curr->row, curr->col - 1);
        addNeighbour(zone, curr->row, curr->col + 1);

        // proceed to the next available cell
        temp = curr;
        curr = nextCell();
        free(temp);
    }

    if (zoneResources != NULL) {
        // initialize the resource struct for the current zone
        zone->resources = zoneResources;
        zoneResources->areasAvail = areasAvail;
        zoneResources->richAreas = richAreas;
        zoneResources->unreadableAreas = unreadable;
        zoneResources->accessibleResources = numResources;
        zoneResources->estPotentialResources = 0;
        zoneResources->totalResources = 0;
        zone->initialized = true;
    }
}

//------------------------------------------------------
// addNeighbour
//
// PURPOSE: This function adds a cell to the stack of cells to be explored if it has
//          not already been visited and is not a wall.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone the cell originates from.
// row (int): The row of the cell.
// col (int): The column of the cell.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void addNeighbour(ZoneData *zone, const int row, const int col) {
    Cell *temp;

    if (zone->probedMapping[row][col] != WALL && zone->probedMapping[row][col] != VISITED) {
        temp = makeCell(row, col);

        assert(temp != NULL);

        if(temp != NULL) {
            addCell(temp);
        }
    }
}

//------------------------------------------------------
// findProbe
//
// PURPOSE: This function finds the Cell that contains the starting location of a probe.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone to be searched.
// OUTPUT PARAMETERS:
// Return: A pointer to the cell that was found, or NULL if there is no probe, or multiple
//         probes exist.
//------------------------------------------------------

Cell* findProbe(ZoneData *zone) {
    Cell *probeCell = NULL;
    Boolean validCell;

    assert(zone != NULL);

    validCell = validProbeCell(zone);

    assert(validCell);

    if (zone != NULL && validCell) {
        for (int i = 1; i < zone->zoneRows - 1 && probeCell == NULL; i++) {
            for (int j = 1; j < zone->zoneCols - 1 && probeCell == NULL; j++) {
                if (zone->zoneArea[i][j] == PROBE) {
                   probeCell = makeCell(i, j);
                }
            }
        }
    }

    return probeCell;
}

//------------------------------------------------------
// validProbeCell
//
// PURPOSE: This function returns true if there is a single valid cell in a given
//          zone containing a probe.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone to be searched.
// OUTPUT PARAMETERS:
// Return: true if a single valid probe was found.
//------------------------------------------------------

Boolean validProbeCell(ZoneData *zone) {
    int numProbeCells = 0;
    
    for (int i = 1; i < zone->zoneRows - 1; i++) {
        for (int j = 1; j < zone->zoneCols - 1; j++) {
            if (zone->zoneArea[i][j] == PROBE) {
                numProbeCells++;
            }
        }
    }

    return numProbeCells == 1;
}

//------------------------------------------------------
// checkZone
//
// PURPOSE: This function validates an existing zone by asserting that all of the essential values
//          of the zone are initialized and ready to be read from.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone to be validated.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void checkZone(ZoneData *zone) {
    assert(zone != NULL);
    assert(zone->resources != NULL);
    assert(zone->initialized);
}

//------------------------------------------------------
// printZoneSummary
//
// PURPOSE: This function prints a summary of a given Zone and its resource data.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone to be printed.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void printZoneSummary(ZoneData *zone) {
    checkZone(zone);
    
    printf("Zone %d Map:\n\n", zone->zoneID);
    
    printZone(zone, zone->zoneArea);

    printf("Summary Zone %d:\n", zone->zoneID);
    printf("-> Available areas: %d\n", zone->resources->areasAvail);
    printf("-> Resource rich areas: %d\n", zone->resources->richAreas);
    printf("-> Resources accessible: %d\n", zone->resources->accessibleResources);
    printf("-> Unreadable areas: %d\n\n", zone->resources->unreadableAreas);

    printf("Probed Mapping:\n\n");

    printZone(zone, zone->probedMapping);
}

//------------------------------------------------------
// insertInOrder
//
// PURPOSE: This function inserts an explored zone into the probe system,
//          ordered by number of total resources.
// INPUT PARAMETERS:
// zone (ZoneData*): Pointer to the zone to be added to the probe system.
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void insertInOrder(ZoneData *zone) {
    ProbeData *toAdd = makeProbe(zone);
    ProbeData *curr = probe;

    assert(toAdd != NULL);
    checkZone(zone);

    if (toAdd != NULL && zone != NULL) {
        // case where there are no zones previously in the probe system
        if (probe == NULL) {
            probe = toAdd;
        // case where zone being added has more resources than the top of the probe system
        } else if (probe->probedZone->resources->totalResources <= zone->resources->totalResources) {
            toAdd->next = probe;
            probe = toAdd;
        // case where zone to add will be added after the top zone        
        } else {
            while (curr->next != NULL 
                   && curr->next->probedZone->resources->totalResources > zone->resources->totalResources) {
                curr = curr->next; 
            }

            if (curr->next == NULL) {
                curr->next = toAdd;
            } else {        
                toAdd->next = curr->next;
                curr->next = toAdd;
            }
        }
    }
}

//------------------------------------------------------
// printProbeSummary
//
// PURPOSE: This function traverses the probe system and prints a summary of each
//          explored zone in order of number of total resources, then cleans up the list.
// INPUT PARAMETERS:
// (-)
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void printProbeSummary() {
    ProbeData *curr = probe;

    if (curr != NULL) {
        printf("Zones by number of total resources available: \n\n");
    }

    while (curr != NULL) {
        printZoneSummary(curr->probedZone);
        curr = curr->next;
    }

    destroyProbeList(probe);
}

//------------------------------------------------------
// checkState
//
// PURPOSE: This function traverses the probe system and validates each zone it contains, and
//          ensures that there are no stray Cells left in the stack.
// INPUT PARAMETERS:
// (-)
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void checkState() {
    ProbeData *curr = probe;

    assert(noMoreCells());

    while (curr != NULL) {
        checkZone(curr->probedZone);
        curr = curr->next;
    }
}

//------------------------------------------------------
// destroyProbeList
//
// PURPOSE: This function frees the memory occupied by the probe system.
// INPUT PARAMETERS:
// (-)
// OUTPUT PARAMETERS:
// (-)
//------------------------------------------------------

void destroyProbeList(ProbeData *curr) {
    if (curr != NULL) {
        if (curr->next != NULL) {
            destroyProbeList(curr->next);
        } else {
            free(curr->probedZone->resources);
            free(curr->probedZone);
            free(curr);
        }
    }
}

int main() {
    int numberOfZones = 0;

    while (loadZone()) {
        numberOfZones++;
        checkState();
        printf("%d Zone(s) loaded.\n", numberOfZones);
        printf("====================================\n");
    }

    printProbeSummary();

    printf("End of processing\n");

    return EXIT_SUCCESS;
}