#include <iostream>
#include <fstream>
#include "str_switch.h"
//#include "my_column.h"
#include "create_table.cpp"
//#include "errors.cpp"

using namespace std;


int main(int argc, char* argv[])
{
    string usage="\nUsage: SeregaSQL file.ssql";
    if (argc<2) cout<<"Not enought arguments"<<usage<<endl;
    if (argc>2) cout<<"Too many arguments"<<usage<<endl;
    if (argc==2)
    {
        ifstream myFile;
        myFile.open(argv[1],ios_base::in);
        if (myFile.is_open())
        {
            cout<<"Working..."<<endl;
            while (!myFile.eof())
            {
                string temp,result;
                myFile>>temp;
                result+=temp+" ";
                SWITCH (temp)
                {
                    CASE ("create"):
                        myFile>>temp;
                        result+=temp;
                        if (temp =="table")
                        {
                            string table_name;
                            getline(myFile,table_name,'(');
                            getline(myFile,temp,')');
                            result+=table_name+" ";
                            result+=temp;
                            create_table(table_name,temp);
                        }
                        break;
                    CASE ("delete"):
                        myFile>>temp;
                        result+=temp+" ";
                        if (temp =="table")
                        {
                            myFile>>temp;
                            result+=temp;
                            //cout<<result;
                            delete_table(temp);
                        }
                        if (temp == "row")
                        {
                            myFile>>temp;
                            result+=temp;
                            if (temp == "from")
                            {
                                string table_name;
                                myFile>>table_name;
                                result+=table_name;
                                ifstream temp_table;
                                temp_table.open(table_name,ios_base::in);
                                if (temp_table.is_open())
                                {
                                    temp_table.close();
                                    myFile>>temp;
                                    result+=temp;
                                    if (temp=="where")
                                    {
                                        getline(myFile,temp);
                                        delete_row(table_name,temp);
                                    }
                                }
                            }
                        }
                        break;
                    CASE ("insert"):
                        myFile>>temp;
                        result+=temp;
                        if (temp =="into")
                        {
                            string table_name;
                            getline(myFile,table_name,'(');
                            getline(myFile,temp,')');
                            insert_row(table_name,temp);
                        }
                        break;
                    CASE ("select"):
                        break;
                    DEFAULT:
                        error(1);
                        break;
                }
                //QString temp1=QString::fromStdString(temp);
                //string temp2=temp1.toStdString();
                cout<<result<<endl;
            }
        }
        else
        {
            cout<<"There is no file"<<usage<<endl;
        }
        myFile.close();
    }
    return 0;
}
