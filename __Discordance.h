#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <deque>
namespace discordance{
    template<typename t>
    class vector : public std::vector<t> {
        using std::vector<t>::vector;
    public:

        template<typename s>
        inline operator vector<s>() {
            vector <s> x;
            x.resize(this->size());
            for (int i = 0; i < this->size(); i++) {
                x[i] = (s) this->at(i);
            }
            return x;
        }
        discordance::vector<t> slice(int first, int last){
            if(last<0){
                last=this->size()+last;
            }
            if(first<0){
                first=this->size()+first;
            }
            if(first<0||last<0){
                throw std::invalid_argument("index out of range");
            } else if(last<first){
                throw std::invalid_argument("last arguement before first arguement: slice invalid");
            }
            discordance::vector<t> dst(last-first);
            std::copy(this->begin()+first, this->begin()+last, dst.begin());
            return dst;
        }
    };

    template<typename t>
    class deque : public std::deque<t> {
        using std::deque<t>::deque;
    public:
        template<typename s>
        inline operator deque<s>() {
            deque <s> x;
            x.resize(this->size());
            for (int i = 0; i < this->size(); i++) {
                x[i] = (s) this->at(i);
            }
            return x;
        }
        template<typename s>
        inline operator vector<s>() {
            vector <s> x;
            x.resize(this->size());
            for (int i = 0; i < this->size(); i++) {
                x[i] = (s) this->at(i);
            }
            return x;
        }
        discordance::vector<t> slice(int first, int last){
            if(last<0){
                last=this->size()+last;
            }
            if(first<0){
                first=this->size()+first;
            }
            if(first<0||last<0){
                throw std::invalid_argument("index out of range");
            } else if(last<first){
                throw std::invalid_argument("last arguement before first arguement: slice invalid");
            }
            discordance::vector<t> dst(last-first);
            std::copy(this->begin()+first, this->begin()+last, dst.begin());
            return dst;
        }
    };
}

namespace discordance {
    class var {
    private:
        void *val{};
    public:
        enum type {
            Bool = 0, Integer = 1, Long = 2, LL = 3, Double = 4, LD = 5, String = 6, Untype = 7
        };
        type Type;

        inline var() : Type(type::Untype), val(nullptr) {};

        inline var(const bool a) : Type(type::Bool), val(new bool(a)) {};

        inline var(int a): Type(type::Bool), val(new int(a)){};

        inline var(const unsigned int a) : Type(type::Long), val(new long(a)) {};

        inline var(const long a) : Type(type::Long), val(new long(a)) {};

        inline var(const long long a) : Type(type::LL), val(new long long(a)) {};

        inline var(const double a) : Type(type::Double), val(new double(a)) {};

        inline var(const long double a) : Type(type::LD), val(new long double(a)) {};

        inline var(std::string a) : Type(type::String), val(new std::string(std::move(a))) {};

        inline var(const char a[]) : Type(type::String), val(new std::string(a)) {};

        //copy constructor
        inline var(const var &other) : Type(other.Type), val(nullptr) {
            switch (other.Type) {
                case Bool:
                    this->val = new bool(*reinterpret_cast<bool *>(other.val));
                    return;
                case Integer:
                    this->val = new int(*reinterpret_cast<int *>(other.val));
                    return;
                case Long:
                    this->val = new long(*reinterpret_cast<long *>(other.val));
                    return;
                case LL:
                    this->val = new long long(*reinterpret_cast<long long *>(other.val));
                    return;
                case Double:
                    this->val = new double(*reinterpret_cast<double *>(other.val));
                    return;
                case LD:
                    this->val = new long double(*reinterpret_cast<long double *>(other.val));
                    return;
                case String:
                    this->val = new std::string(*reinterpret_cast<std::string *>(other.val));
                    return;
                case Untype:
                    this->val = nullptr;
                    return;
            }
        };

        //conversion operations
        inline var &operator=(bool a) {
            this->val = new bool(a);
            Type = Bool;
            return *this;
        };

        inline var &operator=(int a) {
            this->val = new int(a);
            Type = Integer;
            return *this;
        };

        inline var &operator=(long a) {
            this->val = new long(a);
            Type = Long;
            return *this;
        };

        inline var &operator=(long long a) {
            this->val = new long long(a);
            Type = LL;
            return *this;
        };

        inline var &operator=(double a) {
            this->val = new double(a);
            Type = Double;
            return *this;
        };

        inline var &operator=(long double a) {
            this->val = new long double(a);
            Type = LD;
            return *this;
        };

        inline var &operator=(std::string a) {
            this->val = new std::string(std::move(a));
            Type = String;
            return *this;
        };

        inline var &operator=(const var &a) = default;

        template<typename t>
        inline var &operator=(t a) {
            std::cout << "anon" << std::endl;
            this->val = new t(a);
            Type = Untype;
            return *this;
        };

        //destructor
        inline ~var() {
            free(val);
            val = nullptr;
        }

        //arithmetic operations
        var &operator+(const var &a) const {
            switch (this->Type) {
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<bool *>(this->val) + *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) + *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<bool *>(this->val) + *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) + *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) + *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) + *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) + *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) + *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) + *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<long *>(this->val) + *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) + *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<long *>(this->val) + *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) +
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) +
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) +
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case String:
                    switch (a.Type) {
                        case String:
                            return *new var(*reinterpret_cast<std::string *>(this->val) +
                                            *reinterpret_cast<std::string *>(a.val));
                        default:
                            return *new var();
                    }
                default:
                    return *new var();
            }
        }

        void operator+=(const var &a) {
            switch (this->Type) {
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<bool *>(this->val) + *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) + *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(*reinterpret_cast<int *>(this->val) + *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<int *>(this->val) + *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(*reinterpret_cast<int *>(this->val) + *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) + *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) + *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) + *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<long *>(this->val) + *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) + *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) + *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) +
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) + *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) +
                                    *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) + *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) +
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case String:
                    switch (a.Type) {
                        case String:
                            *this = *new var(
                                    *reinterpret_cast<std::string *>(this->val) +
                                    *reinterpret_cast<std::string *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                default:
                    *this = *new var();
                    return;
            }
        }

        var &operator-(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<bool *>(this->val) - *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) - *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<bool *>(this->val) - *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) - *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) - *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) - *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) - *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) - *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) - *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<long *>(this->val) - *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) - *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<long *>(this->val) - *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) -
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) -
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) -
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case String:
                    std::string s;
                    switch (a.Type) {
                        default:
                            return *new var();
                        case Integer:
                            s = *reinterpret_cast<std::string *>(this->val);
                            s.erase(s.end() - *reinterpret_cast<int *>(a.val), s.end());
                            return *new var(s);
                        case Long:
                            s = *reinterpret_cast<std::string *>(this->val);
                            s.erase(s.end() - *reinterpret_cast<long *>(a.val), s.end());
                            return *new var(s);
                        case LL:
                            s = *reinterpret_cast<std::string *>(this->val);
                            s.erase(s.end() - *reinterpret_cast<long long *>(a.val), s.end());
                            return *new var(s);
                        case String:
                            size_t pos = 0;
                            s = *reinterpret_cast<std::string *>(this->val);
                            while ((pos = s.find(*reinterpret_cast<std::string *>(a.val))) != std::string::npos) {
                                s.erase(pos, reinterpret_cast<std::string *>(a.val)->length());
                            }
                            return *new var(s);
                    }
            }
        }

        void operator-=(const var &a) {
            switch (this->Type) {
                default:
                    *this = *new var();
                    return;
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<bool *>(this->val) - *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) - *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(*reinterpret_cast<int *>(this->val) - *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<int *>(this->val) - *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(*reinterpret_cast<int *>(this->val) - *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) - *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) - *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) - *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<long *>(this->val) - *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) - *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) - *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) -
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) - *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) -
                                    *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) - *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) -
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case String:
                    std::string s;
                    switch (a.Type) {
                        default:
                            *this = *new var();
                            return;
                        case Integer:
                            s = *reinterpret_cast<std::string *>(this->val);
                            s.erase(s.end() - *reinterpret_cast<int *>(a.val), s.end());
                            *this = *new var(s);
                            return;
                        case Long:
                            s = *reinterpret_cast<std::string *>(this->val);
                            s.erase(s.end() - *reinterpret_cast<long *>(a.val), s.end());
                            *this = *new var(s);
                            return;
                        case LL:
                            s = *reinterpret_cast<std::string *>(this->val);
                            s.erase(s.end() - *reinterpret_cast<long long *>(a.val), s.end());
                            *this = *new var(s);
                            return;
                        case String:
                            size_t pos = 0;
                            s = *reinterpret_cast<std::string *>(this->val);
                            while ((pos = s.find(*reinterpret_cast<std::string *>(a.val))) != std::string::npos) {
                                s.erase(pos, reinterpret_cast<std::string *>(a.val)->length());
                            }
                            *this = *new var(s);
                            return;
                    }
            }
        }

        var &operator*(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<bool *>(this->val) * *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) * *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<bool *>(this->val) * *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) * *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) * *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) * *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) * *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) * *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) * *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<long *>(this->val) * *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) * *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<long *>(this->val) * *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) *
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) *
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) *
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case String:
                    std::string r, s;
                    switch (a.Type) {
                        default:
                            return *new var();
                        case Integer:
                            s = *reinterpret_cast<std::string *>(this->val);
                            for (int c = 0; c < *reinterpret_cast<int *>(a.val); c++) {
                                r += s;
                            }
                            return *new var(r);
                        case Long:
                            r = *reinterpret_cast<std::string *>(this->val);
                            for (int c = 0; c < *reinterpret_cast<long *>(a.val); c++) {
                                r += s;
                            }
                            return *new var(r);
                        case LL:
                            r = *reinterpret_cast<std::string *>(this->val);
                            for (int c = 0; c < *reinterpret_cast<long long *>(a.val); c++) {
                                r += s;
                            }
                            return *new var(r);
                    }
            }
        }

        void operator*=(const var &a) {
            switch (this->Type) {
                default:
                    *this = *new var();
                    return;
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<bool *>(this->val) * *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) * *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(*reinterpret_cast<int *>(this->val) * *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<int *>(this->val) * *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(*reinterpret_cast<int *>(this->val) * *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) * *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) * *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) * *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<long *>(this->val) * *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) * *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) * *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) *
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) * *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) *
                                    *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) * *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) *
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case String:
                    std::string r, s;
                    switch (a.Type) {
                        default:
                            *this = *new var();
                            return;
                        case Integer:
                            s = *reinterpret_cast<std::string *>(this->val);
                            for (int c = 0; c < *reinterpret_cast<int *>(a.val); c++) {
                                r += s;
                            }
                            *this = *new var(r);
                            return;
                        case Long:
                            r = *reinterpret_cast<std::string *>(this->val);
                            for (int c = 0; c < *reinterpret_cast<long *>(a.val); c++) {
                                r += s;
                            }
                            *this = *new var(r);
                            return;
                        case LL:
                            r = *reinterpret_cast<std::string *>(this->val);
                            for (int c = 0; c < *reinterpret_cast<long long *>(a.val); c++) {
                                r += s;
                            }
                            *this = *new var(r);
                            return;
                    }
            }
        }

        var &operator/(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<bool *>(this->val) / *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) / *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<bool *>(this->val) / *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) / *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) / *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) / *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) / *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) / *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) / *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<long *>(this->val) / *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) / *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<long *>(this->val) / *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) /
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) /
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) /
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case String:
                    std::string r, s;
                    switch (a.Type) {
                        default:
                            return *new var();
                        case String:
                            s = *reinterpret_cast<std::string *>(this->val);
                            r = *reinterpret_cast<std::string *>(a.val);
                            if (reinterpret_cast<std::string *>(a.val)->length() == 0)
                                return *new var(0);
                            int count = 0;
                            for (size_t offset = s.find(r); offset != std::string::npos;
                                 offset = s.find(r, offset + r.length())) {
                                ++count;
                            }
                            return *new var(count);
                    }
            }
        }

        void operator/=(const var &a) {
            switch (this->Type) {
                default:
                    *this = *new var();
                    return;
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<bool *>(this->val) / *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<bool *>(this->val) / *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(*reinterpret_cast<int *>(this->val) / *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<int *>(this->val) / *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(*reinterpret_cast<int *>(this->val) / *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) / *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) / *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<int *>(this->val) / *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(*reinterpret_cast<long *>(this->val) / *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long *>(this->val) / *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) / *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long long *>(this->val) /
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<double *>(this->val) / *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<bool *>(a.val));
                            return;
                        case Integer:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<int *>(a.val));
                            return;
                        case Long:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<long *>(a.val));
                            return;
                        case LL:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) /
                                    *reinterpret_cast<long long *>(a.val));
                            return;
                        case Double:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) / *reinterpret_cast<double *>(a.val));
                            return;
                        case LD:
                            *this = *new var(
                                    *reinterpret_cast<long double *>(this->val) /
                                    *reinterpret_cast<long double *>(a.val));
                            return;
                        default:
                            *this = *new var();
                            return;
                    }
                case String:
                    std::string r, s;
                    switch (a.Type) {
                        default:
                            *this = *new var();
                            return;
                        case String:
                            s = *reinterpret_cast<std::string *>(this->val);
                            r = *reinterpret_cast<std::string *>(a.val);
                            if (reinterpret_cast<std::string *>(a.val)->length() == 0) {
                                *this = *new var(0);
                                return;
                            }
                            int count = 0;
                            for (size_t offset = s.find(r); offset != std::string::npos;
                                 offset = s.find(r, offset + r.length())) {
                                ++count;
                            }
                            *this = *new var(count);
                            return;
                    }
            }
        }

        var &operator%(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<bool *>(this->val) % *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) % *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<bool *>(this->val) % *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) % *reinterpret_cast<long long *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) % *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) % *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) % *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) % *reinterpret_cast<long long *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<long *>(this->val) % *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) % *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<long *>(this->val) % *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) % *reinterpret_cast<long long *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) % *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) % *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) % *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) % *reinterpret_cast<long long *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        var &operator>(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<bool *>(this->val) > *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) > *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<bool *>(this->val) > *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) > *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) > *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) > *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) > *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) > *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) > *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) > *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) > *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) > *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<long *>(this->val) > *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) > *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<long *>(this->val) > *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) > *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) > *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) > *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) > *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) > *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) > *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) > *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) > *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) >
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) > *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) > *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) > *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) > *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) > *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) > *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) > *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) > *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) > *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) > *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        var &operator<(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<bool *>(this->val) < *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) < *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<bool *>(this->val) < *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) < *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) < *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) < *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) < *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) < *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) < *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) < *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) < *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) < *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<long *>(this->val) < *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) < *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<long *>(this->val) < *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) < *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) < *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) < *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) < *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) < *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) < *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) < *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) < *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) <
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) < *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) < *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) < *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) < *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) < *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) < *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) < *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) < *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) < *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) < *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        var &operator>=(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) >= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) >= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) >= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) >= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) >= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) >= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) >= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) >= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) >= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) >= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) >= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) >= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) >= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) >= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) >= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) >= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) >= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) >= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) >= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) >= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) >= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) >= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) >= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) >=
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) >= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) >= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) >= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) >= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) >= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) >= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >=
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) >=
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        var &operator<=(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) <= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) <= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) <= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) <= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) <= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) <= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) <= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) <= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) <= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) <= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) <= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) <= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) <= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) <= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) <= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) <= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) <= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) <= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) <= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) <= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) <= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) <= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) <= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) <=
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) <= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) <= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) <= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) <= *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) <= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) <= *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <= *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <= *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <= *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <=
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <= *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) <=
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        var &operator==(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) == *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) == *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) == *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) == *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) == *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) == *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) == *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) == *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) == *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) == *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) == *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) == *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) == *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) == *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) == *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) == *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) == *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) == *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) == *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) == *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) == *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) == *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) == *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) ==
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) == *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) == *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) == *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) == *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) == *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) == *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) == *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) == *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) == *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) ==
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) == *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) ==
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case String:
                    switch (a.Type) {
                        default:
                            return *new var();
                        case String:
                            return *new var(
                                    *reinterpret_cast<std::string *>(this->val) ==
                                    *reinterpret_cast<std::string *>(a.val));
                    }
            }
        }

        var &operator&&(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) && *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) && *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) && *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) && *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) && *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) && *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) && *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) && *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) && *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) && *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) && *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) && *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) && *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) && *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) && *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) && *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) && *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) && *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) && *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) && *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) && *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) && *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) && *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) &&
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) && *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) && *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) && *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) && *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) && *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) && *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) && *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) && *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) && *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) &&
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) && *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) &&
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        var &operator||(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) || *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) || *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) || *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) || *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) || *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) || *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) || *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) || *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) || *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) || *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) || *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) || *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) || *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) || *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) || *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) || *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) || *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) || *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) || *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) || *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) || *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) || *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) || *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) ||
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) || *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) || *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) || *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) || *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) || *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) || *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) || *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) || *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) || *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) ||
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) || *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) ||
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        var &operator!=(const var &a) const {
            switch (this->Type) {
                default:
                    return *new var();
                case Bool:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) != *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<bool *>(this->val) != *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) != *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) != *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) != *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<bool *>(this->val) != *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Integer:
                    switch (a.Type) {
                        case Bool:
                            return *new var(*reinterpret_cast<int *>(this->val) != *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<int *>(this->val) != *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(*reinterpret_cast<int *>(this->val) != *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) != *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) != *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<int *>(this->val) != *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Long:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) != *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(*reinterpret_cast<long *>(this->val) != *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) != *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) != *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) != *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long *>(this->val) != *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LL:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) != *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) != *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) != *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) != *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) != *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long long *>(this->val) !=
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case Double:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) != *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) != *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) != *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) != *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) != *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<double *>(this->val) != *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
                case LD:
                    switch (a.Type) {
                        case Bool:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) != *reinterpret_cast<bool *>(a.val));
                        case Integer:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) != *reinterpret_cast<int *>(a.val));
                        case Long:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) != *reinterpret_cast<long *>(a.val));
                        case LL:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) !=
                                    *reinterpret_cast<long long *>(a.val));
                        case Double:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) != *reinterpret_cast<double *>(a.val));
                        case LD:
                            return *new var(
                                    *reinterpret_cast<long double *>(this->val) !=
                                    *reinterpret_cast<long double *>(a.val));
                        default:
                            return *new var();
                    }
            }
        }

        friend std::ostream &operator<<(std::ostream &output, const var &x) {
            switch (x.Type) {
                case Bool:
                    return output << std::to_string(*reinterpret_cast<bool *>(x.val));
                case Integer:
                    return output << std::to_string(*reinterpret_cast<int *>(x.val));
                case Long:
                    return output << std::to_string(*reinterpret_cast<long *>(x.val));
                case LL:
                    return output << std::to_string(*reinterpret_cast<long long *>(x.val));
                case Double:
                    return output << std::to_string(*reinterpret_cast<double *>(x.val));
                case LD:
                    return output << std::to_string(*reinterpret_cast<long double *>(x.val));
                case String:
                    return output << *reinterpret_cast<std::string *>(x.val);
                case Untype:
                default:
                    return output << "Untype";
            }
        }

        friend std::istream &operator>>(std::istream &input, var &x) {
            std::string value;
            input >> value;
            bool isint = true, isfloat = true;
            try {
                if (value != std::to_string(stoll(value))) {
                    isint = false;
                }
            } catch (std::invalid_argument) {
                isint = false;
            }
            try {
                if (value != std::to_string(stold(value))) {
                    isfloat = false;
                }
            } catch (std::invalid_argument) {
                isfloat = false;
            }
            if (isint) {
                x = stoll(value);
            } else if (isfloat) {
                x = stold(value);
            } else {
                x = value;
            }
            return input;
        }

        [[nodiscard]] inline auto length() const {
            switch (this->Type) {
                case String:
                    return *new var((int) reinterpret_cast<std::string *>(this->val)->length());
                default:
                    return *new var();
            }
        }

        //implicit casting operators
        inline operator short() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<short int>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<short int>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<short int>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<short int>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<short int>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<short int>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator bool() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<bool>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<bool>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<bool>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<bool>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<bool>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<bool>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator int() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<int>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<int>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<int>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<int>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<int>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<int>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator long() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<long>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<long>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<long>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<long>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<long>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<long>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator long long() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<long long>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<long long>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<long long>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<long long>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<long long>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<long long>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator float() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<float>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<float>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<float>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<float>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<float>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<float>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator double() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<double>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<double>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<double>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<double>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<double>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<double>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator long double() const {
            switch (Type) {
                default:
                    throw std::bad_cast();
                case Bool:
                    return static_cast<long double>(*reinterpret_cast<bool *>(val));
                case Integer:
                    return static_cast<long double>(*reinterpret_cast<int *>(val));
                case Long:
                    return static_cast<long double>(*reinterpret_cast<long *>(val));
                case LL:
                    return static_cast<long double>(*reinterpret_cast<long long *>(val));
                case Double:
                    return static_cast<long double>(*reinterpret_cast<double *>(val));
                case LD:
                    return static_cast<long double>(*reinterpret_cast<long double *>(val));
            }
        }

        inline operator std::string() const {
            switch (Type) {
                default:
                    std::cout<<"not string";
                    throw std::bad_cast();
                case String:
                    return *reinterpret_cast<std::string *>(val);
            }
        }

        friend std::string operator+(const char *a, const var& b){
            std::string b_string=b;
            std::string a_string=a;
            return a_string+b_string;
        }
        template<typename t>
        friend t operator+(t a, const var& b){
            return a+t(b);
        }
        friend std::string operator+(const var& a, const char *b){
            std::string b_string=b;
            std::string a_string=a;
            return a_string+b_string;
        }
        template<typename t>
        friend t operator+(const var&a, t b){
            return t(a)+b;
        }
        template<typename t>
        friend t operator-(t a, const var& b){
            return a-t(b);
        }
        template<typename t>
        friend t operator-(const var&a, t b){
            return t(a)-b;
        }
        template<typename t>
        friend t operator*(t a, const var& b){
            return a*t(b);
        }
        template<typename t>
        friend t operator*(const var&a, t b){
            return t(a)*b;
        }
        template<typename t>
        friend t operator/(t a, const var& b){
            return a/t(b);
        }
        template<typename t>
        friend t operator/(const var&a, t b){
            return t(a)/b;
        }
        template<typename t>
        friend t operator%(t a, const var& b){
            return a%t(b);
        }
        template<typename t>
        friend t operator%(const var&a, t b){
            return t(a)%b;
        }
        template<typename t>
        friend t operator>(t a, const var& b){
            return a>t(b);
        }
        template<typename t>
        friend t operator>(const var&a, t b){
            return t(a)>b;
        }
        template<typename t>
        friend t operator<(t a, const var& b){
            return a<t(b);
        }
        template<typename t>
        friend t operator<(const var&a, t b){
            return t(a)<b;
        }
        template<typename t>
        friend t operator>=(t a, const var& b){
            return a>=t(b);
        }
        template<typename t>
        friend t operator>=(const var&a, t b){
            return t(a)>=b;
        }
        template<typename t>
        friend t operator<=(t a, const var& b){
            return a<=t(b);
        }
        template<typename t>
        friend t operator<=(const var&a, t b){
            return t(a)<=b;
        }
        friend bool operator==(const char *a, const var& b){
            std::string a_string=a;
            std::string b_string=b;
            return a_string==b_string;
        }
        friend bool operator==(const var& a, const char *b){
            std::string b_string=b;
            std::string a_string=a;
            return a_string==b_string;
        }
        template<typename t>
        friend t operator==(t a, const var& b) {
            return a == t(b);
        }
        template<typename t>
        friend t operator==(const var&a, t b){
            return t(a)==b;
        }
        template<typename t>
        friend t operator&&(t a, const var& b){
            return a&&t(b);
        }
        template<typename t>
        friend t operator&&(const var&a, t b){
            return t(a)&&b;
        }
        template<typename t>
        friend t operator||(t a, const var& b){
            return a||t(b);
        }
        template<typename t>
        friend t operator||(const var&a, t b){
            return t(a)||b;
        }
    };
}
