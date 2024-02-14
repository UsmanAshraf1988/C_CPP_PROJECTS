/*
* Dependencies:
*   MySql C++ connector.
*/


#include <stdlib.h>
#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;/*for demonstration only*/

/*
show databases;
use classicmodels;
show tables;
select customerNumber, customerName, phone, country from customers where country='Germany' and customerNumber=278;
select * from customers where country='Germany' and customerNumber > 1 order by customerNumber desc limit 2;
insert into productlines(productline,textDescription) values("manual1","usman1");
insert into productlines values ("manual","usman",NULL,NULL);
delete from productlines where productline='manual';
update productlines set productline='manual2', textDescription='usman2' where productline='manual1';
*/

//for demonstration only. never save your password in the code!
const string server = "tcp://127.0.0.1:3306"; /*Please use your server address here, better to be tcp conn*/
const string username = "username"; /*Please use your db username here*/
const string password = "**********";/*Please use your db password here*/

int main()
{
	sql::Driver* driver = nullptr;
	sql::Connection* con = nullptr;
	sql::Statement* stmt = nullptr;
	sql::PreparedStatement* pstmt = nullptr;
	sql::ResultSet* result = nullptr;

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		cout << "Could not connect to server. Error message: " << e.what() << endl;
		system("pause");
		exit(1);
	}

	try
	{
		//please create database "classicmodels" ahead of time
		con->setSchema("classicmodels");

		stmt = con->createStatement();
		stmt->execute("DROP TABLE IF EXISTS inventory");
		cout << "Finished dropping table (if existed)" << endl;
		stmt->execute("CREATE TABLE inventory (id serial PRIMARY KEY, name VARCHAR(50), quantity INTEGER);");
		cout << "Finished creating table" << endl;
		delete stmt;
		stmt = nullptr;

		pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?,?)");
		pstmt->setString(1, "banana");
		pstmt->setInt(2, 150);
		pstmt->execute();
		cout << "One row inserted." << endl;

		pstmt->setString(1, "orange");
		pstmt->setInt(2, 154);
		pstmt->execute();
		cout << "One row inserted." << endl;

		pstmt->setString(1, "apple");
		pstmt->setInt(2, 100);
		pstmt->execute();
		cout << "One row inserted." << endl;

		if (pstmt)
		{
			delete pstmt;
			pstmt = nullptr;
		}

		//select  
		pstmt = con->prepareStatement("SELECT * FROM inventory;");
		result = pstmt->executeQuery();

		while (result->next())
			printf("Reading from table=(%d, %s, %d)\n", result->getInt(1), result->getString(2).c_str(), result->getInt(3));

	}
	catch (sql::SQLException e)
	{
		cout << "Could not connect to server. Error message: " << e.what() << endl;
		if (pstmt)
		{
			delete pstmt;
			pstmt = nullptr;
		}
		if (con)
		{
			delete con;
			con = nullptr;
		}			
		if (result)
		{
			delete result;
			result = nullptr;
		}
			
		system("pause");
		exit(1);
	}


	if (pstmt)
	{
		delete pstmt;
		pstmt = nullptr;
	}
	if (con)
	{
		delete con;
		con = nullptr;
	}
	if (result)
	{
		delete result;
		result = nullptr;
	}

	system("pause");
	return 0;
}