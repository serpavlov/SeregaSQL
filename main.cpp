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
                string inputss,temp,result;
                getline(myFile,inputss);
                istringstream iss (inputss);

                iss>>temp;
                result+=temp+" ";
                SWITCH (temp)
                {
                    CASE ("create"):
                        iss>>temp;
                        result+=temp;
                        if (temp =="table")
                        {
                            string table_name;
                            iss.get();
                            getline(iss,table_name,'(');
                            getline(iss,temp,')');
                            result+=table_name+" ";
                            result+=temp;
                            create_table(table_name,temp);
                        }
                        break;
                    CASE ("delete"):
                        iss>>temp;
                        result+=temp+" ";
                        if (temp =="table")
                        {
                            iss>>temp;
                            result+=temp;
                            delete_table(temp);
                        }
                        if (temp == "row")
                        {
                            iss>>temp;
                            result+=temp;
                            if (temp == "from")
                            {
                                string table_name;
                                iss>>table_name;
                                result+=table_name;
                                iss>>temp;
                                result+=temp;
                                if (temp=="where")
                                {
                                    iss.get();
                                    getline(iss,temp);
                                    result+=temp;
                                    delete_row(table_name,temp);
                                }
                            }
                        }
                        break;
                    CASE ("insert"):
                        iss>>temp;
                        result+=temp;
                        if (temp =="into")
                        {
                            string table_name;
                            iss.get();
                            getline(iss,table_name,'(');
                            getline(iss,temp,')');
                            insert_row(table_name,temp);
                        }
                        break;
                    CASE ("copy"):
                            iss>>temp;
                            result+=temp;
                            if (temp =="table")
                            {
                                string table_name1,table_name2;
                                iss>>table_name1>>table_name2;
                                copy_table(table_name1,table_name2);
                            }
                            break;
                    CASE ("select"):
                        string tablename, swistr;
                        iss>>tablename;
                        iss>>temp;
                        while(temp=="add")
                        {
                            //iss>>tablename;

                            iss>>swistr;
                            SWITCH (swistr)
                            {
                                CASE ("sort"):
                                    iss>>temp;
                                    if (temp == "by")
                                    {
                                        iss>>temp;
                                        sort_by_column(tablename,temp);
                                    }
                                    break;
                                CASE ("add"):
                                    iss>>temp;
                                    if (temp == "by")
                                    {
                                        iss>>temp;
                                        sort_by_column("temp",temp);
                                    }
                                    break;
                            }
                            iss>>temp;
                        }
                        break;

                    /*DEFAULT:
                        error(1);
                        break;*/

                }
                result=inputss;
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
