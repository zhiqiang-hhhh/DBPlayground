#include <iostream>
#include <vector>

#include "src/Core/MiniKV.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

struct Employee
{
    int         id;
    std::string name;
    int         age;

    Employee(int id_,std::string name_,int age_):id(id_),name(name_),age(age_){}

    friend std::ostream& operator<<(std::ostream& os,const Employee& e)
    {
        os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
        return os;
    }
};

struct id {};
struct name {};
struct age {};

template<typename Tag, typename MultiIndexContainer>
void print_out_by(const MultiIndexContainer& s)
{
    const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i=
            get<Tag>(s);

    typedef typename MultiIndexContainer::value_type value_type;

    /* dump the elements of the index to cout */
    std::copy(i.begin(),i.end(),std::ostream_iterator<value_type>(std::cout));
}


int main(int argc, char** argv) {
//    miniKV::MiniKV db;
//
//    std::vector<std::pair<miniKV::key_t , miniKV::value_t>> entries;
//    for (int i = 0; i < 100; ++i) {
//        entries.push_back(std::pair<miniKV::key_t, miniKV::value_t>(i, i + 100));
//    }
//
//    for (auto entry : entries) {
//        db.insert(entry.first, entry.second);
//    }
//
//    for (auto entry : entries) {
//        std::cout << entry.first << " : " << entry.second
//        <<  " in database: " << entry.first << " : " <<db.get(entry.first) << std::endl;
//    }

    using boost::multi_index_container;
    using namespace boost::multi_index;

    using EmployeeSet =
            multi_index_container<
                    Employee,
                    indexed_by<
                            ordered_unique<
                                tag<id>,
                                BOOST_MULTI_INDEX_MEMBER(Employee, int, id)>,
                            ordered_non_unique<
                                tag<name>,
                                BOOST_MULTI_INDEX_MEMBER(Employee, std::string, name)>,
                            ordered_non_unique<
                                tag<age>,
                                BOOST_MULTI_INDEX_MEMBER(Employee, int, age)>
                    >
            >;

    EmployeeSet employees {
        Employee(0, "Joe", 32),
        Employee(1, "Robert", 27),
        Employee(2, "John", 57)
    };


    std::cout<<"by ID"<<std::endl;
    print_out_by<id>(employees);
    std::cout<<std::endl;

    std::cout<<"by name"<<std::endl;
    print_out_by<name>(employees);
    std::cout<<std::endl;

    std::cout<<"by age"<<std::endl;
    print_out_by<age>(employees);
    std::cout<<std::endl;

    return 0;

}
