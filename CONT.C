#include<stdio.h>
#include<conio.h>
#include<malloc.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>


void addNewContact();
void listAll();
void deleteContact();
void modifyContact();
int findContact();
int prompt();
int findNum(int);


typedef struct contact
{
	int number;
	char name[20];
	char phone[15];
	char email[20];
	struct contact *next;
}Contact;
Contact *firstc, *currentc, *newc;

int cnum = 0;

int main()
{
	FILE *datafile;
	char *filename = "ContactDatabase.dat";
	int ch;
	clrscr();
	firstc = NULL;
	datafile = fopen(filename, "r");
	if(datafile)
	{
		firstc = (struct contact *)malloc(sizeof(struct contact));
		currentc = firstc;
		while(1)
		{
			newc = (struct contact *)malloc(sizeof(struct contact));
			fread(currentc, sizeof(struct contact), 1, datafile);
			if(currentc->next == NULL)
				break;
			currentc->next = newc;
			currentc = newc;
		}
		fclose(datafile);
		cnum = currentc->number;
	}
	while(1)
	{
		fflush(stdin);
		printf("\nWelcome to the Contact Database\n");
		printf("-----------------------------------");
		printf("\n1 - Add a new contact");
		printf("\n2 - Delete contact");
		printf("\n3 - List all contacts");
		printf("\n4 - Modify contact");
		printf("\n5 - Find a contact by name");
		printf("\n6 - Quit\n");
		printf("\nYour choice: ");
		scanf("%d", &ch);
		switch(ch)
		{
			case 1: fflush(stdin);
				addNewContact();
				break;
			case 2: deleteContact();
				break;
			case 3: listAll();
				break;
			case 4: modifyContact();
				break;
			case 5: findContact();
				break;
			case 6: exit(0);
			default: printf("Invalid choice!! Try again.\n");
		}
	}
	currentc = firstc;
	if(currentc == NULL)
		return (0);
	datafile = fopen(filename, "w");
	if(datafile == NULL)
	{
		printf("Error writing to %s\n", filename);
		return (1);
	}
	while(currentc != NULL)
	{
		fwrite(currentc, sizeof(struct contact), 1, datafile);
		currentc = currentc->next;
	}
	fclose(datafile);
	return (0);
	getch();
}


void addNewContact()
{
	newc = (struct contact *)malloc(sizeof(struct contact));
	if(firstc == NULL)
		firstc = currentc = newc;
	else
	{
		currentc = firstc;
		while(currentc->next != NULL)
			currentc = currentc->next;
		currentc->next = newc;
		currentc = newc;
	}
	cnum++;
	printf("%27s: %5i\n", "contact number", cnum);
	currentc->number = cnum;
	printf("%27s: ", "Enter contact name");
	gets(currentc->name);
	printf("%27s: ", "Enter phone number");
	gets(currentc->phone);
	printf("%27s: ", "Enter contact email");
	gets(currentc->email);
	printf("Contact added!\n");
	currentc->next = NULL;
}

void listAll()
{
	if(firstc == NULL)
		printf("There are no contacts to display.\n");
	else
	{
		printf("%6s %-20s %-15s %-20s\n", "Acct#", "Name", "Phone", "Email");
		printf("\n-------------------------------------------------------------\n");
		currentc = firstc;
		do
		{
			printf("%6d: %-20s %-15s %-20s\n",\
			currentc->number,\
			currentc->name,\
			currentc->phone,\
			currentc->email);
		}while((currentc = currentc->next) != NULL);
	}
}

void deleteContact()
{
	int record;
	struct contact *prev;
	if(firstc == NULL)
	{
		printf("There are no contacts to delete.\n");
		return;
	}
	listAll();
	printf("Enter contact account number to delete: ");
	scanf("%d", &record);
	currentc = firstc;
	while(currentc != NULL)
	{
		if(currentc->number == record)
		{
			if(currentc == firstc)
				firstc = currentc->next;
			else
				prev->next = currentc->next;
			free(currentc);
			printf("Contact %d deleted!\n", record);
			return;
		}
		else
		{
			prev = currentc;
			currentc = currentc->next;
		}
	}
	printf("Contact %d not found!\n", record);
}

void modifyContact()
{
	int record, result;
	if(firstc == NULL)
	{
		printf("There are no contacts to modify!\n");
		return;
	}
	listAll();
	printf("Enter contact account number to modify or change: ");
	scanf("%d", &record);
	result = findNum(record);
	if(result > 0)
	{
		printf("Contact %d: \n", currentc->number);
		printf("Name: %s\n", currentc->name);
		if(prompt())
			gets(currentc->name);
		printf("Phone: %s\n", currentc->phone);
		if(prompt())
			gets(currentc->phone);
		printf("Email: %s\n", currentc->email);
		if(prompt())
			gets(currentc->email);
		printf("Contact updated!!\n");
		return;
	}
	printf("Contact %d was not found!\n", record);
}


int findNum(int recordnum)
{
	int record;
	record = recordnum;
	currentc = firstc;
	while(currentc != NULL)
	{
		if(currentc->number == record)
			return 1;
		else
			currentc = currentc->next;
	}
	return -1;
}

int findContact()
{
	char buff[20];
	if(firstc == NULL)
	{
		printf("There are no contacts to find!\n");
		return 1;
	}
	printf("Enter contact name: ");
	fflush(stdin);
	gets(buff);
	currentc = firstc;
	while(currentc != NULL)
	{
		if(strcmp(currentc->name, buff) == 0)
		{
			printf("%6s %-20s %-15s %-15s\n", "Acct#", "Name", "Phone", "Email");
			printf("%6d: %-20s %-15s %-20s\n",\
			currentc->number,\
			currentc->name,\
			currentc->phone,\
			currentc->email);
			return 0;
		}
		else
			currentc = currentc->next;
	}
	printf("contact %s was not found!\n", buff);
	return 1;
}

int prompt()
{
	char ch;
	fflush(stdin);
	printf("Update? (Y to update any other key to not)");
	ch = getchar();
	ch = toupper(ch);
	fflush(stdin);
	if(ch == 'Y')
	{
		printf("Enter new value: ");
		return (1);
	}
	else
		return (0);
}


