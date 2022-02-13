#include "hw/l3_DomainLayer.h"

using namespace std;


bool Store::invariant() const {
        return _cost >= MIN_VALUE && _cost <= MAX_COST
            && _sizeapp >= MIN_VALUE && _sizeapp <= MAX_SIZE
            && _number_of_install >= MIN_VALUE
            && _mark >= MIN_VALUE && _mark <= MAX_MARK 
            && !_name_of_app.empty() && _name_of_app.size() <= MAX_NAME_LENGTH
            && !_category.empty() && _category.size() <= MAX_NAME_LENGTH;
}

bool Store::operator < (const Store & s) const {
    return _sizeapp < s.getSizeApp();
}

Store::Store(const std::string & name_of_app, const std::string & category, float cost, uint32_t sizeapp, uint32_t number_of_install, float mark)
        : _name_of_app(name_of_app), _category(category), _cost(cost), _sizeapp(sizeapp), _number_of_install(number_of_install), _mark(mark)
    {
        assert(invariant());
    }
Store::Store(const std::string & name_of_app, const std::string & category, float cost, uint32_t sizeapp, uint32_t number_of_install, float mark, set<Store> rec)
        : _name_of_app(name_of_app), _category(category), _cost(cost), _sizeapp(sizeapp), _number_of_install(number_of_install), _mark(mark), _rec(rec)
    {
        assert(invariant());
    }

const string & Store::getNameOfApp() const { return _name_of_app;}
const string & Store::getCategory() const { return _category;}
float Store::getCost() const { return _cost; }
uint32_t Store::getSizeApp() const { return _sizeapp; }
uint32_t Store::getNumberOfInstall() const { return _number_of_install; }
float Store::getMark() const { return _mark; }
const set<Store> & Store::getRec() const { return _rec; }

bool   Store::write(ostream& os) {
    writeString(os, _name_of_app);
    writeString(os, _category);
    writeNumber(os, _cost);
    writeNumber(os, _sizeapp);
    writeNumber(os, _number_of_install);
    writeNumber(os, _mark);

    return os.good();
}



void Store::addRec(Store s) {
    _rec.insert(s);
}

shared_ptr<ICollectable> ItemCollector::read(istream& is) {
    string   name_of_app = readString(is, MAX_NAME_LENGTH);
    string   category = readString(is, MAX_NAME_LENGTH);
    float    cost = readNumber<uint32_t>(is);
    uint32_t sizeapp = readNumber<uint32_t>(is);
    uint32_t number_of_install = readNumber<uint32_t>(is);
    float mark = readNumber<float>(is);
    set<Store> rec;
    size_t rec_size = readNumber<size_t>(is);
    for (size_t i = 0; i < rec_size; ++i) {
        string   name_of_app = readString(is, MAX_NAME_LENGTH);
        string   category = readString(is, MAX_NAME_LENGTH);
        float    cost = readNumber<uint32_t>(is);
        uint32_t sizeapp = readNumber<uint32_t>(is);
        uint32_t number_of_install = readNumber<uint32_t>(is);
        float mark = readNumber<float>(is);
        rec.emplace(name_of_app, category, cost, sizeapp, number_of_install, mark);
    }

    return make_shared<Store>(name_of_app, category, cost, sizeapp, number_of_install, mark, rec);
}

void History::addRecord(Store s) {
    _hist.insert(s);
}

set<Store> History::getSet() {
    return _hist;
}

void Recommend::initialise(){
    if (!_num_categories.empty()) {
        _num_categories.clear();
    }
    set<Store> st = getSet();
    for (set<Store>::const_iterator iter = st.begin(); iter != st.end(); ++iter) {
        string cat = iter->getCategory();
        if (_num_categories.find(cat) == _num_categories.end()) {
            _num_categories.insert(std::pair<string, int>(cat, 1));
        } else {
            ++_num_categories.find(cat)->second;
        }
    }
}

map<string,int> Recommend::getMap(){
    return _num_categories;
} 

vector<string> Recommend::maxCat(){
    vector<string> max_cat;
    map<string,int> map_straight = getMap();
    map<int,string> map_reverse;
    for (pair<string,int> pair : map_straight) {
        map_reverse[pair.second] = pair.first;
    }
    for (map<int,string>::reverse_iterator iter = map_reverse.rbegin(); iter != map_reverse.rend(); ++iter) {
        max_cat.push_back(iter->second);
    }
    return max_cat;
}

string Store::size_of_app() const {
    string strsize;
    int size = _sizeapp;
    if (size < 1024) {
        strsize = "б";
    }
    else if (size < 1048576) {
        size /= 1024;
        strsize = "Кб";
    }
    else if (size < 1073741824) {
        size /= 1048576;
        strsize = "Мб";
    }
    else {
        size /= 1073741824;
        strsize = "Гб";
    }
    return to_string(size)+strsize;
}
