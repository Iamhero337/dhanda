#include <dhanda/dhanda.h>
#include <dhanda/txn.h>


void
txn_insert_in_list(dhanda *app, txn *t)
{
	Node *node;
	debug_print("");

	node = list_new_node(app->txn_list, t);
	if (node)
		list_insert_end(app->txn_list, node);
}

txn *
txn_first_in_list(dhanda *app)
{
	txn *t = NULL;
	Node *head;
	debug_print("");

	head = app->txn_list->head;
	if (head)
		t = (txn *) head->data;

	return t;
}

txn *
txn_second_in_list(dhanda *app)
{
	txn *t = NULL;
	Node *head;
	debug_print("");

	head = app->txn_list->head;
	if (head && head->next)
		t = (txn *) head->next->data;

	return t;
}

//txn_read

int txn_findbyid(dhanda *app, int id, txn *result)
{
	int matched = 0;

	debug_print("");

	fseek(app->txn_fp, 0, SEEK_SET);
	while(fread(result, sizeof(txn), 1, app->txn_fp) > 0) {
		if(id == result->id) {
			matched = 1;
			break;
		}
	}
	if(matched == 0) {
		if(ferror(app->txn_fp))
			matched = -1;
		
		else
			matched = 0;
	}
	return matched;


}


	
int txn_search(dhanda *app, char *query, struct list *result)
{
	Node *n;
	txn temp;
	int matched = 0;
	debug_print("");
	int party_id = atoi(query);
	
	fseek(app->txn_fp, 0, SEEK_SET);
	while(fread(&temp, sizeof(temp), 1, app->txn_fp) > 0) {
		if (party_id == temp.party_id) {
			n = list_new_node(result, &temp);
			list_insert_end(result, n);
			++matched;
		}
		/* @TODO */
		/*
		if (strcmp(temp.cat, query) == 0) {
			matched = 0;
			break;
		}
		*/
	}

	return matched;
}

int txn_findbytype(dhanda *app, int type, struct list *result)
{	
	txn temp;
	int count;
	Node *n;

	count = 0;
	debug_print("");
	while(fread(&temp, sizeof(txn), 1, app->txn_fp)>0) {

		if (type == temp.type)
		{
			n = list_new_node(result, &temp);
			list_insert_end(result, n);
			count++;
		}
	}

	return count;
}

int txn_get(dhanda *app, txn_filter filter, struct list *result)
{
	txn temp;
	Node *node;
	int count, offset;
	debug_print("");

	count = 0;
	// offset = (filter.page - 1) * filter.items * sizeof(txn);
	// fseek(app->txn_fp, -offset, SEEK_END);

	fseek(app->txn_fp, 0, SEEK_SET);
	while (fread(&temp, sizeof(temp), 1, app->txn_fp) > 0) {
		if (count >= filter.items)
			break;

		node = list_new_node(result, (void *) &temp);
		if (node == NULL)
			break;
		list_insert_end(result, node);
	}

	return count;
}

//txn_create

int txn_add(dhanda *app, txn *txn)
{
	struct txn temp;
	int init_posn, fin_posn, txn_id;
	debug_print("");

	fseek(app->txn_fp, 0, SEEK_END);
	init_posn = ftell(app->txn_fp);
	if (init_posn) {
		fseek(app->txn_fp, -sizeof(*txn), SEEK_END);
		fread(&temp, sizeof(*txn), 1, app->txn_fp);
		txn_id = temp.id;
		txn_id++;
	} else {
		txn_id = 1;
	}

	txn->id = txn_id;

	init_posn = ftell(app->txn_fp);
	fwrite(txn, sizeof(*txn), 1, app->txn_fp);
	fin_posn = ftell(app->txn_fp);




	if (init_posn < fin_posn)
		return 0;
	else
		return -1;
}



