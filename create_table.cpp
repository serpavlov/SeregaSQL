#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "my_column.h"
//#include "errors.cpp"

using namespace std;

void error(int e)
{
    cout<<"You have "<<e<<" error"<<endl;
}

void delete_table(string tablename)
{
    ofstream out;
    out.open(tablename, ios_base::trunc);
    out.close();
    if (remove(tablename.c_str())!=0)
        error(12);
}

bool is_valid_type(string stype)
{
    string* types = new string[3];
    types[0]="string";
    types[1]="int";
    types[2]="real";
    for (int i=0;i<sizeof(types);i++)
    {
        if (stype.compare(types[i])) return true;
    }
    error(9);
    return false;
}

my_column* get_parameters(string line)
{
    my_column* array;
    int kol_count=0;
    int zap_count=0;
    int n=line.size();
    for (int i=0;i<n;i++)
    {
        if (line[i]==':') kol_count++;
        if (line[i]==',') zap_count++;
    }
    if (kol_count-1!=zap_count) error(2);
    else
    {
        string tname,ttype;
        array = new my_column[kol_count];
        int col_count=0;
        char search_for=':';
        for (int i=0;i<=n;i++)
        {
            if (search_for==':')
            {
                if (line[i]!=search_for)
                    tname+=line[i];
                else search_for=',';
            }
            else if (search_for == ',')// || i>=n)
            {
                if (line[i]!=search_for && i<n)
                    ttype+=line[i];
                else
                {
                    search_for=':';
                    if (is_valid_type(ttype) && tname.size()>0)
                    {
                        array[col_count] = my_column(tname,ttype,kol_count);
                        tname.clear();
                        ttype.clear();
                        col_count++;
                    }
                    else break;//спорный момент лучше ретурн
                }
            }
        }
    }
    return array;
}

bool create_table(string name, string scolumns)
{
    ifstream in;
    in.open(name,ios_base::in);
    if (!in.is_open())
    {
        ofstream out;
        out.open(name,ios_base::out);//можно добавить невозможность перезаписать таблицу
        if (out.is_open())
        {
            my_column* columns = get_parameters(scolumns);
            for (int i=0;i<columns[0].n;i++)
            {
                out<<columns[i].name<<':'<<columns[i].type;
                if (i < (columns[0].n-1)) out<<',';
            }
            //out<<endl;
            out.close();
            return true;
        }
        else error(3);
    }
    else error(201);
    return false;
}

bool check_type(string stype, string srow)
{
    if (stype == "string") return true;
    else if (stype == "int")
    {
        for (int i=0;i<srow.size();i++)
        {
            if (!isdigit(srow[i])) return false;
        }
        return true;
    }
    else if (stype == "real")
    {
        int dot_counter=0;
        for (int i=0;i<srow.size();i++)
        {
            if (!isdigit(srow[i]))
            {
                if(srow[i]=='.' && dot_counter<1) dot_counter++;
                else return false;
            }
        }
        return true;
    }
    return false;
}

int insert_row(string name, string srow)
{
    ifstream in;
    in.open(name,ios_base::in);
    if(in.is_open())
    {
        string scolumns;
        getline(in,scolumns);
        in.close();
        my_column* columns = get_parameters(scolumns);
        int n=0;
        for (int i=0;i<srow.size();i++)
        {
            if (srow[i]==',') n++;
        }
        if (n+1 == columns[0].n)
        {
            string* cols = new string[n+1];
            int iter=0;
            for(int i=0;i<srow.size();i++)
            {
                if (srow[i]!=',') cols[iter]+=srow[i];
                else iter++;
            }
            for (int i=0;i<columns[i].n;i++)
            {
                if (!check_type(columns[i].type,cols[i])) error(9);
            }
            ofstream out;
            out.open(name,ios_base::app);
            if (out.is_open())
            {

                out<<endl;
                for (int i=0;i<=n;i++)
                {
                    out<<cols[i];
                    if (i<=n-1) out<<',';
                }
            }
            else error(3);
            out.close();
        }
        else error(4);

    }
    else error(3);
    return 0;
}

string find_column_string(int n, string sstring)
{
    string key_finder;
    int zap_count=0;
    for (int i=0;i<sstring.size();i++)
    {
        if (sstring[i]==',')
        {
            zap_count++;
        }
        else if(zap_count+1==n)
        {
            key_finder+=sstring[i];
        }
        if(zap_count+1>n)
            break;
    }
    return key_finder;
}
int copy_table(string sname, string oname)
{
    ifstream in;
    in.open(sname,ios_base::in);
    int i=1;
    if(in.is_open())
    {
        string scolumns;
        getline(in,scolumns);
        //create_table(oname,scolumns);
        cout<<"creating table"<<endl;
        if (create_table(oname,scolumns))
        while(!in.eof())
        {
            getline(in,scolumns);
            cout<<"inserting "<<i<<" row"<<endl;
            insert_row(oname,scolumns);
            i++;
        }
        in.close();
    }
    else error(200);
    return 0;
    //not working now
}
int sort_by_column(string name, string parameter)
{
    ifstream in;
    in.open(name,ios_base::in);
    if(in.is_open())
    {
        string scolumns;
        getline(in,scolumns);
        my_column* columns = get_parameters(scolumns);
        int n_col;
        for(int i=0;i<columns[0].n;i++)
        {
            if (columns[i].name==parameter) n_col=i+1;
        }
        string temp,key_finder,min_key,max_key;
        //ofstream out;
        //out.open("temp",ios_base::trunc | ios_base::out);
        //out<<scolumns<<endl;
        create_table("temp",scolumns);
        in>>temp;
        key_finder=find_column_string(n_col,temp);
        max_key=min_key=key_finder;
        insert_row("temp",temp);
        //out<<temp;
        //out.close();
        while(!in.eof())
        {
            in >> temp;
            //cout<<temp<<endl;
            key_finder = find_column_string(n_col,temp);
            if (key_finder>=max_key)
            {
                cout<<"more";
                //out.open("temp",ios_base::app);
                insert_row("temp",temp);
                //out << temp << endl;
                //out.close();
                max_key=key_finder;
            }
            else if (key_finder<=min_key)
            {
                cout<<"less";
                min_key=key_finder;
                //ofstream out2;
                //out2.open("temp2",ios_base::trunc | ios_base::out);

                //cout<<"out state is "<<out.good()<<endl;
                //out.close();
                ifstream in2;
                in2.open("temp",ios_base::in);
                string temp2;
                if (in2.good())
                {
                    getline(in2,temp2);
                    create_table("temp2",temp2);
                    //out2<<temp2<<endl;
                    insert_row("temp2",temp);
                    //out2<<temp<<endl;
                    while (!in2.eof())
                    {
                        getline(in2,temp2);
                        insert_row("temp2",temp2);
                        //out2<<temp2<<endl;
                    }
                    //out2.close();
                    in2.close();
                    delete_table("temp");
                    copy_table("temp2","temp");
                    delete_table("temp2");
                }
            }
            else
            {
                cout<<"hz"<<endl;

                //ofstream out2;
                //out2.open("temp2",ios_base::trunc | ios_base::out);
                //cout<<"out state is "<<out.good()<<endl;
                //out.close();
                ifstream in2;
                in2.open("temp",ios_base::in);
                string temp2,key_finder2,key_finder2_last=key_finder;
                int trig=0;
                if (in2.good())
                {

                    getline(in2,temp2);
                    create_table("temp2",temp2);
                    //out2<<temp2<<endl;
                    while (!in2.eof())
                    {
                        getline(in2,temp2);
                        key_finder2 = find_column_string(n_col,temp2);
                        if (key_finder<=key_finder2 && trig==0)
                        {
                            cout<< temp <<" rule is working, but "<<key_finder<<" < "<<key_finder2<< endl;
                            insert_row("temp2",temp);
                            //out2<<temp<<endl;
                            trig=1;
                        }
                        cout<<temp2<<endl;
                        insert_row("temp2",temp2);
                        //out2<<temp2<<endl;

                        key_finder2_last=key_finder2;
                    }
                    //out2.close();
                    in2.close();
                    delete_table("temp");
                    rename("temp2","temp");

                }else error(1234);
                //out.close();
                //out.open("temp",ios_base::ate);
            }
        }
        in.close();
        //out.close();
        //delete_table(name);
        //rename("temp",name.c_str());
    } else error(12);
    return 0;
}

void delete_row(string name, string parameter)
{
    ifstream in;
    in.open(name,ios_base::in);
    if(in.is_open())
    {
        string scolumns;
        getline(in,scolumns);
        my_column* columns = get_parameters(scolumns);
        string column,key;
        int n;
        for (n=0;n<parameter.size();n++)
        {
            if (parameter[n]!='=')
            {
                column+=parameter[n];
            }
            else break;
        }
        for (int i=n+1;i<parameter.size();i++)
        {
            key+=parameter[i];
        }
        int n_col;
        for(int i=0;i<columns[0].n;i++)
        {
            if (columns[i].name==column) n_col=i+1;
        }
        string temp;
        //ofstream out;
        //out.open("temp", ios_base::trunc | ios_base::trunc);
        //out<<scolumns<<endl;
        create_table("temp",scolumns);
        while (!in.eof())
        {
            getline(in,temp);
            string key_finder=find_column_string(n_col,temp);
            if (key_finder.size()>0)
            {
                if (key_finder!=key)
                {
                    //out<<temp<<endl;
                    insert_row("temp",temp);
                }
            }
        }
        in.close();
        //out.close();
        delete_table(name);
        copy_table("temp",name);
        delete_table("temp");
        //rename("temp",name.c_str());
    } else error(12);
}
