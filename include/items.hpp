//
// Created by uberdever on 09.05.2020.
//

#ifndef CREN_ITEMS_HPP
#define CREN_ITEMS_HPP

#include "util.hpp"

namespace items
{
    using namespace util;
    using namespace math;

    //constexpr vars (items)
    const constexpr char* DBPath = "../data/db/items.xml";
    const constexpr char* attrsPath = "../data/db/items_properties.xml";
    constexpr int itemNameLen = 30;
    constexpr int itemDescLen = 200;
    constexpr int attrDescLen = 100;

    constexpr size_t tokenLen = 500;           //Length of token
    constexpr size_t dataLen = 500;            //Length of text data
    constexpr size_t tokenNameLen = 100;
    constexpr size_t attrNameLen = 100;
    constexpr size_t valueLen = 200;

    constexpr size_t attrIdLen = 10;

    constexpr int typeMaxNumber = 31;
    constexpr int attrMaxNumber = 211;

    constexpr int magicChance = (float)RAND_MAX * 0.4f;
    constexpr int rareChance = (float)RAND_MAX * 0.2f;
    constexpr int uniqueChance = (float)RAND_MAX * 0.05f;
    constexpr int intQualityChance = (float)RAND_MAX * 0.05f;

    constexpr float rareScale = 1.2f; //rare item attr values scaler

    constexpr int attrGrades = 20; //every n levels, there are new count of attrs, according to rarity
    constexpr int attrNums = 100 / attrGrades;
    const v2<int> attrNumM[attrNums] = {v2<int>{0, 1}, v2<int>{1, 1}, v2<int>{1, 2}, v2<int>{2, 2}, v2<int>{2, 3}}; //G L
    const v2<int> attrNumR[attrNums] = {v2<int>{1, 1}, v2<int>{1, 2}, v2<int>{2, 2}, v2<int>{2, 3}, v2<int>{4, 3}};

    //items enums
    enum ITEM_TYPES : uint32_t
    {
        NONE        = 0x00,
        THING       = 0x01,
        OHWEAP      = 0x02,
        THWEAP      = 0x04,
        CHEST       = 0x08,
        HELM        = 0x10,
        GLOVES      = 0x20,
        BOOTS       = 0x40,
        RING        = 0x80,
        AMULET      = 0x100,
        SHIELD      = 0x200,
        CONSUMABLE  = 0x400,
        QUEST       = 0x800,
        INTERNAL    = 0x1000,
        SIZE        = 13
    };

    enum ITEM_QUALITY : size_t
    {
        COMMON = 0,
        ENCHANTED,
        SPECIAL,
        UNIQUE,
    };

    //items structs
    struct ITEM_ATTR
    {
        char id[attrIdLen] {""}; char desc[attrDescLen] {0};
        v2<int> val; v2<int> cap; int minIlvl{0};
        float growth{0};
        bool single {true};

        bool operator==(ITEM_ATTR& rhs) const {return !(strcmp(id, rhs.id));}
        ITEM_ATTR& operator=(ITEM_ATTR const& rhs)
        {
            id[0] = 0; desc[0] = 0;
            strncat(id, rhs.id, attrIdLen); strncat(desc, rhs.desc, attrDescLen);
            val = rhs.val; cap = rhs.cap; minIlvl = rhs.minIlvl;
            growth = rhs.growth; single = rhs.single;
            return *this;
        }
    };

    struct BASE_ITEM
    {
        char name[itemNameLen] {"void"};
        char desc[itemDescLen] {"it is empty"};
        int ilvl{0}; int lvl{0};
        ITEM_TYPES type {THING};
        ITEM_QUALITY quality{COMMON};
        int cost{0}; int id{0};

        int sprite_id{0};
        v2<int> size {1, 1}; v2<int> pos {0, 0};

        list<ITEM_ATTR> attrs;

        BASE_ITEM& operator=(BASE_ITEM const& rhs)
        {
            name[0] = 0; desc[0] = 0;
            strncat(name, rhs.name, itemNameLen);
            strncat(desc, rhs.desc, itemDescLen);
            ilvl = rhs.ilvl; lvl = rhs.lvl;
            type = rhs.type; quality = rhs.quality;
            cost = rhs.cost; id = rhs.id;
            sprite_id = rhs.sprite_id;
            size = rhs.size; pos = rhs.pos;
            auto* itRhs = rhs.attrs.head;
            for (;itRhs; itRhs = itRhs->nx )
            {
                attrs.append(itRhs->data);
            }
            return *this;
        }
    };

    struct INV
    {
        list<BASE_ITEM> inv;
        int* invMask {nullptr};
        v2<int> size;
        int curID {0};

        BASE_ITEM voidItem;

        INV() = delete;

        INV(const int w, const int h): size(w, h)
        {
            inv.append(voidItem);
            invMask = new int[w * h];
            memset(invMask, -1, w * h * sizeof(int));
        }

        bool add(BASE_ITEM item)
        {
            bool isFree = false;
            v2<int> curPos;
            int y; int x;
            for (y = 0; (y <= size.y - item.size.y) && !isFree; y++)
                for (x = 0; (x <= size.x - item.size.x) && !isFree; x++)
                {
                    isFree = true;
                    for (int iy = 0; iy < item.size.y; iy++)
                        for (int ix = 0; ix < item.size.x; ix++)
                        {
                            if (invMask[(y * size.x) + x + (iy * size.x) + ix] != -1)
                            {
                                isFree = false;
                                break;
                            }
                        }
                }
            if (!isFree) return false;

            curPos = v2<int>{x - 1, y - 1};
            item.pos = curPos;
            int offset = curPos.y * size.x + curPos.x;
            for (int y = 0; y < item.size.y; y++)
                for (int x = 0; x < item.size.x; x++)
                {
                    invMask[(y * size.x) + x + offset] = curID;
                }

            curID++;
            inv.append(item);
            return true;
        }

        [[nodiscard]] BASE_ITEM& get_by_id(int id) const
        {
            auto* p = inv.head;
            for (int i = 0; i <= id; i++)
                p = p->nx;
            return p->data;
        }

        [[nodiscard]] BASE_ITEM& get(int x, int y) const
        {
            return get_by_id(invMask[y * size.x + x]);
        }

        void print() const
        {
            for (int y = 0; y < size.y; y++)
            {
                for (int x = 0; x < size.x; x++)
                {
                    if (invMask[y * size.x + x] == -1)
                        std::cout << 'X' << "  ";
                    else
                        std::cout << invMask[y * size.x + x] << "  ";
                }
                std::cout << std::endl;
            }
        }
    };

    BASE_ITEM constructItem(int ilvl, ITEM_TYPES possibleType);

    e_exitCodes items_init();
    void items_clean();
}

#endif //CREN_ITEMS_HPP
