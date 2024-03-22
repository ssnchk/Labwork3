#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdint>

struct Node {
    int16_t x;
    int16_t y;
    uint64_t val;
    Node* next;
    Node(int16_t _x,int16_t _y,uint64_t _val) : x(_x), y(_y), val(_val), next(nullptr){}
    ~Node()= default;
};

struct Sandpile;

struct Field;

struct list {
    Node * first;

    list() : first(nullptr) {}
    ~list()= default;
    bool is_empty() {
        return first == nullptr;
    }

    void push_forward(int16_t _x,int16_t _y,uint64_t _val) {
        Node * p = new Node(_x,_y,_val);
        if (is_empty()) {
            first = p;
            return;
        }
        p->next=first;
        first=p;
    }

    void push_forward(Node* q) {
        Node * p = new Node(q->x,q->y,q->val);
        if (is_empty()) {
            first = p;
            return;
        }
        p->next=first;
        first=p;
    }
};


struct Field{
    list unstable;
    uint32_t height{};
    uint32_t width{};
    uint64_t* m_field{};
    Field(uint32_t _height,uint32_t _width):height(_height),width(_width){
        m_field=new uint64_t[height*width]{0};
    }
    Field()=default;
    ~Field()= default;
    void print();
};

struct Sandpile {
    Field grid{};
    Sandpile(Field &_grid){
        grid=_grid;
    };
    uint64_t get_value(uint32_t, uint32_t);
    void scattering(uint16_t, uint16_t,uint32_t*,uint32_t*);
    void add_all_ways(uint32_t,uint32_t,bool*,Node*,uint32_t,uint32_t);
    void expandleft();
    void expandright();
    void expandtop();
    void expandbot();
    void all_field_skattering(uint32_t max_iter,uint32_t freq,char* path);
    bool have_node(int16_t x,int16_t y,Node* p,Node* now,uint32_t l,uint32_t t);
};

struct Color {
    uint8_t r, g, b;
    uint8_t reserved=0;
    Color() : r(255), g(255), b(255){}
    Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
};

struct Palette{
    Color all_colors[5]{Color(0,0,0),Color(0,255,0),Color(255,0,0),Color(255,255,255),Color(255,255,0)};
};

struct Image {
    unsigned int m_width;
    unsigned int m_height;
    Color* m_colors;
    Image(uint16_t width,uint16_t height) : m_width(width), m_height(height) {    };
    ~Image() = default;

    void Export(const char* path,Sandpile sandpile) const;
};
