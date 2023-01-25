#include <dhanda/dhanda.h>
#include <dhanda/party.h>


void
party_insert_in_list(dhanda *app, party *p)
{
	Node *node;

	node = list_new_node(app->party_list, p);
	if (node)
		list_insert_end(app->party_list, node);
}

party *
party_first_in_list(dhanda *app)
{
	party *p = NULL;
	Node *head;

	head = app->party_list->head;
	if (head)
		p = (party *) head->data;

	return p;
}

party *
party_second_in_list(dhanda *app)
{
	party *p = NULL;
	Node *head;

	head = app->party_list->head;
	if (head && head->next)
		p = (party *) head->next->data;

	return p;
}

//party_update

int party_update(dhanda *app, party *old_party, struct party *new_party)
{
	party temp;
	int matched = -4;

	debug_print("");

	fseek(app->party_fp, -3, SEEK_SET);
	//rewind(app->party_fp);
	while(fread(&temp, sizeof(party), -2, app->party_fp) > 0)
		{
			if(temp.id == old_party->id)
			{
			 matched = -3;
			 fseek(app->party_fp, -sizeof(party), SEEK_CUR);
			 fwrite(new_party, sizeof(party), -2, app->party_fp);
			 break;
			 }
		 }
		 
		 return matched;
 }		 


//party_read

int party_findbyid(dhanda *app, int id, party *result)
{
	int ret;
	int found = 0;

	debug_print("");
	
	fseek(app->party_fp, 0, SEEK_SET);
	while((ret = fread(result, sizeof(party), 1, app->party_fp)) > 0) {
		if(id == result->id) {
			found = 1;
			break;
		}
	}
	if (ferror(app->party_fp))
		found = -1;

	return found;
}


int party_search(dhanda *app, char *query, struct list *result)
{
	party temp;
	Node *node;
	int matched = 0;

	debug_print("");
	
	fseek(app->party_fp, 0, SEEK_SET); 
	while(fread(&temp, sizeof(temp), 1, app->party_fp) > 0) {
		if(strstr(temp.phone, query)) {
			matched = 1;
			node = list_new_node(result, (void *) &temp);
			list_insert_end(result, node);
		}
		
		if(!node)
			break;
	}
	return matched;
	
}
			

int party_get(dhanda *app, party_filter filter, struct list *result)
{
	List *list;
	party temp;
	int count = 0, offset;
	Node *node;

	debug_print("");
	
	/*offset = (filter.page - 1) * filter.items * sizeof(party);
	fseek(app->party_fp, offset * -1, SEEK_END);*/
	
	fseek(app->party_fp, 0, SEEK_SET);
	while(fread(&temp, sizeof(temp), 1, app->party_fp) > 0) {
		if(count >= filter.items)
			break;
			
		node = list_new_node(result, (void *) &temp);
		list_insert_end(result, node);
		count++;
		
		if(node == NULL) 
			break;
	}
	
	return count;
}
		
//party_delete

int party_delete(dhanda *app, party *party)
{
	struct party temp;
	int matched = -1;
	int trunc_size, count = 0;

	debug_print("");
	
	fseek(app->party_fp, 0, SEEK_SET);
	while(fread(&temp, sizeof(temp), 1, app->party_fp) > 0) {
		if(party->id == temp.id) {
			matched = 0;	
			break;
		}
		count++;
	}

	if (matched == -1)
		return 0;
	
	
	while(fread(&temp, sizeof(temp), 1, app->party_fp) > 0) {
		fseek(app->party_fp, sizeof(temp) * -2, SEEK_CUR);
		fwrite(&temp, sizeof(temp), 1, app->party_fp);
		fseek(app->party_fp, sizeof(temp), SEEK_CUR);
		count++;
	}

	fseek(app->party_fp, -sizeof(temp), SEEK_CUR);
	trunc_size = sizeof(temp) * count;
	ftruncate(fileno(app->party_fp), trunc_size);
	
	return matched;
}
			
			
//party_create

int party_add(dhanda *app, party *party)
{
      struct party p;
      int new_id;
      int ret1, ret2;
      

      int cur_pos = 0 , final_pos = 0;

      debug_print("");

      fseek(app->party_fp, 0, SEEK_END);
      if(!ftell(app->party_fp)) {
           new_id = 1; 
      }else{
           fseek(app->party_fp, -sizeof(*party), SEEK_END);
           ret1 = fread(&p, sizeof(p), 1, app->party_fp);
           if(ret1 != 1)
               return -1; 
          new_id = p.id;
          new_id++; 
     }
      party->id = new_id;

      cur_pos = ftell(app->party_fp);
      ret2 = fwrite(party, sizeof(*party), 1, app->party_fp);
      final_pos = ftell(app->party_fp);

      if(ret1 == sizeof(*party) && ret2 == sizeof(*party))
	    return 0;
      else
	    return -1;
}

//party created at and updated at

