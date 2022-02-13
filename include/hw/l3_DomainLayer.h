#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include "hw/l4_InfrastructureLayer.h"
#include <set>
#include <map>

const size_t MAX_NAME_LENGTH     = 50;
const size_t MIN_VALUE           = 0;
const size_t MAX_COST            = 50000;
const size_t MAX_SIZE            = 4294967296;
const size_t MAX_MARK            = 5;

class Store : public ICollectable {
    std::string   _name_of_app;
    std::string   _category;
    float    _cost;
    uint32_t _sizeapp;
    uint32_t _number_of_install;
    float    _mark;
    std::set<Store> _rec; //set, в котором хранятся рекомендации пользователю
    
protected:
    bool invariant() const;

public:
    Store() = delete;

    Store & operator = (const Store & s) = delete;
    bool operator < (const Store & s) const;

    Store(const std::string & name_of_app, const std::string & category, float cost, uint32_t sizeapp, uint32_t number_of_install, float mark);
    Store(const std::string & name_of_app, const std::string & category, float cost, uint32_t sizeapp, uint32_t number_of_install, float mark, std::set<Store> rec);

    const std::string & getNameOfApp() const;
    const std::string & getCategory() const;
    float               getCost() const;
    uint32_t            getSizeApp() const;
    uint32_t            getNumberOfInstall() const;
    float               getMark() const;
    const std::set<Store> & getRec() const;

    void addRec(Store s);
    std::string size_of_app() const; //вывести размер приложения в более удобном формате
    virtual bool   write(std::ostream& os) override;
};

class ItemCollector: public ACollector
{
public:
    virtual std::shared_ptr<ICollectable> read(std::istream& is) override;
};

class History {
    std::set<Store> _hist; //set, в котором хранится история заказов
public:
    virtual void addRecord(Store s); //добавить в set заказ
    std::set<Store> getSet();
};

class Recommend : public History {
    std::map<std::string,int> _num_categories; //map, в котором хранится, какие категории и сколько раз выбрал пользователь
public:
    void initialise(); //инициализируем map значениями из set
    std::map<std::string,int> getMap();
    std::vector<std::string> maxCat(); //определить категории, которые чаще всего выбирал пользователь 
};

#endif // HW_L3_DOMAIN_LAYER_H
