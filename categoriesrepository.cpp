#include <QDir>
#include "categoriesrepository.h"

#include <QDebug>

CategoriesRepository::CategoriesRepository()
{ }

QSharedPointer<CategoriesRepository> CategoriesRepository::Instance()
{
    static auto instance = QSharedPointer<CategoriesRepository>(new CategoriesRepository());
    return instance;
}

void CategoriesRepository::loadCategoriesFromDir(QString root)
{
    _categories.clear();

    QDir dir = QDir(root);
    dir.setFilter(QDir::AllDirs);

    auto list = dir.entryList();
    foreach(auto dir, list) {
        if (dir == "." || dir == "..")
            continue;
        qDebug() << dir;
    }
}
