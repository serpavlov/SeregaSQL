using namespace std;

class my_column {
    public:
        string name;
        string type;
        int n;
        my_column (){}
        my_column (string sname,string stype,int sn) : name(sname), type(stype), n(sn) {}
        my_column(const my_column& obj)
        {
            name=obj.name;
            type=obj.type;
            n=obj.n;
        }
        ~my_column(){}
};
class my_table {
    public:
        string table_name;
        my_column* columns;
};
