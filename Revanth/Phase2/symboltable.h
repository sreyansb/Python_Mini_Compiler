typedef struct record
{
	char *type;
	char *name;
	int decLineNo;
	int lastUseLine;
	
	int value; //		
} record;

typedef struct STable
{
	int no;
	int noOfElems;
	int scope;
	record *Elements;
	int Parent;	
} STable;


int power(int base, int exp)
{
	int res = 1;
	for(int i = 0; i < exp; i++)
	{
		res *= base;
	}
	return res;
}

void updateCScope(int scope)
{
	currentScope = scope;
}

int scopeBasedTableSearch(int scope)
{
	for(int i = sIndex; i > -1; i--)//sIndex indicates number of symbol tables i.e. length of symboltables array
	{
		if(symbolTables[i].scope == scope)
		{
			return i;
		}
	}
	return -1;
}

void initNewTable(int scope)
{
	arrayScope[scope]++;
	sIndex++;
	symbolTables[sIndex].no = sIndex;
	symbolTables[sIndex].scope = power(scope, arrayScope[scope]);
	symbolTables[sIndex].noOfElems = 0;		
	symbolTables[sIndex].Elements = (record*)calloc(200, sizeof(record));
	
	symbolTables[sIndex].Parent = scopeBasedTableSearch(currentScope); 
}

void init()
{
	argsList = (char *)malloc(1000);
	strcpy(argsList, "");
	symbolTables = (STable*)calloc(1000, sizeof(STable));

	arrayScope = (int*)calloc(100, sizeof(int));
	initNewTable(1);
}

// searching in the particular symbol table
int searchRecordInScope(const char* type, const char *name, int index)
{
	for(int i = 0; i < symbolTables[index].noOfElems; i++)
	{
		if((strcmp(symbolTables[index].Elements[i].type, type)==0) && (strcmp(symbolTables[index].Elements[i].name, name)==0))
		{
			return i;
		}
	}
	return -1;
}
	
void insertRecord(const char* type, const char *name, int lineNo, int scope)
{ 
	int FScope = power(scope, arrayScope[scope]);
	int index = scopeBasedTableSearch(FScope);
	int recordIndex = searchRecordInScope(type, name, index);

	if(recordIndex==-1) //record doesnt exist in the scope
	{
		symbolTables[index].Elements[symbolTables[index].noOfElems].type = (char*)calloc(30, sizeof(char));
		symbolTables[index].Elements[symbolTables[index].noOfElems].name = (char*)calloc(20, sizeof(char));
	
		strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].type, type);	
		strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].name, name);
		symbolTables[index].Elements[symbolTables[index].noOfElems].decLineNo = lineNo;
		symbolTables[index].Elements[symbolTables[index].noOfElems].lastUseLine = lineNo;
		symbolTables[index].noOfElems++;
	}
	else
	{
		symbolTables[index].Elements[recordIndex].lastUseLine = lineNo;
	}
}

void modifyRecordID(const char *type, const char *name, int lineNo, int scope)
{		
	int FScope = power(scope, arrayScope[scope]);
	int index = scopeBasedTableSearch(FScope);
	
	if(index==0)
	{
		for(int i = 0;i < symbolTables[index].noOfElems; i++)
		{	
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}	
		}
		printf("\nIdentifier '%s' at line %d Not Declared\n", name, yylineno);

		//Error Recovery
		insertRecord(type, name, lineNo, scope);

		yyerror("Invalid Python Syntax");
		//printSTable();
		check_error = 1;
		//exit(1);
	}
	
	for(int i = 0;i < symbolTables[index].noOfElems; i++)
	{
		if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
		{
			symbolTables[index].Elements[i].lastUseLine = lineNo;
			return;
		}	
	}
	return modifyRecordID(type, name, lineNo, symbolTables[symbolTables[index].Parent].scope);
}
