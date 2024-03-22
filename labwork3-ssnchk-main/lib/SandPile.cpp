#include "SandPile.h"
void Field::print() {
    for (uint64_t i = 0; i < width * height; i++) {
        std::cout << (int)m_field[i] << ' ';
        if ((i + 1) % width == 0 && i != 0) {
            std::cout << '\n';
        }
    }
}

void Sandpile::expandleft() {
    uint64_t * tmp;
    tmp = new uint64_t[(grid.width + 1) * grid.height]{0};

    for (uint64_t x = 0; x < grid.width; ++x) {
        for (uint64_t y = 0; y < grid.height; ++y) {
            tmp[y * (grid.width + 1) + x + 1] = get_value(x,y);
        }
    }
    delete grid.m_field;
    ++grid.width;
    grid.m_field = tmp;
}

uint64_t Sandpile::get_value(uint32_t x,uint32_t y){
    return grid.m_field[y*grid.width+x];
}

void Sandpile::expandright() {
    uint64_t * tmp;
    tmp = new uint64_t[(grid.width + 1) * grid.height]{0};
    for (uint64_t x = 0; x < grid.width; ++x) {
        for (uint64_t y = 0; y < grid.height; ++y) {
            tmp[y * (grid.width + 1) + x] = get_value(x,y);
        }
    }
    delete grid.m_field;
    ++grid.width;
    grid.m_field = tmp;
}

void Sandpile::expandtop() {
    uint64_t * tmp;
    tmp = new uint64_t[grid.width * (grid.height + 1)]{0};
    for (uint64_t x = 0; x < grid.width; ++x) {
        for (uint64_t y = 0; y < grid.height; ++y) {
            tmp[(y + 1) * (grid.width) + x] = get_value(x,y);
        }
    }
    delete grid.m_field;
    ++grid.height;
    grid.m_field = tmp;
}

void Sandpile::expandbot() {
    uint64_t * tmp;
    tmp = new uint64_t[grid.width * (grid.height + 1)]{0};
    for (uint64_t x = 0; x < grid.width; ++x) {
        for (uint64_t y = 0; y < grid.height; ++y) {
            tmp[y * (grid.width) + x] = get_value(x,y);
        }
    }
    delete grid.m_field;
    ++grid.height;
    grid.m_field = tmp;
}

void Sandpile::scattering(uint16_t x, uint16_t y,uint32_t* left_shift,uint32_t* top_shift) {
    grid.m_field[y*grid.width+x]-=4;
    if (x==0){
        ++x;
        *left_shift+=1;
        expandleft();
    }

    if (x==grid.width-1){
        expandright();
    }

    if (y==0){
        y++;
        *top_shift+=1;
        expandtop();
    }

    if (y==grid.height-1){
        expandbot();
    }
    grid.m_field[y*grid.width+x-1]++;
    grid.m_field[y*grid.width+x+1]++;
    grid.m_field[(y+1)*grid.width+x]++;
    grid.m_field[(y-1)*grid.width+x]++;
}

void Sandpile::all_field_skattering(uint32_t max_iter,uint32_t freq,char* path) {
    Node* now;
    uint32_t l_shift=0;
    uint32_t t_shift=0;
    bool is_added;
    Node * prev;
    int k=0;

    while (!grid.unstable.is_empty()) {
        now = grid.unstable.first;
        is_added=false;
        while (now != nullptr) {
            scattering(now->x+l_shift, now->y+t_shift, &l_shift, &t_shift);

            now->val -= 4;

            add_all_ways(now->x+l_shift,now->y+t_shift,&is_added,now,l_shift,t_shift);

            prev = now;
            now = now->next;
            if (prev->val < 4) {
                if (!is_added){
                    grid.unstable.first=now;
                }
                delete prev;
                continue;
            }
            grid.unstable.push_forward(prev);
            if (!is_added) {
                is_added = true;
                grid.unstable.first->next=nullptr;
            }
            delete prev;
        }
        if (max_iter!=0 && k>max_iter){
            break;
        }
        if (max_iter!=0 && k!=0 && k%freq) {
            Image bmp(grid.width, grid.height);
            bmp.Export(path, *this);
        }
        ++k;
    }
    Image bmp(grid.width, grid.height);
    bmp.Export(path, *this);
}

void Sandpile::add_all_ways(uint32_t x, uint32_t y, bool* is_added,Node* now,uint32_t l_shift,uint32_t t_shift) {
    if (get_value(x-1,y)>=4 && !have_node(x-1-l_shift,y-t_shift,now,grid.unstable.first,l_shift,t_shift)) {
        grid.unstable.push_forward(x - 1-l_shift, y-t_shift, get_value(x-1,y));
        if (!*is_added) {
            *is_added = true;
            grid.unstable.first->next=nullptr;
        }
    }

    if (get_value(x+1,y)>=4 && !have_node(x+1-l_shift,y-t_shift,now,grid.unstable.first,l_shift,t_shift)) {
        grid.unstable.push_forward(x+1-l_shift, y-t_shift, get_value(x+1,y));
        if (!*is_added) {
            *is_added = true;
            grid.unstable.first->next=nullptr;
        }
    }

    if (get_value(x,y+1)>=4 && !have_node(x-l_shift,y+1-t_shift,now,grid.unstable.first,l_shift,t_shift)) {
        grid.unstable.push_forward(x-l_shift, y+1-t_shift, get_value(x,y+1));
        if (!*is_added) {
            *is_added = true;
            grid.unstable.first->next=nullptr;
        }
    }

    if (get_value(x,y-1)>=4 && !have_node(x-l_shift,y-1-t_shift,now,grid.unstable.first,l_shift,t_shift)) {
        grid.unstable.push_forward(x-l_shift, y-1-t_shift, get_value(x,y-1));
        if (!*is_added) {
            *is_added = true;
            grid.unstable.first->next=nullptr;
        }
    }
}


bool Sandpile::have_node(int16_t x, int16_t y, Node * p, Node * now, uint32_t l, uint32_t t) {
    while (now != nullptr) {
        if (now->x == x && now->y == y) {
            now->val = get_value(x + l, y + t);
            return true;
        }
        now = now->next;
    }
    now = p;
    while (now != nullptr) {
        if (now->x == x && now->y == y) {
            now->val = get_value(x + l, y + t);
            return true;
        }
        now = now->next;
    }
    return false;
}


