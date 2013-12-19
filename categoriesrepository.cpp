#include <QDir>
#include "categoriesrepository.h"

#include <QDebug>

CategoriesRepository::CategoriesRepository()
{ }

CategoriesRepository *CategoriesRepository::Instance()
{
    static auto instance = new CategoriesRepository();
    return instance;
}

void CategoriesRepository::loadCategoriesFromDir(QString root)
{
    _categories.clear();

    QDir dir = QDir(root);
    dir.setFilter(QDir::AllDirs);

    auto list = dir.entryInfoList();
    foreach(QFileInfo dirInfo, list) {
        if (dirInfo.fileName() == "." || dirInfo.fileName() == "..")
            continue;
        auto category = Category::CreateFromDir(dirInfo.absolutePath());
        _categories.push_back(category);
    }
}

const QVector<Category::CategoryPtr> CategoriesRepository::listCategories() const
{ return _categories; }
