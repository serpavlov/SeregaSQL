#include <iostream>
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

int create_table(string name, string scolumns)
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
        out<<endl;
        out.close();
    }
    else error(3);
    return 0;
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

                for (int i=0;i<=n;i++)
                {
                    out<<cols[i];
                    if (i<=n-1) out<<',';
                }
                out<<endl;
            }
            else error(3);
            out.close();
        }
        else error(4);

    }
    else error(3);
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
        ofstream out;
        out.open("temp", ios_base::trunc);
        out<<scolumns<<endl;
        while (!in.eof())
        {
            getline(in,temp);
            int zap_count=0;
            string key_finder;
            for (int i=0;i<temp.size();i++)
            {
                if (temp[i]==',')
                {
                    zap_count++;
                }
                else if(zap_count+1==n_col)
                {
                    key_finder+=temp[i];
                }
                if(zap_count+1>n_col) 
                    break;
            }
            if (zap_count == n_col)
            {
                if (key_finder!=key)
                {
                    out<<temp<<endl;
                }
            }
        }
        in.close();
        out.close();
        delete_table(name);
        rename("temp",name.c_str());
    } else error(12);
}
