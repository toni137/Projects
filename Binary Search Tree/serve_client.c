
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *downtime(){

	sleep(1);
	//TODO: 1st downtime: Do balanceTree here
	pthread_rwlock_wrlock(&lock);
	Node * balance_root = balanceTree(root);
	freeSubtree(root);
	root=NULL;
	root=balance_root;
	pthread_rwlock_unlock(&lock);

	sleep(1);
	//TODO: 2nd downtime: Do balanceTree here
	pthread_rwlock_wrlock(&lock);
	balance_root = balanceTree(root);
	freeSubtree(root);
	root=NULL;
	root=balance_root;
	pthread_rwlock_unlock(&lock);

	sleep(1);
	//TODO: 3rd downtime: Do balanceTree here
	pthread_rwlock_wrlock(&lock);
        balance_root = balanceTree(root);
	freeSubtree(root);
	root=NULL;
	root=balance_root;
	pthread_rwlock_unlock(&lock);
	return NULL;
}


void* ServeClient(char *client){

	// TODO: Open the file and read commands line by line
	FILE *f;
	char line[100];
	f=fopen(client,"r");
	if(f==NULL) exit(-1);
	while(fgets(line,100,f)!=NULL)
       	   {	pthread_rwlock_wrlock(&lock);
		if(strstr(line,"insertNode")!=NULL)
		   {
			char *token = strtok(line," ");
			if(token!=NULL) token = strtok(NULL," ");
			int i = atoi(token);			
			root = insertNode(root,i);
			printf("%s", "[");
			printf("%s", client);
			printf("%s", "]");
			printf("%s ", "insertNode");
			printf("%d\n", i);
			

		   }
		if(strstr(line,"deleteNode")!=NULL)
		   {
			
			char *token = strtok(line," ");
			if(token!=NULL) token = strtok(NULL," ");
			int i = atoi(token);
			root = deleteNode(root,i);			
			printf("%s", "[");
			printf("%s", client);
			printf("%s", "]");
			printf("%s ", "deleteNode");
			printf("%d\n", i);
			
		   }
		if(strstr(line,"countNodes")!=NULL)
		   {
			int sum = countNodes(root);
			printf("%s", "[");
			printf("%s", client);
			printf("%s", "]");
			printf("%s ", "countNodes");
			printf("%d\n", sum);
			
			
		   }
		if(strstr(line,"sumSubtree")!=NULL)
		   {

			int sum = sumSubtree(root);
			printf("%s", "[");
			printf("%s", client);
			printf("%s", "]");
			printf("%s ", "sumSubtree");
			printf("%d\n", sum);
		   }
		pthread_rwlock_unlock(&lock);
 		
	   }
	fclose(f);

	return NULL;
}
