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
            int counter=0;
            bool is_error = false;
            cout<<"Working..."<<endl;
            while (!myFile.eof() && !is_error)
            {
                counter++;
                string inputss,temp,result;
                getline(myFile,inputss);
                istringstream iss (inputss);
                iss>>temp;
                SWITCH (temp)
                {
                    CASE ("create"):
                    {
                        iss>>temp;
                        result+=temp;
                        if (temp =="table")
                        {
                            string table_name;
                            iss.get();
                            getline(iss,table_name,'(');
                            if (table_name!="temp_sort"||table_name!="temp2"||table_name!="temp")
                            {
                                getline(iss,temp,')');
                                result+=table_name+" ";
                                result+=temp;
                                create_table(table_name,temp);
                            }
                            else
                            {
                                is_error = true;
                                error(110);
                            }
                        }
                        else
                        {
                            is_error = true;
                            error(101);
                        }
                        break;
                    }
                    CASE ("delete"):
                    {
                        iss>>temp;
                        result+=temp+" ";
                        if (temp =="table")
                        {
                            iss>>temp;
                            result+=temp;
                            delete_table(temp);
                        }
                        else if (temp == "row")
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
                                } else
                                {
                                    is_error = true;
                                    error(104);
                                }
                            } else
                            {
                                is_error = true;
                                error(103);
                            }
                        } else
                        {
                            is_error = true;
                            error(102);
                        }
                        break;
                    }
                    CASE ("insert"):
                    {
                        iss>>temp;
                        result+=temp;
                        if (temp =="into")
                        {
                            string table_name;
                            iss.get();
                            getline(iss,table_name,'(');
                            getline(iss,temp,')');
                            insert_row(table_name,temp);
                        } else
                        {
                            is_error = true;
                            error(106);
                        }
                        break;
                    }
                    CASE ("copy"):
                    {
                        iss>>temp;
                        result+=temp;
                        if (temp =="table")
                        {
                            string table_name1,table_name2;
                            iss>>table_name1>>table_name2;
                            copy_table(table_name1,table_name2);
                        }
                        else
                        {
                            is_error = true;
                            error(107);
                        }
                        break;
                    }
                    CASE ("sort"):
                    {
                        iss>>temp;
                        result+=temp;
                        if (temp =="table")
                        {
                            string table_name;
                            iss>>table_name>>temp;
                            if (temp == "by")
                                sort_by_column(table_name,temp);
                        } else
                        {
                            is_error = true;
                            error(108);
                        }
                        break;
                    }
                    CASE ("select"):
                    {
                        string tablename, swistr;
                        iss>>tablename;
                        iss>>temp;
                        int limit=-2;
                        copy_table(tablename,"temp");
                        while(temp=="add" && !is_error && iss.good())
                        {
                            iss>>swistr;
                            SWITCH (swistr)
                            {
                                CASE ("sort"):
                                {
                                    iss>>temp;
                                    if (temp == "by")
                                    {
                                        iss>>temp;
                                        sort_by_column("temp",temp);
                                    }else
                                    {
                                        is_error = true;
                                        error(109);
                                    }
                                    break;
                                }
                                CASE ("limit"):
                                {
                                    iss>>limit;
                                    if (limit<=0)
                                    {
                                        is_error = true;
                                        error(109);
                                    }
                                    break;
                                }
                            }
                            iss>>temp;
                        }
                        select("temp",limit);
                        delete_table("temp");
                        break;
                    }
                    DEFAULT:
                    {
                        is_error=true;
                        error(100);
                        break;
                    }
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
