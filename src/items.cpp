//
// Created by uberdever on 09.05.2020.
//

#include "items.hpp"

namespace items
{
    hashT<vector<BASE_ITEM> *> *baseItems;
    hashT<vector<BASE_ITEM> *> *unqItems;

    hashT<vector<ITEM_ATTR> *> *lesserAttrs; //interfaces for type->attribute
    hashT<vector<ITEM_ATTR> *> *greaterAttrs;
    hashT<ITEM_ATTR *> *attrLib; //interface for id->attribute (for retrieval unique attrs)

    hashT<ITEM_TYPES> *itemTypes;
    const char *typeNames[ITEM_TYPES::SIZE]{"thing", "ohweap", "thweap", "chest", "helm", "gloves", "boots", "ring", "amulet", "shield", "consumable", "quest", "internal"};

    e_exitCodes parse_attrs()
    {
        FILE *in;
        in = fopen(attrsPath, "r");
        if (!in)
            return ITM_PARSE_ERR;
        hashT<vector<ITEM_ATTR> *> *curAttrsHashT = nullptr;
        vector<ITEM_ATTR> *curAttrs = nullptr;
        uint32_t id = 0;
        char token[tokenLen] = {0};
        char data[dataLen] = {0};
        size_t dataIndex = 0;
        while (true)
        {
            while (fscanf(in, "<%[^>]s", token) == 0)
            {
                data[dataIndex] = (char)getc(in);
                if (data[dataIndex] == '#')
                {
                    fscanf(in, "%*[^\n]\n");
                }
                dataIndex = (dataIndex + 1) % dataLen;
            }
            if (feof(in))
                break;
            data[dataIndex] = 0;
            if (token[0] == '/')
            {
                dataIndex = 0;
                continue;
            }
            else
            {
                uint32_t scanned = 0;
                char value[valueLen] = {};
                char name[tokenNameLen] = {};
                char attr[attrNameLen] = {};

                sscanf(token, "%s %n", name, &scanned);
                memcpy(token, token + scanned, strlen(token));

                uint32_t hashName = hash(name, strlen(name));
                switch (hashName)
                {
                case 3036281108: //greater_attrs
                {
                    greaterAttrs = new hashT<vector<ITEM_ATTR> *>(typeMaxNumber);
                    for (int i = 0; i < ITEM_TYPES::SIZE; i++)
                    {
                        (*greaterAttrs)[typeNames[i]] = new vector<ITEM_ATTR>(5);
                    }
                    (*greaterAttrs)["internal"] = new vector<ITEM_ATTR>(10);
                    curAttrsHashT = greaterAttrs;
                    break;
                }
                case 1391562357: //lesser_attrs
                {
                    lesserAttrs = new hashT<vector<ITEM_ATTR> *>(typeMaxNumber);
                    for (int i = 0; i < ITEM_TYPES::SIZE; i++)
                    {
                        (*lesserAttrs)[typeNames[i]] = new vector<ITEM_ATTR>(5);
                    }
                    (*lesserAttrs)["internal"] = new vector<ITEM_ATTR>(10);
                    curAttrsHashT = lesserAttrs;
                    break;
                }
                case 1504409333: //thing
                case 1462045896: //ohweap
                case 673390874:  //thweap
                case 405349056:  //chest
                case 2235868255: //helm
                case 1308970545: //gloves
                case 1147806260: //boots
                case 4005582228: //ring
                case 768211442:  //amulet
                case 3308515071: //shield
                case 1866109996: //consumable
                case 2986651522: //quest
                case 1700942598: //internal
                {
                    auto p = curAttrsHashT->search(name);
                    if (p)
                    {
                        curAttrs = *p;
                    }
                    else
                    {
                        std::cout << name << std::endl;
                        return ITM_ATTR_ERR;
                    }
                    break;
                }
                case 340446259: //attr
                {
                    curAttrs->load++;
                    if (curAttrs->is_full())
                    {
                        curAttrs->resize(curAttrs->capacity * 2);
                    }
                    break;
                }
                //case
                default:
                    std::cout << name << " token name\n";
                    return ITM_ATTR_ERR;
                }
                while (sscanf(token, "%[^=]s", attr) != -1)
                {
                    value[0] = 0;
                    sscanf(token, R"(%*[^"]"%n%[^"]s)", &scanned, value); //Raw string literal, from c++11
                    memcpy(token, token + scanned + strlen(value) + 2, strlen(token));
                    uint32_t hashAttr = hash(attr, strlen(attr));

                    //char* str = "attrs";
                    //std::cout << "name: " << name << " attr: " << attr << " value: " << value << " hash: " << hash(str, strlen(str)) << std::endl;

                    switch (hashAttr)
                    {              //only attr can have attrs
                    case 93303930: //id
                    {
                        strncat(curAttrs->tail()->id, value, attrIdLen);
                        break;
                    }
                    case 3881192334: //min_ilvl
                    {
                        curAttrs->tail()->minIlvl = atoi(value);
                        break;
                    }
                    case 1158842434: //growth
                    {
                        curAttrs->tail()->growth = strtof(value, nullptr);
                        break;
                    }
                    case 3477808853: //double
                    {
                        curAttrs->tail()->single = value[0] == 'f';
                        break;
                    }
                    case 1664273236: //val
                    {
                        curAttrs->tail()->val = parseV2(value);
                        break;
                    }
                    case 991029995: //cap
                    {
                        curAttrs->tail()->cap = parseV2(value);
                        break;
                    }
                    case 676451634: //str
                    {
                        strncat(curAttrs->tail()->desc, value, attrDescLen);
                        break;
                    }
                    default:
                        std::cout << name << " token name\n";
                        return ITM_ATTR_ERR;
                    }
                }
            }
            dataIndex = 0;
        }
        for (int i = 0; i < ITEM_TYPES::SIZE; i++)
        {
            (*(*greaterAttrs).search(typeNames[i]))->shrink();
            (*(*lesserAttrs).search(typeNames[i]))->shrink();
        }
        for (int i = 0; i < ITEM_TYPES::SIZE; i++)
        {
            auto *p = *greaterAttrs->search(typeNames[i]);
            for (int j = 0; j < p->load; j++)
            {
                (*attrLib)[(*p)[j]->id] = (*p)[j];
            }
        }
        for (int i = 0; i < ITEM_TYPES::SIZE; i++)
        {
            auto *p = *lesserAttrs->search(typeNames[i]);
            for (int j = 0; j < p->load; j++)
            {
                (*attrLib)[(*p)[j]->id] = (*p)[j];
            }
        }
        return OK;
    }

    e_exitCodes parse_items()
    {
        FILE *in;
        in = fopen(DBPath, "r");
        if (!in)
            return ITM_PARSE_ERR;
        hashT<vector<BASE_ITEM> *> *curItemsHashT = nullptr;
        vector<BASE_ITEM> *curItems = nullptr;
        ITEM_QUALITY curQuality = COMMON;
        char token[tokenLen] = {0};
        char data[dataLen] = {0};
        size_t dataIndex = 0;
        while (true)
        {
            while (fscanf(in, "<%[^>]s", token) == 0)
            {
                data[dataIndex] = (char)getc(in);
                if (data[dataIndex] == '#')
                {
                    fscanf(in, "%*[^\n]\n");
                }
                dataIndex = (dataIndex + 1) % dataLen;
            }
            if (feof(in))
                break;
            data[dataIndex] = 0;
            if (token[0] == '/')
            {
                if (hash(token, strlen(token)) == 341766032) // /item
                {
                    curItems->tail()->desc[0] = 0;
                    strncat(curItems->tail()->desc, data + 1, itemDescLen);
                }
                dataIndex = 0;
                continue;
            }
            else
            {
                uint32_t scanned = 0;
                char value[valueLen] = {};
                char name[tokenNameLen] = {};
                char attr[attrNameLen] = {};

                sscanf(token, "%s %n", name, &scanned);
                memcpy(token, token + scanned, strlen(token));

                uint32_t hashName = hash(name, strlen(name));
                switch (hashName)
                {
                case 1798481567: //base_items
                {
                    baseItems = new hashT<vector<BASE_ITEM> *>(typeMaxNumber);
                    for (int i = 0; i < ITEM_TYPES::SIZE; i++)
                    {
                        (*baseItems)[typeNames[i]] = new vector<BASE_ITEM>(10);
                    }
                    curItemsHashT = baseItems;
                    break;
                }
                case 3342854066: //unq_items
                {
                    unqItems = new hashT<vector<BASE_ITEM> *>(typeMaxNumber);
                    curQuality = UNIQUE;
                    for (int i = 0; i < ITEM_TYPES::SIZE; i++)
                    {
                        (*unqItems)[typeNames[i]] = new vector<BASE_ITEM>(10);
                    }
                    curItemsHashT = unqItems;
                    break;
                }
                case 1504409333: //thing
                case 1462045896: //ohweap
                case 673390874:  //thweap
                case 405349056:  //chest
                case 2235868255: //helm
                case 1308970545: //gloves
                case 1147806260: //boots
                case 4005582228: //ring
                case 768211442:  //amulet
                case 3308515071: //shield
                case 1866109996: //consumable
                case 2986651522: //quest
                {
                    auto p = curItemsHashT->search(name);
                    if (p)
                    {
                        curItems = *p;
                    }
                    else
                    {
                        std::cout << name << std::endl;
                        return ITM_PARSE_ERR;
                    }
                    break;
                }
                case 3892294984: //item
                {

                    curItems->load++;
                    if (curItems->load >= curItems->capacity)
                        curItems->reserve(curItems->capacity * 2);
                    curItems->tail()->quality = curQuality;
                    break;
                }
                default:
                    std::cout << name << " token name\n";
                    return ITM_PARSE_ERR;
                }
                while (sscanf(token, "%[^=]s", attr) != -1)
                {
                    value[0] = 0;
                    sscanf(token, R"(%*[^"]"%n%[^"]s)", &scanned, value); //Raw string literal, from c++11
                    memcpy(token, token + scanned + strlen(value) + 2, strlen(token));
                    uint32_t hashAttr = hash(attr, strlen(attr));

                    //char* str = "size";
                    //std::cout << "name: " << name << " attr: " << attr << " value: " << value << " hash: " << hash(str, strlen(str)) << std::endl;

                    switch (hashAttr)
                    {                //only item can have attributes
                    case 3241396435: //name
                    {
                        curItems->tail()->name[0] = 0;
                        strncat(curItems->tail()->name, value, itemNameLen);
                        break;
                    }
                    case 1255330124: //size
                    {
                        curItems->tail()->size = parseV2(value);
                        break;
                    }
                    case 1611040044: //sprite_id
                    {
                        char path[100] = {"../data/sprites/items/"};
                        strcat(path, value);

                        curItems->tail()->sprite_id = atoi(value);
                        break;
                    }
                    case 26091019: //ilvl
                    {
                        curItems->tail()->ilvl = atoi(value);
                        break;
                    }
                    case 1377543050: //lvl
                    {
                        curItems->tail()->lvl = atoi(value);
                        break;
                    }
                    case 1780700712: //type
                    {
                        auto *p = itemTypes->search(value);
                        if (p)
                            curItems->tail()->type = *p;
                        else
                            return ITM_PARSE_ERR;
                        break;
                    }
                    case 2140652628: //quality
                    {
                        switch (hash(value, strlen(value)))
                        {
                        case 4292097054:
                            curItems->tail()->quality = COMMON;
                            break;
                        case 1084529594:
                            curItems->tail()->quality = ENCHANTED;
                            break;
                        case 837296561:
                            curItems->tail()->quality = SPECIAL;
                            break;
                        case 1180910182:
                            curItems->tail()->quality = UNIQUE;
                            break;
                        default:
                            std::cout << "quality\n";
                            return ITM_PARSE_ERR;
                        }
                        break;
                    }
                    case 1731429204: //attrs
                    {
                        int k = 1;
                        int index = 0;
                        char tmpStr[10]{0};
                        while (k)
                        {
                            k = 0;
                            sscanf(value, "%s%n", tmpStr, &k);

                            if (k)
                            {
                                memcpy(value, value + k, strlen(value));
                                curItems->tail()->attrs.append(**attrLib->search(tmpStr));
                                tmpStr[0] = 0;
                            }
                        }
                        break;
                    }
                    case 955530522: //cost
                    {
                        curItems->tail()->cost = atoi(value);
                        break;
                    }
                    default:
                        std::cout << name << " token name\n";
                        return ITM_PARSE_ERR;
                    }
                }
            }
            dataIndex = 0;
        }
        for (int i = 0; i < ITEM_TYPES::SIZE; i++)
        {
            (*(*baseItems).search(typeNames[i]))->shrink();
            (*(*unqItems).search(typeNames[i]))->shrink();
        }
        return OK;
    }

    void parseAttr(ITEM_ATTR &attr, int val)
    {
        char buf[10]{};
        int len = strlen(attr.desc);
        sprintf(buf, "%d", val);
        for (int ind = 0; ind < len; ind++)
        {
            if (attr.desc[ind] == '&')
            {
                memcpy(attr.desc + ind + strlen(buf) - 2, attr.desc + ind - 1, strlen(attr.desc) - ind + 1);
                memcpy(attr.desc + ind, buf, strlen(buf));
                break;
            }
        }
    }

    void setAttrVal(ITEM_ATTR &attr)
    {
        if (attr.val.x > attr.cap.x)
            attr.val.x = attr.cap.x;
        if (attr.val.y > attr.cap.y)
            attr.val.y = attr.cap.y;
        if (attr.single)
        {
            attr.val.x = randRange(attr.val.x, attr.val.y);
            attr.val.y = 0;
            parseAttr(attr, attr.val.x);
        }
        else
        {
            const int rangeSize = rand() % 20 - 10;
            int range = (attr.val.x * rangeSize) / 100;
            if (range)
                attr.val.x += (rand() % range);
            range = (attr.val.y * rangeSize) / 100;
            if (range)
                attr.val.y += (rand() % range);
            if (attr.val.x > attr.val.y)
            {
                auto temp = attr.val.x;
                attr.val.x = attr.val.y;
                attr.val.y = temp;
            }
            parseAttr(attr, attr.val.x);
            parseAttr(attr, attr.val.y);
        }
    }

    BASE_ITEM constructItem(int ilvl, ITEM_TYPES possibleType)
    {
        int curChance = rand();
        BASE_ITEM item;
        auto resultType = static_cast<ITEM_TYPES>(1 << 31);
        vector<ITEM_TYPES> types(ITEM_TYPES::SIZE);
        while (resultType)
        {
            if (!(resultType & possibleType))
            {
                resultType = static_cast<ITEM_TYPES>(resultType >> 1);
                continue;
            }
            types.push(resultType);
            resultType = static_cast<ITEM_TYPES>(resultType >> 1);
        }
        resultType = *types[curChance / (RAND_MAX / types.load)];
        vector<BASE_ITEM> *itemVec = *baseItems->search(typeNames[math::log2(resultType)]);
        item = *(*itemVec)[curChance % itemVec->load];
        for (auto *p = item.attrs.head; p; p = p->nx)
            p->data.val += (int)(ilvl * p->data.growth);
        if (rand() < intQualityChance && (resultType >= OHWEAP && resultType <= HELM))
        {
            char tmpName[6] = {"intQ"};
            int index = rand() % 3 + 1;
            tmpName[4] = index + '0';
            item.attrs.append(**attrLib->search(tmpName));
            int tmpVal[3] = {item.attrs.tail->data.val.x, item.attrs.tail->data.val.y, (int)item.attrs.tail->data.growth};
            auto *p = item.attrs.head;
            for (int i = 0; p != item.attrs.tail; p = p->nx, i++)
            {
                p->data.val += (p->data.val * tmpVal[i]) / 100;
            }
        }
        for (auto *p = item.attrs.head; p; p = p->nx)
            setAttrVal(p->data);
        if (curChance < magicChance)
        {
            curChance = rand();
            if (curChance < uniqueChance)
            {
                itemVec = *unqItems->search(typeNames[math::log2(resultType)]);
                item = *(*itemVec)[curChance % itemVec->load];
                return item;
            }
            hashT<vector<ITEM_ATTR> *> *curAttrVecT = greaterAttrs;
            vector<ITEM_ATTR> *attrVec{nullptr}; //*greaterAttrs->search(typeNames[math::log2(resultType)]);
            int grade = ilvl / attrGrades;
            if (grade > (attrNums - 1))
                grade = attrNums - 1;
            int attrNum = attrNumM[grade].x;
            float scale = 1.f;

            item.quality = ENCHANTED;
            if (curChance < rareChance)
            {
                item.quality = SPECIAL;
                attrNum = attrNumR[grade].x;
                scale = rareScale;
            }
            for (int flip = 0; flip < 2; flip++)
            {
                list<uint32_t> curAttrs;
                for (int i = 0; i < attrNum; i++)
                {
                    attrVec = rand() < RAND_MAX / 2 ? *curAttrVecT->search(typeNames[math::log2(resultType)]) : *curAttrVecT->search("thing");
                    ITEM_ATTR attr = *(*attrVec)[rand() % attrVec->load];
                    bool isRepeat = true;
                    while (isRepeat)
                    {
                        isRepeat = false;
                        for (auto *it = curAttrs.head; it; it = it->nx)
                        {
                            if (hash(attr.id, strlen(attr.id)) == it->data)
                            {
                                attr = *(*attrVec)[rand() % attrVec->load];
                                isRepeat = true;
                                break;
                            }
                        }
                    }
                    curAttrs.append(hash(attr.id, strlen(attr.id)));
                    item.attrs.append(attr);
                    item.attrs.back()->val += (int)(ilvl * attr.growth * scale);
                    setAttrVal(*item.attrs.back());
                }
                curAttrVecT = lesserAttrs;
                attrNum = scale == rareScale ? attrNumR[grade].y : attrNumM[grade].y;
                curAttrs.clean();
            }
        }
        return item;
    }

    e_exitCodes items_init()
    {
        itemTypes = new hashT<ITEM_TYPES>(typeMaxNumber);
        attrLib = new hashT<ITEM_ATTR *>(attrMaxNumber);

        for (int i = 0; i < ITEM_TYPES::SIZE; i++)
            (*itemTypes)[typeNames[i]] = static_cast<const ITEM_TYPES>(1 << i);
        e_exitCodes exitCode = parse_attrs();
        exitCode = static_cast<e_exitCodes>(exitCode + parse_items());

        for (int i = 0; i < 20; i++)
        {
            BASE_ITEM item = constructItem(30, static_cast<ITEM_TYPES>(THING | OHWEAP | CHEST | HELM)); //THING | OHWEAP | THWEAP |
            if (item.quality == UNIQUE)
                std::cout << "UNIQUE: ";
            else if (item.quality == ENCHANTED)
                std::cout << "ENCHANTED: ";
            else if (item.quality == SPECIAL)
                std::cout << "SPECIAL: ";
            else
            {
                std::cout << "COMMON: ";
            };
            //std::cout << item.name << std::endl;
            if (strcmp(item.name, "oh1") == 0)
            {
                std::cout << "One handed" << std::endl;
            }
            else if (strcmp(item.name, "th1") == 0)
            {
                std::cout << "Two handed" << std::endl;
            }
            else if (strcmp(item.name, "ch1") == 0)
            {
                std::cout << "Body armor" << std::endl;
            }
            else if (strcmp(item.name, "hm1") == 0)
            {
                std::cout << "Helmet" << std::endl;
            }
            else if (strcmp(item.name, "gl1") == 0)
            {
                std::cout << "Gloves" << std::endl;
            }
            else if (strcmp(item.name, "bt1") == 0)
            {
                std::cout << "Boots" << std::endl;
            }
            else if (strcmp(item.name, "r1") == 0)
            {
                std::cout << "Ring" << std::endl;
            }
            else if (strcmp(item.name, "a1") == 0)
            {
                std::cout << "Amulet" << std::endl;
            }
            else if (strcmp(item.name, "s1") == 0)
            {
                std::cout << "Shield" << std::endl;
            }
            else if (strcmp(item.name, "c1") == 0)
            {
                std::cout << "Consumable" << std::endl;
            }
            else if (strcmp(item.name, "q1") == 0)
            {
                std::cout << "Quest" << std::endl;
            }
            else if (strcmp(item.name, "itm1") == 0)
            {
                std::cout << "Item 1" << std::endl;
            }
            else if (strcmp(item.name, "itm2") == 0)
            {
                std::cout << "Item 2" << std::endl;
            }
            else if (strcmp(item.name, "itm3") == 0)
            {
                std::cout << "Item 3" << std::endl;
            }
            for (auto &it = item.attrs.head; it; it = it->nx)
            {
                std::cout << it->data.desc << std::endl;
            }
            std::cout << std::endl;
        }
        return exitCode;
    }

    void items_clean()
    {
        delete itemTypes;
        for (int i = 0; i < ITEM_TYPES::SIZE; i++)
        {
            delete *baseItems->search(typeNames[i]);
            *baseItems->search(typeNames[i]) = nullptr;
            delete *unqItems->search(typeNames[i]);
            *unqItems->search(typeNames[i]) = nullptr;
            delete *greaterAttrs->search(typeNames[i]);
            *greaterAttrs->search(typeNames[i]) = nullptr;
            delete *lesserAttrs->search(typeNames[i]);
            *lesserAttrs->search(typeNames[i]) = nullptr;
        }
        //TODO ITEM CLEAN
    }
} // namespace items
