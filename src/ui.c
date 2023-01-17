#include <dhanda/dhanda.h>
#include <dhanda/ui.h>
#include <dhanda/txn.h>
#include <dhanda/party.h>


void
ui_home(dhanda *app)
{
	int txn = 567,
		amount = 12399,
		party = 56,
		debit = 144,
		credit = 12;

	sky();
	bold();
	centreprint("DHANDA");
	printf("\n\n");
	reset();
	printf("Total # txn     Total amount   Total # party    Total debit     Total credit");
	printf("\n");
	printf("___________     ____________   _____________    ___________     ____________");
	printf("\n\n");
	printf("%-15d %-15d %-15d %-15d %-15d ", txn, amount, party, debit, credit);
	printf("\n\n");
	puts("");


}


//ui_txn_show

void ui_txn_show(struct dhanda *app)
{
	Node *ptr;
	ptr = app->txn_list->head;
	txn *t;
	debug_print("");

	t = (txn *) ptr->data;

	sky();
	bold();
	centreprint("DHANDA > TRANSACTION > SHOW");
	printf("\n\n");
	reset();
	printf("%-10s %-10s %-10s %-10s %-10s %-10s", "ID", 
												  "AMOUNT",
												  "CAT",
												  "TYPE",
												  "PARTY_ID",
												  "DESC");
	printf("\n\n");
	
	printf("%-10d %-10d %-10ld %-10d %-10d %s\n\n", t->id, 
										  t->amount, 
										  t->cat, 
										  t->type,
										  t->party_id,
										  t->desc);
		
	

	  /*printf("PARTY TRANSACTION......\n\n");
		printf("NAME       TXN        DEBIT      CREDIT     ");
		printf("\n\n");
		printf("\n\n");
		printf("%-10d %-10d %-10d %-10d", id2, txn2, debit2, credit2);
		printf("\n\n");
		printf("%-10d %-10d %-10d %-10d", id3, txn3, debit3, credit3);
			puts("");*/

}

//ui_txn_list

void ui_txn_list(struct dhanda *app)
{
	Node *temp;
	txn *t;
	debug_print("");

	sky();
	bold();
	centreprint("DHANDA > TRANSACTION > LIST");
	printf("\n\n");
	reset();
	printf("   ID         PARTY_ID            AMOUNT          DEBIT/CREDIT(D/C)\n");
	printf("\n\n");

	temp = app->txn_list->head;
	while(temp != NULL)
	{	
		t = (txn *)temp->data;
         	printf("   %-10d %-19d %-15d %-15d\n\n",
			       	t->id,
				t->party_id,
			       	t->amount,
			       	t->type);

		temp = temp->next;
	}




	/*
	 char ptype1[32] = "CREDITED", ptype2[32] = "DEBITED";
	char name1[64] = "Safwan", name2[64] = "Habibi", name3[64] = "Iqra", name4[64] = "Hero";
	int id1 = 4, id2 = 3, id3 = 2, id4 = 1;
	int amount1 = 31231, amount2 = 34334, amount3 = 33324, amount4 = 4545;

	


	printf("TRANSACTION ----- HISTORY\n\n");

	printf("ID         NAME            AMOUNT          PTYPE\n");
	printf("__         ____            ______          _____");
	printf("\n\n");
	printf("%-10d %-15s %-15d %-15s\n\n", id1, name1, amount1, ptype2);
	printf("%-10d %-15s %-15d %-15s\n\n", id2, name2, amount2, ptype1);
	printf("%-10d %-15s %-15d %-15s\n\n", id3, name3, amount3, ptype2);
	printf("%-10d %-15s %-15d %-15s\n\n", id4, name4, amount4, ptype1);
	*/


}


//ui_txn_create

void ui_txn_create(struct dhanda *app)
{
	txn t;
	char line[256], ch;
	int len;
	debug_print("");

	sky();
	bold();
	centreprint("DHANDA > TRANSACTION > CREATE");
	printf("\n\n");
	reset();
	printf("   PARTY ID : ");
	scanf("%d", &t.party_id);
	printf("\n");
	printf("   AMOUNT   : ");
	scanf("%d", &t.amount);
	printf("\n");
	printf("[O FOR CRED, 1 FOR DEBT]\n");
	printf("   TYPE     : ");
	scanf("%d", &t.type);
	scanf("%c", &ch);
	printf("\n");
	printf("   DESC     : ");
	get_line(t.desc, 256);
	printf("\n");
	/*if(len > 0)
		strcpy(t.desc, line);*/
	//scanf("%s", t.desc);

	t.cat = time(NULL);
	txn_insert_in_list(app, &t);
	puts("");
}


//ui_party_show

void ui_party_show(struct dhanda *app)
{
	int id;
	party *p;
	char fname[32];
	char lname[32];
	char phone[12];
	int amount;

	debug_print("");
	p = party_first_in_list(app);

	sky();
	bold();
	centreprint("DHANDA > PARTY > SHOW");
	printf("\n\n");
	reset();

	grey();
	printf("   ID NO      : %d               \n\n", p->id);

	printf("   FIRST NAME : %s               ", p->fname);
	printf("\n\n");
	printf("   LAST NAME  : %s               ", p->lname);
	printf("\n\n");

	printf("   PHONE NO   : %s               \n\n", p->phone);
	reset();
	
}


//ui_party_list

void ui_party_list(struct dhanda *app)
{
	Node *ptr;
	party *p;
	ptr = app->party_list->head;
	/*printf("\x1b[30;106m  \x1b[1m \x1b[3m DHANDA > PARTY > LIST                    \n\n");
	printf("\x1b[0m");*/
	sky();
	bold();
	centreprint("DHANDA > PARTY > LIST");
	printf("\n\n");
	reset();
	grey();

	printf("   %-13s %-20s %-20s %-15s %-10s\n\n\n", "ID", "FIRST NAME", "LAST NAME", "AMOUNT", "PHONE");
	reset();
	while(ptr){
		p = (party *) ptr->data;
		printf("   %-13d %-20s %-20s %-15d %-10s\n\n", p->id, p->fname, p->lname, p->amount, p->phone);
		ptr = ptr->next;
	}
}



//ui_party_edit


void ui_party_edit(struct dhanda *app)
{
	long int phone, amount;
	char fname[64], lname[64];
	party *old_party, *new_party;
	int len;
	char *ptr;
	long int ret;

	old_party = party_first_in_list(app);
	new_party = party_second_in_list(app);

	debug_print("");

	sky();
	bold();
	centreprint("DHANDA > PARTY > EDIT");
	printf("\n\n");
	reset();

	printf("FIRST NAME :  %s\n\n", old_party->fname);
	printf("LAST NAME  :  %s\n\n", old_party->lname);
	printf("PHONE      :  %s\n\n", old_party->phone);
	printf("AMOUNT     :  %d\n\n", old_party->amount);
	puts("");


	printf("FIRST NAME :  ");
	get_string(new_party->fname, sizeof(new_party->fname));

	printf("LAST NAME  :  ");
	get_string(new_party->lname, sizeof(new_party->lname));

	printf("PHONE      :  ");
	get_string(new_party->phone, sizeof(new_party->phone));
	
	printf("AMOUNT     :  ");
	scanf("%d", &new_party->amount);
}


//ui_party_delete

void ui_party_delete(struct dhanda *app)
{
	Node *ptr;
	party *p;

	debug_print("");


	sky();
	bold();
	centreprint("DHANDA > PARTY > DELETE");
	printf("\n\n");
	reset();

	
	ptr = app->party_list->head;
	while(!ptr){
		p = (party *) ptr->data;
	}

}


//ui_party_create


void ui_party_create(struct dhanda *app)
{
	/*long int phone = 9748538411,
		amount = 10000;
	char fname[64] = "Safwan", lname[64] = "Haider";
	*/
	party p;
	int len;
	long ret;
	char s[100];
	char *ptr;


	sky();
	bold();
	centreprint("DHANDA > PARTY > CREATE");
	printf("\n\n");
	reset();
	grey();
	printf("   FIRST NAME :  ");
	get_string(p.fname, sizeof(p.fname));
	printf("\n");
	printf("   LAST NAME  :  ");
	get_string(p.lname, sizeof(p.lname));
	printf("\n");
	printf("   PHONE      :  ");
	get_string(p.phone, sizeof(p.phone));
	
	printf("\n");
	printf("  AMOUNT     :  ");
    scanf("%d",&p.amount);
		
	reset();
	party_insert_in_list(app, &p);

	puts("");
}

