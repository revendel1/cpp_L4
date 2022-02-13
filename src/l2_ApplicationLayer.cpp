#include "hw/l2_ApplicationLayer.h"

using namespace std;
bool was_recommendations = false;

bool Application::performCommand(const vector<string> & args)
{    

    if (args.empty())
        return false;

    if (args[0] == "l" || args[0] == "load")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!_col.loadCollection(filename))
        {
            _out.Output("Ошибка при загрузке файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "s" || args[0] == "save")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!_col.saveCollection(filename))
        {
            _out.Output("Ошибка при сохранении файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "c" || args[0] == "clean")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды clean");
            return false;
        }

        _col.clean();

        return true;
    }

    if (args[0] == "a" || args[0] == "add")
    {
        if (args.size() != 7)
        {
            _out.Output("Некорректное количество аргументов команды add");
            return false;
        }

        _col.addItem(make_shared<Store>(args[1].c_str(), args[2].c_str(), stof(args[3]), stoul(args[4]), stoul(args[5]), stof(args[6])));
        Store h(args[1].c_str(), args[2].c_str(), stof(args[3]), stoul(args[4]), stoul(args[5]), stof(args[6]));
        historia.addRecord(h); //заказ пользователя добавляется также в историю заказов
        return true;
    }

    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды remove");
            return false;
        }

        _col.removeItem(stoul(args[1]));
        return true;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 8)
        {
            _out.Output("Некорректное количество аргументов команды update");
            return false;
        }

        _col.updateItem(stoul(args[1]), make_shared<Store>(args[2].c_str(), args[3].c_str(), stof(args[4]), stoul(args[5]), stoul(args[6]), stof(args[7])));
        return true;
    }

    if (args[0]== "ar" || args[0] == "addrecommendation") {
        if (args.size() != 8) {
            _out.Output("Некорректное количество аргументов команды addrecomendation");
            return false;
        }

        Store & s = static_cast<Store &>(*_col.getItem(stoul(args[1])));

        Store app(args[2], args[3], stof(args[4]), stoul(args[5]), stoul(args[6]), stof(args[7]));
        s.addRec(app); //добавляем рекомендации касательно приложения
        was_recommendations = true;
        return true;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды view");
            return false;
        }

        size_t count = 0;
        for(size_t i=0; i < _col.getSize(); ++i)
        {
            const Store & item = static_cast<Store &>(*_col.getItem(i));

            if (!_col.isRemoved(i))
            {
                _out.Output("[" + to_string(i) + "] "
                        + item.getNameOfApp() + " "
                        + item.getCategory() + " "
                        + to_string(item.getCost()) + " "
                        + item.size_of_app() + " "
                        + to_string(item.getNumberOfInstall()) + " "
                        + to_string(item.getMark()));
                
                const set<Store> & recs = item.getRec();
                if (!recs.empty()) { //если есть рекомендации, то выводим их
                    _out.Output("    Вам также может понравиться это:");
                    for (set<Store>::const_iterator iter = recs.begin(); iter != recs.end(); ++iter) {
                        _out.Output("        "+ iter->getNameOfApp() + " " + iter->getCategory() + " " + to_string(iter->getCost()) 
                        + " " + iter->size_of_app() + " " + to_string(iter->getNumberOfInstall()) + " " + to_string(iter->getMark()));
                    }                
                }  
                
                count ++;
            }
        
        }
        if (was_recommendations) {
            set<Store> historia2 = historia.getSet();
            if (!historia2.empty()){ 
                //если есть история заказов, то выводим 2 любимые категории пользователя
                historia.initialise();
                vector<string> hist = historia.maxCat();
                if (hist.size() > 2) {
                    _out.Output("Ваши любимые категории приложений: " + 
                        hist[0] + ", " + hist[1]); 
                } else {
                    _out.Output("Ваша любимая категория приложений: " + hist[0]);
                }
                
            }
        }
        
        _out.Output("Количество элементов в коллекции: " + to_string(count));
        return true;
    }

    _out.Output("Недопустимая команда '" + args[0] + "'");
    return false;
}
