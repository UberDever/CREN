//
// Created by uberdever on 18.02.2020.
//

#ifndef CREN_UTIL_H
#define CREN_UTIL_H

#include "global.hpp"

//cfg - all configuration parameters, useful for application.

namespace math {template <typename T> struct v2;}

//util - all utility constructions, does not required to be in particular context, or used by system itself
//includes: enums, struct/class prototypes, general use functions
namespace util
{
    //constexpr vars
    constexpr int HASH_KEY_LEN = 50;
    constexpr int SDL_ParseCount = 8;

    //game enums
    enum e_exitCodes : uint8_t
    {
        OK              =  0x00,
        GUT_SDL_ERR     =  0x01,
        GUT_TTF_ERR     =  0x02,
        GUT_IMG_ERR     =  0x03,
        RES_TTF_ERR     =  0x04,
        RES_IMG_ERR     =  0x05,
        GAME_MAP_ERR    =  0x06,
        UI_PARSE_ERR    =  0x07,
        ITM_PARSE_ERR   =  0x08,
        ITM_ATTR_ERR    =  0x09,
        UNUSED2_ERR     =  0x10,
    };

    enum e_gameStates : size_t
    {
        EXIT      = 0x00,
        UI        = 0x01,
        GAMEPLAY  = 0x02,
        TEMP      = 0x03
    };

    /************************************************************/

    //scene prototype
    struct SCENE
    {
        SCENE() { init = nullptr; event = update = nullptr; render = nullptr; clean = nullptr;};

        e_exitCodes init_scene(e_exitCodes (*_init)(), e_gameStates (*_event)(), e_gameStates (*_update)(), void (*_render)(float), void (*_clean)())
        {
            init = _init;
            event = _event;
            update = _update;
            render = _render;
            clean = _clean;
            return init();
        };
        ~SCENE() { clean(); }

        e_exitCodes (*init)();
        e_gameStates (*event)();
        e_gameStates (*update)();
        void (*render)(float);
        void (*clean)();
    };

    //custom event struct
    struct GAMEEVENT
    {
        union {int mx; uint32_t key{};}; // mx is key in onKeyUp and onKeyDown events
        int my;
        int32_t mxRel; int32_t myRel;
        union {uint32_t mbutton; uint32_t scancode;};
        uint8_t type;
        uint8_t winEv;

        GAMEEVENT(): mx(0), my(0), mbutton(0), type(8), mxRel{0}, myRel{0}, winEv{0} {}
    };

    //first 128 keyboard keys, for elegant implementation of repeating
    static bool kbState[128];

    //union for direct access of colors values
    struct COLOR
    {
        union {
            uint32_t color;
            struct {uint8_t a; uint8_t b; uint8_t g; uint8_t r;};
        };
    };

    template <typename T>
    struct lNode
    {
        lNode* pr;
        lNode* nx;
        T data;

        lNode<T>(): pr{nullptr}, nx{nullptr} {}
        ~lNode<T>() = default;
    };

    template <typename T>
    struct list
    {
        lNode<T>* head;
        lNode<T>* tail;

        list<T>() : head{nullptr}, tail{nullptr} {}
        ~list<T>() {}

        bool is_empty() {return !head;}

        T* back() {
            if (is_empty()) return nullptr;
            return &tail->data;
        }

        void append(T& _data)
        {
            if (!head)
            {
                head = tail = new lNode<T>();
                tail->data = _data;
                return;
            }
            tail->nx = new lNode<T>();
            tail->nx->data = _data;
            tail->nx->pr = tail;
            tail = tail->nx;
        }

        void append(T&& _data)
        {
            if (!head)
            {
                head = tail = new lNode<T>();
                tail->data = _data;
                return;
            }
            tail->nx = new lNode<T>();
            tail->nx->data = _data;
            tail->nx->pr = tail;
            tail = tail->nx;
        }

        void clean()
        {
            if (is_empty()) return;
            auto* p = head;
            while (head)
            {
                p = p->nx;
                delete head;
                head = p;
            }
        }

        void clean_heap()
        {
            if (is_empty()) return;
            auto* p = head;
            while (head)
            {
                p = p->nx;
                delete head->data;
                head->data = nullptr;
                delete head;
                head = p;
            }
        }

        uint32_t size()
        {
            auto p = head;
            uint32_t size = 0;
            while (p) {p = p->nx; size++;}
            return size;
        }

        void print()
        {
            for (lNode<T>* it = head; it != nullptr; it = it->nx)
            {
                std::cout << it->data << " ";
            }
        }
    };

    template <typename T>
    struct vector
    {
        T* data {nullptr};
        size_t capacity {1};
        int load {0};

        vector() { data = new T[capacity]{}; };
        explicit vector(size_t _capacity) : capacity{_capacity}  {data = new T[capacity]{};}
        ~vector() {delete[] data;}

        bool is_empty()
        {
            return load <= 0;
        }

        bool is_full()
        {
            return load >= capacity;
        }

        void reserve(size_t _capacity)
        {
            std::cout << "Vector is resized (reserved)" << std::endl;
            delete[] data;
            capacity = _capacity;
            data = new T[capacity];
        }

        void resize(size_t _capacity)
        {
            std::cout << "Vector is resized" << std::endl;
            capacity = _capacity;
            T* tmpData = new T[capacity]{};
            memcpy(tmpData, data, load * sizeof(T));
            delete [] data;
            data = tmpData;
        }

        void push(T& el)
        {
            if (is_full())
            {
                std::cout << "Vector is resized (pushed)" << std::endl;
                resize(capacity * 2);
            }
            //std::cerr << "data is: " << (void*)data << std::endl;
            data[load] = el;
            load++;
        }

        /*void push(T&& el)
        {
            if (is_full())
            {
                capacity *= 2;
                T* tmpData = new T[capacity]{};
                memcpy(tmpData, data, load * sizeof(T));
                delete [] data;
                data = tmpData;
            }
            std::cout << "pushed" << std::endl;
            data[load] = el;
            load++;
        }*/

        T* pop()
        {
            if (is_empty()) return nullptr;
            return &data[--load];
        }

        T* tail()
        {
            if (is_empty()) return nullptr;
            return &data[load - 1];
        }

        T* operator[](int index)
        {
            if (index >= load || index < 0) {return nullptr;}
            return &data[index];
        }

        void shrink()
        {
            if (load <= 0) return;
            T* tmpData = new T[load]();
            memcpy(tmpData, data, load * sizeof(T));
            delete [] data;
            data = tmpData;
            capacity = load;
        }
    };

    template <typename T>
    struct hNode
    {
        T data;
        char key[HASH_KEY_LEN];
        bool isEmpty;
        bool isLast;

        hNode() : data{}, isEmpty{true}, isLast{false}, key{""} {}
        ~hNode() = default;

        hNode<T>& operator=(const T& rhs)
        {
            data = rhs;
            return *this;
        }
    };

    uint32_t hash(const char* data, size_t len);

    template <typename T>
    struct hashT
    {
        const size_t size;
        hNode<T>* table;

        explicit hashT(const uint32_t _size) : size{_size}, table{nullptr} { table = new hNode<T>[size]; }
        ~hashT() {delete[] table;}

        hNode<T>& operator[](const char* _key) //do NOT use this for retrieving the element
        {
            char key[HASH_KEY_LEN] {};
            strncat(key, _key, HASH_KEY_LEN - 1);
            uint32_t h = hash(key, strlen(key)) % size;
            uint32_t ind = 0; uint32_t probe = h;
            while (!table[probe].isEmpty) {
                table[probe].isLast = false;
                ind++;
                probe = (h + (ind * ind)) % size;
                if (probe == h)
                {
                    std::cout << "hashTable is full" << std::endl;
                    break;
                }
            }
            strncat(table[probe].key, key, HASH_KEY_LEN - 1);
            table[probe].isEmpty = false;
            table[probe].isLast = true;
            return table[probe];
        }

        T* search(const char* _key)
        {
            char key[HASH_KEY_LEN] {};
            strncat(key, _key, HASH_KEY_LEN - 1);
            uint32_t h = hash(key, strlen(key)) % size;
            if (table[h].isEmpty) return nullptr;
            uint32_t ind = 0;
            uint32_t probe = h;
            while (strcmp(table[probe].key, key))
            {
                if (table[probe].isLast)
                    return nullptr;
                ind++;
                probe = (h + (ind * ind)) % size;
            }
            //std::cout << table[probe].key << " -> " << h << std::endl;
            return &table[probe].data;
        }

        void clean()
        {
            for (int i = 0; i < size; i++)
            {
                delete table[i]->data;
                table[i]->isEmpty = true;
            }
        }
    };

    void parse_event(SDL_Event* rawEvent, GAMEEVENT* gameEvent);

    //generic use functions
    bool inField(int mx, int my, int px, int py, int sx, int sy);

    math::v2<int> parseV2(const char* str);

    template <typename T>
    void traverseTrie(lNode<T>* pEl, void (*pFunc)(T))
    {
        pFunc(pEl->data);
        if (!pEl->data->childNodes.is_empty())
        {
            traverseTrie(pEl->data->childNodes.head, pFunc);
        }
        if (pEl->nx)
            traverseTrie(pEl->nx, pFunc);
    }

    inline int randRange(int min, int max)
    {
        if (min == max) return max;
        if (!(min || max)) return 0;
        return rand() % (max - min) + min;
    }
}

//res - all technical outer world resource handling
namespace res
{
    struct FNT
    {
        TTF_Font* font;
        size_t size;
        uint32_t id;

        FNT(): font{nullptr}, size{0}, id{0} {}
        ~FNT() {TTF_CloseFont(font);}
    };

    TTF_Font* res_getFont(const char* name, uint32_t size);

    SDL_Surface* res_loadPNG(const char* path);
    TTF_Font* res_loadTTF(const char* path, uint32_t size);

    util::e_exitCodes res_init();
    void res_clean();
}

//math - all math, used in project (it can be not much, but it still required)
namespace math
{
    constexpr float PI = 3.14159265;
    //Vec2 struct and related functions
    template <typename T>
    struct v2
    {
        T x; T y;

        constexpr explicit v2(T _x, T _y) : x(_x), y(_y) {};
        v2() {x = y = 0;}

        inline T norm1() {return fabs(x) + fabs(y);}
        inline T norm2() {return sqrt(x * x + y * y);}
        inline v2<T> rotate(T ang) {return v2<T>{(this->x * cos(ang) - this->y * sin(ang)), (this->x * sin(ang) + this->y * cos(ang))};}

        inline v2<T> operator-() {return v2<T>{-this->x,-this->y};}

        inline v2<T>& operator=(T rhs) {this->x = rhs; this->y = rhs; return *this;}
        inline v2<T>& operator=(const v2<T> rhs) {this->x = rhs.x; this->y = rhs.y; return *this;}
        inline v2<T>& operator+=(const v2<T>&rhs) {this->x += rhs.x; this->y += rhs.y; return *this;}
        inline v2<T>& operator+=(const T rhs) {this->x += rhs; this->y += rhs; return *this;}
        inline v2<T>& operator-=(const v2<T>& rhs) {this->x -= rhs.x; this->y -= rhs.y; return *this;}
        inline v2<T>& operator-=(const T rhs) {this->x -= rhs; this->y -= rhs; return *this;}
        inline v2<T>& operator*=(const v2<T>& rhs) {this->x *= rhs.x; this->y *= rhs.y; return *this;} //vector-row multiplication
        inline v2<T>& operator*=(const T rhs) {this->x *= rhs; this->y *= rhs; return *this;}
        template <typename TC>
        inline explicit operator v2<TC>() const {return v2<TC>{static_cast<TC>(this->x), static_cast<TC>(this->y)};}
    };

    template <typename T>
    inline v2<T> operator+ (const v2<T>& lhs, const v2<T>& rhs)
    {
        return v2<T>{lhs.x + rhs.x, lhs.y + rhs.y};
    }

    template <typename T>
    inline v2<T> operator- (const v2<T>& lhs, const v2<T>& rhs)
    {
        return v2<T>{lhs.x - rhs.x, lhs.y - rhs.y};
    }

    template <typename T> //vector-row multiplication
    inline v2<T> operator^ (const v2<T>&lhs, const v2<T>&rhs)
    {
        return v2<T>{lhs.x * rhs.x, lhs.y * rhs.y};
    }

    template <typename T> //vector-scalar multiplication
    inline v2<T> operator* (const v2<T>&lhs, const T rhs)
    {
        return v2<T>{lhs.x * rhs, lhs.y * rhs};
    }

    template <typename T> //vector-scalar division
    inline v2<T> operator/ (const v2<T>&lhs, const T rhs)
    {
        return v2<T>{lhs.x / rhs, lhs.y / rhs};
    }

    template <typename T> //scalar multiplication
    inline T operator* (const v2<T>&lhs, const v2<T>& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const v2<T>& vec)
    {
        os << "X: " << +vec.x << " Y: " << +vec.y << " ";
        return os;
    }

    inline int log2(uint32_t num)
    {
        int counter = 0;
        while (num >>= 1) counter++;
        return counter;
    }

}

#endif //CREN_UTIL_H
