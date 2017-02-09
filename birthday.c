/********************************************************************
*	William Sawran													*
*	PID:  3951385													*
*	March 7, 2016													*
*	COP 3502 - COMPUTER SCIENCE I RECITATION						*		
*	Days/Times/Location:  Fr/11:20AM-12:10PM/HEC O106				*
*	Instructor:  Dr. Rochelle Elva									*
*	Recitation:  Birthday											*
*	Due date:  Tuesday, March 8, 2016								*
*																	*
*	This program reads in information on student names and their	*
*	respective birthdays for multiple classes of students, then		*
*	outputs for a given class the name of the student whose			*
*	birthday comes closest to the name of the student specified 	*
*	by the user.													*
*																	*
*********************************************************************/

/*** Note: Please fill in the incomplete functions. These will be indicated in both the
           function prototype and their empty definition.***/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_LENGTH 30
#define MAX_PEOPLE 1000
#define DAYS_IN_YEAR 365

// month,day and dayNumber are redundant, but both need to be stored because
// when the month and day are read in, the dayNumber is unknown. Only after
// we know whether or not a leap year baby is in the class can we assign
// the day number, which either ranges from 1 to 365 or 1 to 366.
struct student {
    char first[MAX_LENGTH];
    char last[MAX_LENGTH];
    int month;
    int day;
    int dayNumber;
};

// Store each month as they are stored in the input file.
char months[][MAX_LENGTH] = {"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE",
                  "JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"};

// Store the number of days completed BEFORE the corresponding month.
// For example, noleap[2] stores 59, because there are 59 days total in
// January and February, that complete before March begins so long as it's
// not a leap year.
int noleap[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
int leap[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

// Functions to read and process the data.
int readData(struct student* thisClass[], int numStudents);
void processQueries(struct student* thisClass[], int numStudents, int isleap);
int lookUpMonth(char *theMonth);
void setDayNumber(struct student* thisClass[],int length, int freq[]);

// Functions aiding the sort.
/*** You will be required to fill in these five functions. ***/
void quickSort(struct student* thisClass[], int length);
void quickSortRec(struct student* thisClass[], int low, int high);
int studentcmp(struct student *a, struct student *b);
int partition(struct student* thisClass[], int low, int high);
void swap(struct student* thisClass[], int index1, int index2);

// Functions used for debugging.
int issorted(struct student* thisClass[], int length);
void print(struct student* thisClass[], int length);

// Functions for solving the queries.
int find(struct student* thisClass[], int length, char* firstName, char* lastName);
void solve(struct student* thisClass[],int length, int location, int isleap);

int main() {

    int i, j, numClasses;

    // Read in number of classes
    scanf("%d", &numClasses);

    // Go through each class.
    for (i=1; i<= numClasses; i++) {

        int numStudents;
        scanf("%d", &numStudents);

        // We have an array of pointers so that when we sort, all we have to
        // do is swap references and not all the actual information.
        struct student* thisClass[MAX_PEOPLE];

        // Will be set to 1 if someone in the class is born on 2/29.
        int leapYear = readData(thisClass, numStudents);

        // Set up the dayNumber variable.
        if (leapYear)
            setDayNumber(thisClass, numStudents, leap);
        else
            setDayNumber(thisClass, numStudents, noleap);

        // Sort the students and process queries.
        quickSort(thisClass, numStudents);
        printf("Class #%d:\n%c\n", i, (issorted(thisClass, numStudents)) ? 'Y' : 'N');
        processQueries(thisClass, numStudents, leapYear);
    }

    return 0;
}

// Reads in teh data from the file pointed to by ifp (which must be in the
// designated format, into the array thisClass. numStudents must equal the
// number of students for this class.
int readData(struct student* thisClass[], int numStudents) {

    int j, leapYear = 0;

    // Read in each student's data.
    for (j=0; j<numStudents; j++) {

        // Need to allocate space for this student.
        thisClass[j] = (struct student*)(malloc(sizeof(struct student)));
        scanf("%s%s", thisClass[j]->first, thisClass[j]->last);

        // Store the month.
        char tmp[MAX_LENGTH];
        scanf("%s", tmp);
        thisClass[j]->month = lookUpMonth(tmp);

        // Won't get used...
        int year;

        // Read in the day and year.
        scanf("%d%d", &thisClass[j]->day, &year);

        // See if this student is a leap year baby!
        if (thisClass[j]->month == 1 && thisClass[j]->day == 29)
            leapYear = 1;
    }

    // This information is needed by main.
    return leapYear;
}

// Given the sorted array of students thisClass, the number of students in that
// array, whether or not there's a leap year baby in that array, and the file
// pointer to the file with the queries, this function answers the queries
// as dictated by the assignment write up.
void processQueries(struct student* thisClass[], int numStudents, int isleap) {

        // Read in queries and solve them.
        int j, numQueries;
        scanf("%d", &numQueries);
        for (j=0; j<numQueries; j++) {
            char first[MAX_LENGTH];
            char last[MAX_LENGTH];

            // Get the name, and solve that case.
            scanf("%s%s", first, last);
            int location = find(thisClass, numStudents, first, last);
            solve(thisClass, numStudents, location, isleap);
        }
        printf("\n");
}

// Returns the numeric equivalent (0-11) of the month stored in theMonth.
// It is assumed that theMonth is stored in all capital letters and spelled
// correctly.
int lookUpMonth(char *theMonth) {

    int i;

    // Find the month in the lookup list.
    for (i=0; i<12; i++)
        if (strcmp(theMonth, months[i]) == 0)
            return i;

    // Invalid months are assigned to December =)
    return 11;
}

// Sets the day number for each student in the array thisClass. freq must
// be a cumulative frequency array of the number of days that precede the
// corresponding month.
void setDayNumber(struct student* thisClass[],int length, int freq[]) {

    int i;
    for (i=0; i<length; i++)
        thisClass[i]->dayNumber = freq[thisClass[i]->month] + thisClass[i]->day;
}

// Wrapper function - sorts thisClass[0...length-1] as determined by the assignment.
void quickSort(struct student* thisClass[],int length)
{
    quickSortRec(thisClass, 0, length - 1);
    return;
}

// Implements a quick sort of the items in thisClass in between indexes low
// and high, inclusive. Please pick a RANDOM partition element!
void quickSortRec(struct student* thisClass[],int low, int high)
{
	// Variable declarations
	int pivot;
    
    if (low < high)
    {
        pivot = partition(thisClass, low, high);
        quickSortRec(thisClass, low, pivot - 1);
        quickSortRec(thisClass, pivot + 1, high);
    }
}

// Partitions the portion of the array thisClass from index low to index high,
// returning the index storing the partition element.
int partition(struct student* thisClass[], int low, int high) 
{
    // Variable declarations
    int loop, current, smallest, temp, pivotIndex, pivot, i, j;
    int randomIndexArray[3];
    struct student* pivotPtr = NULL;
    
    // Seed random variable function.
    srand( time(0) );
    
    for(loop = 0; loop < 3; loop++)
    {
	    // Randomly generate an index in the portion of the array thisClass between
	    // index low to index high and store that index in randomIndexArray array.
	    randomIndexArray[loop] = low + rand() % (high - low + 1);
	}
	
	// Sort the randomIndexArray array in ascending order from least to greatest.
	for(current = 0; current < 3; current++)
	{
		smallest = current;
		for(loop = current + 1; loop < 3; loop++)
		{
			if(randomIndexArray[loop] > randomIndexArray[smallest])
			{
				smallest = loop;
			}
			temp = randomIndexArray[loop];
			randomIndexArray[loop] = randomIndexArray[smallest];
			randomIndexArray[smallest] = temp;
		}
	}
	
	// Return the value of the middle index in the randomIndexArray array.
	pivotIndex = randomIndexArray[1];
	pivotPtr = thisClass[pivotIndex];
	
    i = low - 1;
    pivot = pivotPtr->dayNumber;
    
    // Swap element in pivot with element in highest index
    swap(thisClass, pivotIndex, high);
    
    // Traverse the array from lowest index to highest index
    for (j = low; j < high; j++)
    {
    	// If the dayNumber is less than the dayNumber in the pivot,
    	// increment i and swap the elements
        if (thisClass[j]->dayNumber < pivotPtr->dayNumber)
        {
            i++;
            swap(thisClass, i, j);
        }
    }
    
    // Swap the elements between index i and the highest index in the array
    swap(thisClass, i+1, high);
    return i + 1;
}

// Returns a negative integer if the student pointed to by a comes before
// the student pointed to by b, 0 if they are equal, and a positive integer
// if the student pointed to by a comes after the student pointed to by b.
// This uses the comparison technique described in the assignment writeup (first
// by birthday, then last name, then first name.
int studentcmp(struct student *a, struct student *b)
{
	// If student a's birthday comes before student b's birthday, return a negative number
    if(a->dayNumber < b->dayNumber)
    {
    	return -1;
    }
    // If student a's birthday comes after student b's birthday, return a positive number
    else if(a->dayNumber > b->dayNumber)
    {
    	return 1;
    }
    // If student a's birthday and student b's birthday are the same, compare their last names
    else
    {
    	// If student a's last name comes before student b's last name, return a negative number
    	if(strcmp(a->last, b->last) < 0)
    	{
    		return -1;
    	}
    	// If student a's last name comes after student b's last name, return a positive number
    	else if(strcmp(a->last, b->last) > 0)
    	{
    		return 1;
   		}
   		else
   		// If student a's birthday and student b's birthday are the same and their
		// last names are the same, then compare their first names
   		{
   			// If student a's first name comes before student b's first name, return a negative number
   			if(strcmp(a->first, b->first) < 0)
	    	{
	    		return -1;
	    	}
	    	// If student a's first name comes after student b's first name, return a positive number
	    	else if(strcmp(a->first, b->first) > 0)
	    	{
	    		return 1;
	   		}
	   		// If student a's birthday and student b's birthday are the same, their
			// last names are the same and their first names are the same, then return zero.
	   		else
	   		{
	   			return 0;
	   		}
   		}
	}
}

// Swaps the values (pointers) in thisClass stored in index1 and index2.
void swap(struct student* thisClass[], int index1, int index2)
{
    // Variable declarations
    struct student* temp;
    temp = thisClass[index1];
	thisClass[index1] = thisClass[index2];
	thisClass[index2] = temp;
}

// Returns 1 iff a is sorted by dayNumber.
int issorted(struct student* thisClass[], int length) {
    int i;

    // Looks for an adjacent pair out of order.
    for (i=0; i<length-1; i++)
        if (thisClass[i]->dayNumber > thisClass[i+1]->dayNumber)
            return 0;

    // Never found one.
    return 1;
}

// Prints out relevant information for each student in thisClass.
void print(struct student* thisClass[], int length) {
    int i;
    for (i=0; i<length; i++)
        printf("%s %s %d\n", thisClass[i]->first, thisClass[i]->last, thisClass[i]->dayNumber);
    printf("\n");
}

// Returns the index of the student with the name first last in thisClass.
// Returns 0 if there is no such student.
int find(struct student* thisClass[], int length, char* firstName, char* lastName) {

    int i;

    // Go through each student.
    for (i=0; i<length; i++)

        // Both names must match exactly.
        if (strcmp(thisClass[i]->first, firstName) == 0 &&
            strcmp(thisClass[i]->last, lastName) == 0)

            return i;

    // Should never get here, but we return the first student if we do.
    return 0;
}

// Given a sorted list of students in thisClass, its length, the index location
// of a queried student and whether or not a leap year baby was in the class,
// this function determines the student with the closest birthday to this
// student according to the rules given in the assignment and prints out the
// appropriate output.
void solve(struct student* thisClass[],int length, int location, int isleap) {

    int diffPrev, diffNext;

    // Wrap around for the previous name.
    if (location == 0) {
        diffNext = thisClass[location+1]->dayNumber - thisClass[location]->dayNumber;
        diffPrev = DAYS_IN_YEAR + isleap + thisClass[location]->dayNumber - thisClass[length-1]->dayNumber;
    }

    // Wrap around for the next name.
    else if (location == length-1) {
        diffNext = DAYS_IN_YEAR + isleap + thisClass[0]->dayNumber - thisClass[location]->dayNumber;
        diffPrev = thisClass[location]->dayNumber - thisClass[location-1]->dayNumber;
    }

    // Regular case.
    else {
        diffNext = thisClass[location+1]->dayNumber - thisClass[location]->dayNumber;
        diffPrev = thisClass[location]->dayNumber - thisClass[location-1]->dayNumber;
    }

    // Adjust differences, because no day is more than 1/2 a year away!!!
    if (DAYS_IN_YEAR+isleap - diffNext < diffNext)
        diffNext = DAYS_IN_YEAR+isleap - diffNext;
    if (DAYS_IN_YEAR+isleap - diffPrev < diffPrev)
        diffPrev = DAYS_IN_YEAR+isleap - diffPrev;

    // Avoids array out of bounds errors.
    int locNext = (location+1)%length;
    int locPrev = (length-1+location)%length;

    // Invokes the tiebreaker that we go to the next person first.
    if (diffNext <= diffPrev)
        printf("%s %s has the closest birthday to %s %s.\n", thisClass[locNext]->first,
        thisClass[locNext]->last, thisClass[location]->first, thisClass[location]->last);
    else
        printf("%s %s has the closest birthday to %s %s.\n", thisClass[locPrev]->first,
        thisClass[locPrev]->last, thisClass[location]->first, thisClass[location]->last);

}
