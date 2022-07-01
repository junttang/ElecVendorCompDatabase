/**************************************************
 * Title: DB-Project 2 - Electronics Vendor Company
 * Summary: This is MySQL C APL for the project 2.
   You can see details on the PDF file attached!!!
 * WARNING: While executing the program, you must 
   quit the program only with '0' input option, no-
   -t press 'CTRL+C' to quit the program, because 
   if you press 'CTRL+C', the newly created tempor-
   -ary database will not be deleted on your compu-
   -ter(MySQL). This is very important !!!
 *  |Date              |Author             |Version
	|2022-06-05        |Park Junhyeok      |1.0.0
**************************************************/

/******************** Headers ********************/
#include <stdio.h>
#include <string.h>
#include "mysql.h"
#pragma comment(lib, "libmysql.lib")
#define MAX_LEN			13000
#define QRY_LEN			300
#define clear_buf()		while (getchar() != '\n') { input = 0; };// for brevity
#define clear_buf2()	while (getchar() != '\n');				 // for brevity


/****************** Declaration ******************/
// Global variables for MySQL connection
const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";

// Subroutines for GUI support
void print_prompt(void);
void press_any_key(void);
void try_again_routine(void);

// Subroutines for MySQL setting or an error
void set_safety_mode(MYSQL*);
void unset_safety_mode(MYSQL*);
void error_routine(MYSQL*);

// Subroutines for each query processing
void type1_routine(MYSQL*);
void type1_sub1_routine(MYSQL*, char*, char*);
void type1_sub1_update(MYSQL*, MYSQL_RES*, char*);
bool type1_sub1_error_check(MYSQL*, MYSQL_RES*);
void type2_routine(MYSQL*);
void type2_sub1_routine(MYSQL*, char*, char*);
void type3_routine(MYSQL*);
void type3_sub1_routine(MYSQL*);
void type3_sub2_routine(MYSQL*, int);
void type4_routine(MYSQL*);
void type4_sub1_routine(MYSQL*);
void type4_sub2_routine(MYSQL*, int);
void type5_routine(MYSQL*);
void type6_routine(MYSQL*);
void type7_routine(MYSQL*);


/**************** Implementation *****************/
/*******   Main Function for the program   *******/
int main(void) {
	MYSQL* connection = NULL, conn;
	bool exit_flag = false;
	char line[MAX_LEN], input;
	FILE* fp = fopen("myddl.txt", "rt");			// open CRUD file.

	if (mysql_init(&conn) == NULL)					// initialize the connection
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, NULL, 3306, (const char*)NULL, 0);
	if (connection == NULL) 
		error_routine(&conn);
	else {
		while (fgets(line, sizeof(line), fp) != NULL) {
			if (!strcmp(line, "$$$\n"))			// read lines from CRUD file
				break;							// '$$$' separates CREATE / DELETE parts
			mysql_query(connection, line);			// these are CREATEs & INSERTs
		}										

		while (1) {
			print_prompt();							// print prompt
			scanf("%c", &input);					// read the type
			clear_buf();
		
			switch (input) {								// handling each query
			case '1': type1_routine(connection); break;
			case '2': type2_routine(connection); break;
			case '3': type3_routine(connection); break;
			case '4': type4_routine(connection); break;
			case '5': type5_routine(connection); break;
			case '6': type6_routine(connection); break;
			case '7': type7_routine(connection); break;
			case '0': exit_flag = true; break;
			default: try_again_routine(); break;
			}
			printf("----------------------------------\n");

			if (exit_flag) break;							// if input is '0', exit
		}

		while (fgets(line, sizeof(line), fp) != NULL)
			mysql_query(connection, line);			// these are DELETEs & DROPs 

		mysql_close(connection);
	}

	return 0;
}

/********   Subroutines for GUI support   ********/
void print_prompt(void) {
	printf("\n\n\n\n\n------- SELECT QUERY TYPES -------\n\n");
	printf("\t1. TYPE 1\n");
	printf("\t2. TYPE 2\n");
	printf("\t3. TYPE 3\n");
	printf("\t4. TYPE 4\n");
	printf("\t5. TYPE 5\n");
	printf("\t6. TYPE 6\n");
	printf("\t7. TYPE 7\n");
	printf("\t0. QUIT\n");
	printf("\n      > ");
}

void press_any_key(void) {
	printf("Press <enter> to continue...");
	while (getchar() != '\n');
	printf("\n\n");
}

void try_again_routine(void) {
	printf("\n\nYou pressed an incorrect input! Please try again...\n\n");
}


/*   Subroutines for MySQL setting or an error   */
// This procedure set the SAFE_MODE of MySQL (for blocking updates)
void set_safety_mode(MYSQL* conn) {
	char set_safe_query[QRY_LEN] = "set SQL_SAFE_UPDATES = 1;";
	mysql_query(conn, set_safe_query);
}

// This procedure unset the SAFE_MODE of MySQL (for allowing updates)
void unset_safety_mode(MYSQL* conn) {
	char unset_safe_query[QRY_LEN] = "set SQL_SAFE_UPDATES = 0;";
	mysql_query(conn, unset_safe_query);
}

// Error handling function
void error_routine(MYSQL* conn) {
	printf("%d ERROR : %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
}


/***   Subroutines for each query processing   ***/
void type1_routine(MYSQL* conn) {
	char query1[QRY_LEN] = "select * from (select sales_id, ID, name from (select S.ID, S.sales_id from sales S where S.sales_id in (select OS.sales_id from onlinesales OS where OS.shipper = 'USPS' and OS.track_number = 'X' and complete_date = '2100-12-31')) P natural join customer) T natural join customer_phone";
	MYSQL_RES* res_q1;
	MYSQL_ROW row_q1;
	char input, cus_id[10] = "", sale_id[10] = "";
	bool first_row_flag = true;			// print 'customer ~' only for the first row

	printf("\n\n\n\n\n------------- TYPE 1 -------------\n\n");
	printf("** Assume the package shipped by USPS with tracking\n   number X is reported to have been destroyed in an\n   accident. Find the contact information for the customer. **\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query1) == 0) {
		res_q1 = mysql_store_result(conn);

		while ((row_q1 = mysql_fetch_row(res_q1)) != NULL) {
			if (first_row_flag) {
				printf("\t| Customer '%s(id: %s)'s contact info:\n", row_q1[2], row_q1[0]);
				strcpy(cus_id, row_q1[0]); strcpy(sale_id, row_q1[1]);
				first_row_flag = false;
			}
			printf("\t|\t - %s\n", row_q1[3]);			// print phone_number value
		}
		if (first_row_flag)					// if the result of query is empty, 
			printf("\t| *ALERT: There's no such shipment! Check carefully again!\n");

		mysql_free_result(res_q1);
	}
	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
	if (first_row_flag) return;				// if the result of query is empty, exit

	while (1) {													// subtype selection
		printf("\n------- Subtypes in TYPE 1 -------\n\n");
		printf("\t1. TYPE 1-1\n\t0. QUIT\n\n      > ");
		scanf("%c", &input);
		clear_buf();

		if (input == '1') { type1_sub1_routine(conn, cus_id, sale_id); break; }
		else if (input == '0') { printf("\n\n"); break; }
		else printf("\n\nYou pressed an incorrect input! Please try again...\n\n");
	}
}

void type1_sub1_routine(MYSQL* conn, char* cus_id, char* sale_id) {
	char query[QRY_LEN] = "select T.product_id, P.name, P.manu_name from (select product_id from package where item_id in (select S.item_id from sales S where S.sales_id in (select OS.sales_id from onlinesales OS where OS.shipper = 'USPS' and OS.track_number = 'X' and complete_date = '2100-12-31'))) T natural join product P";
	MYSQL_RES* res;

	printf("\n\n\n\n\n------------ TYPE 1-1 ------------\n\n");
	printf("** Then find the contents of that shipment\n   and create a new shipment of replacement items. **\n");
	printf("\t| The contents of the target shipment(id: %s)\n", sale_id);
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		if (type1_sub1_error_check(conn, res)) {	// i will explain here (next func)
			printf("\t|\n\t| *ALERT: Some products are currently out of stock in\n");
			printf("\t|         all warehouses regardless of regions, and must be\n");
			printf("\t|         reordered first. Thus, replacement is rejected!\n");
		}
		else {
			unset_safety_mode(conn);				// ready for updating DB

			mysql_query(conn, query);
			res = mysql_store_result(conn);				// update
			type1_sub1_update(conn, res, sale_id);		// - revise the amount of stock
														// - adjust the shipment info
			set_safety_mode(conn);
		}

		mysql_free_result(res);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

// This procedure returns whether there's enough amount of stocks to re-package 
bool type1_sub1_error_check(MYSQL* conn, MYSQL_RES* res) {
	MYSQL_ROW row;
	bool error_flag;
	int amount = 0;
													// for each product in the 
	while ((row = mysql_fetch_row(res)) != NULL) {	// package currently considering
		char query[QRY_LEN] = "select * from stock_ware natural join warehouse where product_id = ";
		MYSQL_RES* res_temp;
		MYSQL_ROW row_temp;
		strcat(query, row[0]);

		if (mysql_query(conn, query) == 0) {
			error_flag = true;
			res_temp = mysql_store_result(conn);

			while ((row_temp = mysql_fetch_row(res_temp)) != NULL) {
				sscanf(row_temp[2], "%d", &amount);

				if (amount > 0) {				// if there's at least one warehouse
					error_flag = false;			// which can provide replacement item
					break;						// (that is currently considered),
				}								// then 'OK' for this 'product'
			}
			mysql_free_result(res_temp);

			if (error_flag) return true;		// if not, we should re-order first!
		}
	}
	return false;
	// +PS) we need an assumption: "Regardless of the region the warehouse is located,
	// if there's at least one warehouse that can provide replacement, this is 'OK'
	// for the product currently considering. And if all products in the package get 
	// 'OK', this means it's finally 'OK' for replacing the broken shipment.
}

// This procedure updates the stock(in the warehouses) and online salesment info
void type1_sub1_update(MYSQL* conn, MYSQL_RES* res, char* sale_id) {
	MYSQL_ROW row;
	int amount = 0;

	while ((row = mysql_fetch_row(res)) != NULL) {	// for each product in package
		printf("\t| - %s's %s(id: %s)\n", row[2], row[1], row[0]);

		char query[QRY_LEN] = "select * from stock_ware natural join warehouse where product_id = ";
		MYSQL_RES* res_temp;
		MYSQL_ROW row_temp;
		strcat(query, row[0]);

		if (mysql_query(conn, query) == 0) {
			res_temp = mysql_store_result(conn);

			while ((row_temp = mysql_fetch_row(res_temp)) != NULL) {
				sscanf(row_temp[2], "%d", &amount);

				if (amount > 0) {	// the first 'OK' warehouse will be selected 
					printf("\t|\t-> Replacement is released from warehouse '%s'(residual stock: %d)\n",
						row_temp[0], --amount);

					char update_stock_query[QRY_LEN] = "";
					sprintf(update_stock_query, "update stock_ware set amount = %d where product_id = %s and warehouse_id = %s;",
						amount, row[0], row_temp[0]);
					mysql_query(conn, update_stock_query);	// update stock info

					break;
				}
			}
			mysql_free_result(res_temp);
		}
	}

	char update_sales_query[QRY_LEN] = "";
	sprintf(update_sales_query, "update onlinesales set track_number = 'newX', promised_date = date_format(curdate(), '%%Y-%%m-%%d') where sales_id = %s;",
		sale_id);
	mysql_query(conn, update_sales_query);					// update shipment info
	printf("\t|\n\t|  => New shipment created! (with new track number 'newX')\n");
}

void type2_routine(MYSQL* conn) {
	char query[QRY_LEN] = "select C.ID, C.name from (select S.ID, sum(market_price) sum_pay from (select * from sales where sales_date_year = year(current_date - interval 1 year)) S natural join MarketItem group by S.ID order by sum_pay desc) K natural join customer C limit 1";
	char name[20], id[20], input;
	MYSQL_RES* res;
	MYSQL_ROW row;

	printf("\n\n\n\n\n------------- TYPE 2 -------------\n\n");
	printf("** Find the customer who has bought the most (by price) in the past year. **\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);

		printf("\t| Customer '%s(id: %s)' has bought the most in the past year!\n", row[1], row[0]);
		strcpy(name, row[1]); strcpy(id, row[0]);			// for subtype processing

		mysql_free_result(res);
	}
	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();

	while (1) {
		printf("\n------- Subtypes in TYPE 2 -------\n\n");
		printf("\t1. TYPE 2-1\n\t0. QUIT\n\n      > ");
		scanf("%c", &input);
		clear_buf();

		if (input == '1') { type2_sub1_routine(conn, name, id); break; }
		else if (input == '0') { printf("\n\n"); break; }
		else printf("\n\nYou pressed an incorrect input! Please try again...\n\n");
	}
}

// This procedure prints the products bought most by the customer who bought most  
void type2_sub1_routine(MYSQL* conn, char* cus_name, char* cus_id) {
	char query[QRY_LEN] = "select product_id, P.name, P.manu_name from (select product_id, count(*) count from (select * from sales where sales_date_year = year(current_date - interval 1 year) and ID = '";
	MYSQL_RES* res;
	MYSQL_ROW row;
	strcat(query, cus_id);
	strcat(query, "') T natural join package group by product_id order by count desc) K natural join product P limit 1");

	printf("\n\n\n\n\n------------ TYPE 2-1 ------------\n\n");
	printf("** Then find the product that the customer bought the most. **\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);

		printf("\t| Customer '%s(id: %s)' has bought\n", cus_name, cus_id);
		printf("\t|\t%s's %s(id: %s) the most!\n", row[2], row[1], row[0]);

		mysql_free_result(res);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

void type3_routine(MYSQL* conn) {
	char query[QRY_LEN] = "select product_id, P.name, P.manu_name from (select * from sales where sales_date_year = year(current_date - interval 1 year)) S natural join package natural join product P group by product_id order by manu_name";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int total_cnt = 0;					// for calculating index of 10%
	char input;

	printf("\n\n\n\n\n------------- TYPE 3 -------------\n\n");
	printf("** Find all products sold in the past year. **\n");
	printf("\t| [ Product, ProductID, Manufacturer ]\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)) != NULL) {
			printf("\t|  (%s, %s, %s)\n", row[1], row[0], row[2]);
			total_cnt++;
		}

		mysql_free_result(res);
	}
	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();

	while (1) {
		printf("\n------- Subtypes in TYPE 3 -------\n\n");
		printf("\t1. TYPE 3-1\n\t2. TYPE 3-2\n\t0. QUIT\n\n      > ");
		scanf("%c", &input);
		clear_buf();

		if (input == '1') { type3_sub1_routine(conn); break; }
		else if (input == '2') { type3_sub2_routine(conn, total_cnt); break; }
		else if (input == '0') { printf("\n\n"); break; }
		else printf("\n\nYou pressed an incorrect input! Please try again...\n\n");
	}
}

// This procedure prints top k rankers just by using 'limit' directive in MySQL 
void type3_sub1_routine(MYSQL* conn) {
	char query[QRY_LEN] = "select product_id, P.name, sum(P.price) dollar_amount from (select * from sales where sales_date_year = year(current_date - interval 1 year)) S natural join package natural join product P group by product_id order by dollar_amount desc limit ";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int index = 1;			// for printing ranks
	char input[5];

	printf("\n\n\n\n\n------------ TYPE 3-1 ------------\n\n");
	printf("** Then find the top k products by dollar-amount sold. **\nWhick K? : ");
	scanf("%s", input);		// get k input (as string)
	clear_buf2();
	strcat(query, input);	// and concatenate k to query
	printf("\t| [ Product, ProductID, Dollar-Amount ]\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)) != NULL)
			printf("\t| %d. (%s, %s, %s$)\n", index++, row[1], row[0], row[2]);

		mysql_free_result(res);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

// This procedure prints top 10% rankers just by using 'limit' and calculation
void type3_sub2_routine(MYSQL* conn, int total) {
	char query[QRY_LEN] = "select product_id, P.name, sum(P.price) dollar_amount from (select * from sales where sales_date_year = year(current_date - interval 1 year)) S natural join package natural join product P group by product_id order by dollar_amount desc limit ";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int index = 1, k = (int)((float)total / 10.0);	// calculating 10% index
	char input[5];								// with flooring by casting

	printf("\n\n\n\n\n------------ TYPE 3-2 ------------\n\n");
	printf("** Then find the top 10%% products by dollar-amount sold. **\n");
	sprintf(input, "%d", k);
	strcat(query, input);
	printf("\t| [ Product, ProductID, Dollar-Amount ]\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)) != NULL)
			printf("\t| %d. (%s, %s, %s$)\n", index++, row[1], row[0], row[2]);

		mysql_free_result(res);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

// This procedure has a logical flow that is quite similar to the previous's
void type4_routine(MYSQL* conn) {
	char query[QRY_LEN] = "select product_id, P.name, P.manu_name, count(*) sales_count from (select * from sales where sales_date_year = year(current_date - interval 1 year)) S natural join package natural join product P group by product_id order by manu_name";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int total_cnt = 0;					// for calculating index of 10%
	char input;

	printf("\n\n\n\n\n------------- TYPE 4 -------------\n\n");
	printf("** Find all products by unit sales in the past year. **\n");
	printf("\t| [ Product, ProductID, Manufacturer, Sales-Count ]\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)) != NULL) {
			printf("\t|  (%s, %s, %s, %sEA)\n", row[1], row[0], row[2], row[3]);
			total_cnt++;
		}

		mysql_free_result(res);
	}
	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();

	while (1) {
		printf("\n------- Subtypes in TYPE 4 -------\n\n");
		printf("\t1. TYPE 4-1\n\t2. TYPE 4-2\n\t0. QUIT\n\n      > ");
		scanf("%c", &input);
		clear_buf();

		if (input == '1') { type4_sub1_routine(conn); break; }
		else if (input == '2') { type4_sub2_routine(conn, total_cnt); break; }
		else if (input == '0') { printf("\n\n"); break; }
		else printf("\n\nYou pressed an incorrect input! Please try again...\n\n");
	}
}

// This procedure prints top k rankers just by using 'limit' directive in MySQL 
void type4_sub1_routine(MYSQL* conn) {
	char query[QRY_LEN] = "select product_id, P.name, count(*) sales_count from (select * from sales where sales_date_year = year(current_date - interval 1 year)) S natural join package natural join product P group by product_id order by sales_count desc limit ";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int index = 1;
	char input[5];

	printf("\n\n\n\n\n------------ TYPE 4-1 ------------\n\n");
	printf("** Then find the top k products by unit sales. **\nWhick K? : ");
	scanf("%s", input);
	clear_buf2();
	strcat(query, input);
	printf("\t| [ Product, ProductID, Sales-Count ]\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)) != NULL)
			printf("\t| %d. (%s, %s, %sEA)\n", index++, row[1], row[0], row[2]);
		
		mysql_free_result(res);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

// This procedure prints top 10% rankers just by using 'limit' and calculation
void type4_sub2_routine(MYSQL* conn, int total) {
	char query[QRY_LEN] = "select product_id, P.name, count(*) sales_count from (select * from sales where sales_date_year = year(current_date - interval 1 year)) S natural join package natural join product P group by product_id order by sales_count desc limit ";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int index = 1, k = (int)((float)total / 10.0);
	char input[5];

	printf("\n\n\n\n\n------------ TYPE 4-2 ------------\n\n");
	printf("** Then find the top 10%% products by unit sales. **\n");
	sprintf(input, "%d", k);
	strcat(query, input);
	printf("\t| [ Product, ProductID, Sales-Count ]\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)) != NULL)
			printf("\t| %d. (%s, %s, %sEA)\n", index++, row[1], row[0], row[2]);
		
		mysql_free_result(res);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

void type5_routine(MYSQL* conn) {
	char query1[QRY_LEN] = "select store_id, store_name from store where region = 'California'";
	char query2[QRY_LEN] = "select product_id, P.name, P.manu_name from (select * from stock_store where amount = '0') K natural join product P where store_id = ";
	MYSQL_RES* res_q1, * res_q2;
	MYSQL_ROW row_q1, row_q2;

	printf("\n\n\n\n\n------------- TYPE 5 -------------\n\n");
	printf("** Find those products that are out-of-stock at every store in California. **\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query1) == 0) {
		res_q1 = mysql_store_result(conn);

		while ((row_q1 = mysql_fetch_row(res_q1)) != NULL) {  // for each store 
			printf("\t| %s\n", row_q1[1]);						// in California
			char temp[QRY_LEN]; strcpy(temp, query2); strcat(temp, row_q1[0]);

			if (mysql_query(conn, temp) == 0) {
				res_q2 = mysql_store_result(conn);

				while ((row_q2 = mysql_fetch_row(res_q2)) != NULL)
					printf("\t|\t - %s %s (id: %s)\t\t\n", row_q2[2], row_q2[1], row_q2[0]);
											// prints products that is out-of-stock
				mysql_free_result(res_q2);
			}
			printf("\t|\n");
		}

		mysql_free_result(res_q1);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

void type6_routine(MYSQL* conn) {
	char query1[QRY_LEN] = "select sales_id, item_id, promised_date, complete_date from sales natural join (select sales_id, promised_date, complete_date from onlinesales where (promised_date - complete_date) < 0 and complete_date <> '2100-12-31') S order by complete_date desc";
	char query2[QRY_LEN] = "select P.manu_name, product_id, P.name from package natural join product P where item_id = ";
	MYSQL_RES* res_q1, * res_q2;
	MYSQL_ROW row_q1, row_q2;

	printf("\n\n\n\n\n------------- TYPE 6 -------------\n\n");
	printf("** Find those packages that were not delivered within the promised time. **\n");
	printf("\t| Package ID\t| Expected\t| Real\t\t (Sale ID)\t\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query1) == 0) {
		res_q1 = mysql_store_result(conn);

		while ((row_q1 = mysql_fetch_row(res_q1)) != NULL) {	// for each package
			printf("\t| %s\t\t| %s\t| %s\t (%s)\t\n", row_q1[1], row_q1[2], row_q1[3], row_q1[0]);
			char temp[QRY_LEN]; strcpy(temp, query2); strcat(temp, row_q1[1]);

			if (mysql_query(conn, temp) == 0) {
				res_q2 = mysql_store_result(conn);

				while ((row_q2 = mysql_fetch_row(res_q2)) != NULL)
					printf("\t|\t\t - %s %s(id: %s)\t\t\n", row_q2[0], row_q2[2], row_q2[1]);
																// prints products included
				mysql_free_result(res_q2);
			}
			printf("\t|\n");
		}

		mysql_free_result(res_q1);
	}

	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();
}

void type7_routine(MYSQL* conn) {
	char query[QRY_LEN] = "select * from (select ID, item_id, left_total_bill, monthly_bill from (select ID, item_id from sales where sales_date_year = year(current_date) and sales_date_month = month(current_date - interval 1 month)) P natural join contractcustomer) as K natural join marketitem";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int left_bill, month_bill, charge;
	char input;

	unset_safety_mode(conn);

	printf("\n\n\n\n\n------------- TYPE 7 -------------\n\n");
	printf("** Generate the bill for each customer for the past month. **\n");
	printf("\t|---------------------------------------------------------\n");

	if (mysql_query(conn, query) == 0) {
		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)) != NULL) {
			sscanf(row[2], "%d", &left_bill);			// fetch left_total_bill
			sscanf(row[4], "%d", &charge);				// fetch 'this month's payment'

			left_bill += charge;						// add charge to the left bill
			month_bill = (int)((float)left_bill / 12.0);	// calculate monthly bill

			char make_bill[QRY_LEN] = "";
			sprintf(make_bill, "update contractcustomer set left_total_bill = %d, monthly_bill = %d where ID = %s;",
				left_bill, month_bill, row[1]);			// updates bill info
			mysql_query(conn, make_bill);

			printf("\t| - Charge %d$ to customer(id: %s, purchased item '%s')\n",
				charge, row[1], row[0]);
		}

		printf("\t|\n\t| * Bill has been generated successfully! *\n\t|\n");

		strcat(query, " group by ID");
		mysql_query(conn, query);
		res = mysql_store_result(conn);
		while ((row = mysql_fetch_row(res)) != NULL)	// prints the new generated bills
			printf("\t| ~> New 'monthly bill' for customer(id: %s) is %s$\n",
				row[1], row[3]);

		mysql_free_result(res);
	}
	printf("\t|\n\t| *ALERT: You must execute this query only once for a month.\n");
	printf("\t|---------------------------------------------------------\n\n");
	press_any_key();

	set_safety_mode(conn);
}
/************** End of the program ***************/